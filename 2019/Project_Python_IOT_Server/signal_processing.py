#EE 250L Lab 10 Signal Processing

This file is the starter code for the lab assignment.

TODO: Andrew Schwefler, Alex Vilesov


import paho.mqtt.client as mqtt
import time
import requests
import json
from datetime import datetime

# MQTT variables
broker_hostname = "eclipse.usc.edu"
broker_port = 11000

#uncomment these lines to subscribe to real-time published data
ultrasonic_ranger1_topic = "ultrasonic_ranger1/real_data"
ultrasonic_ranger2_topic = "ultrasonic_ranger2/real_data"

#uncomment these lines to subscribe to recorded data being played back in a loop
#ultrasonic_ranger1_topic = "ultrasonic_ranger1/fake_data"
#ultrasonic_ranger2_topic = "ultrasonic_ranger2/fake_data"

# Lists holding the ultrasonic ranger sensor distance readings. Change the 
# value of MAX_LIST_LENGTH depending on how many distance samples you would 
# like to keep at any point in time.
MAX_LIST_LENGTH = 100
ranger1_dist = []
ranger2_dist = []

def derivative(data_points):
    slopes = [0]*4
    for x in range(4):
        slopes[x] = int(data_points[x]) - int(data_points[x+1])
    avg_deriv = sum(slopes)
    return avg_deriv

def average_buffer(list):
    average = 0
    for x in range(10):
        average = average + float(list[x])
    average = average/10
    return average

def ranger1_callback(client, userdata, msg):
    global ranger1_dist
    ranger1_dist.append(int(msg.payload))
    #truncate list to only have the last MAX_LIST_LENGTH values
    ranger1_dist = ranger1_dist[-MAX_LIST_LENGTH:]

def ranger2_callback(client, userdata, msg):
    global ranger2_dist
    ranger2_dist.append(int(msg.payload))
    #truncate list to only have the last MAX_LIST_LENGTH values
    ranger2_dist = ranger2_dist[-MAX_LIST_LENGTH:]

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))
    client.subscribe(ultrasonic_ranger1_topic)
    client.message_callback_add(ultrasonic_ranger1_topic, ranger1_callback)
    client.subscribe(ultrasonic_ranger2_topic)
    client.message_callback_add(ultrasonic_ranger2_topic, ranger2_callback)

# The callback for when a PUBLISH message is received from the server.
# This should not be called.
def on_message(client, userdata, msg): 
    print(msg.topic + " " + str(msg.payload))

if __name__ == '__main__':
    # Connect to broker and start loop    
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect(broker_hostname, broker_port, 60)
    client.loop_start()

    buffer_10_1 = ["0"]*10
    buffer_10_2 = ["0"]*10
    buffer_10_1_time = ["0"]*10
    buffer_10_2_time = ["0"]*10
    iteration = 0
    sensor_1_flag = 0
    sensor_2_flag = 0
    state = 0
    temp_state = 0
    counter = 0

    hdr = {
        'Content-Type': 'application/json',
        'Authorization': None #not using HTTP secure
    }

    payload = {
        'time': str(datetime.now()),
        'event': "Starting up"
    }

    while True:
        iteration = iteration + 1 #to avoid one off error
        """ two lists, ranger1_dist and ranger2_dist, which hold a window
        of the past MAX_LIST_LENGTH samples published by ultrasonic ranger 1
        and 2, respectively. The signals are published roughly at intervals of
        200ms, or 5 samples/second (5 Hz). The values published are the 
        distances in centimeters to the closest object. The measurements will vary 
        between 0-700. Jumps in values will most likely be from 
        inter-sensor-interference.
        """
        
        time.sleep(.2)


        while(iteration > 1):
        	#print("ranger1: " + str(ranger1_dist[-1:]) + ", ranger2: " + str(ranger2_dist[-1:])) 

        
        	time.sleep(.2)
        	iteration = iteration +1

     		#gets rid of the brackets that are transmitted through MQTT
	        s = list(str(ranger1_dist[-1:]))
	        length = len(s)
	        s.pop(length-1)
	        s.pop(0)
	        new_dist_1 = "".join(s)
	        #print(new_dist)
	        s = list(str(ranger2_dist[-1:]))
	        length = len(s)
	        s.pop(length-1)
	        s.pop(0)
	        new_dist_2 = "".join(s)


	        #puts any incoming values, into two buffers
	        	#1 buffer_10_1_time is the real time buffer
	        	#2 buffer_10_1 is the moving average buffer
	        if(int(new_dist_1) < 300):
	        	moving_average = 0
		        for x in range(0,9):
		        	buffer_10_1[9-x] = buffer_10_1[8-x]
		        	pass
		        buffer_10_1[0] = new_dist_1
		        for x in range(0,10):
		        	moving_average = moving_average + int(buffer_10_1[x])
		        buffer_10_1[0] = float(moving_average)/10

	        if(int(new_dist_1) < 300):
		        for x in range(0,9):
		        	buffer_10_1_time[9-x] = buffer_10_1_time[8-x]
		        	pass
		        buffer_10_1_time[0] = new_dist_1

		    #creates a buffer, made up of derivatives from the moving average buffer
	        average_deriv_1 = derivative(buffer_10_1)
	        #print("Derivative 1:" + str(average_deriv_1))

	        #same but for second rangefinder
	        if(int(new_dist_2) < 300):
	        	moving_average = 0
		        for x in range(0,9):
		        	buffer_10_2[9-x] = buffer_10_2[8-x]
		        	pass
		        buffer_10_2[0] = new_dist_2
		        for x in range(0,10):
		        	moving_average = moving_average + int(buffer_10_2[x])
		        buffer_10_2[0] = float(moving_average)/10

	        if(int(new_dist_2) < 300):
		        for x in range(0,9):
		        	buffer_10_2_time[9-x] = buffer_10_2_time[8-x]
		        	pass
		        buffer_10_2_time[0] = new_dist_2
		    

	        average_deriv_2 = derivative(buffer_10_2)
	        #print("Derivative 2:" + str(average_deriv_2))

	        #let the buffers fill up for 15 iterations to avoid any errors in the derivative and moving average
	        if (iteration <= 15):
	        	print("_")

	        if(iteration > 15):

	        	#140 cm is the determined threshold to see if someone is in front of the rangefinder
	        	#we poll for one second to avoid saying someone is still, when a person is actually moving
		        if(average_buffer(buffer_10_1_time) < 140):
		        	sensor_1_flag = sensor_1_flag + 1
		        else:
		            sensor_1_flag = 0
		        if(sensor_1_flag > 5):
		        	print("Still - Left")
		        	sensor_1_flag = 0
		        	payload = {
	            		'time': str(datetime.now()),
	            		'event': "Still at Left"
	        		}
	        		response = requests.post("http://0.0.0.0:5000/post-event", headers = hdr, data = json.dumps(payload))
		        
		        if(average_buffer(buffer_10_2_time) < 140):
		        	sensor_2_flag = sensor_2_flag + 1
		        else:
		            sensor_2_flag = 0
		        if(sensor_2_flag > 5):
		        	print("Still - Right")
		        	sensor_2_flag = 0
		        	payload = {
	            		'time': str(datetime.now()),
	            		'event': "Still at Right"
	        		}
	        		response = requests.post("http://0.0.0.0:5000/post-event", headers = hdr, data = json.dumps(payload))

        		#temp_state initialized to 0
        			#if either derivative is less than a threshold value set equal to -1
        		# if temp_state is -1, then state can be updated	

	        	#state 1 means movement at rangefinder one first
	        	#state 2 means movement at rangefinder two first

	        	# depending on which state was first initialzed we can tell
	        	# where the movement began

	        	counter = counter + 1

		        if(average_deriv_1 < -8):
		        	temp_state = -1
		        
		        if(temp_state == -1):
		        	if (state != 2):
		        		state = 1
	        		else:
	        			print("Movement towards left")
	        			state = 0
	        			temp_state = 0
	        			payload = {
	            			'time': str(datetime.now()),
	            			'event': "Moving Left"
	        			}
	        			response = requests.post("http://0.0.0.0:5000/post-event", headers = hdr, data = json.dumps(payload))

	        	if(average_deriv_2 < -8):
		        	temp_state = -2

		        if(temp_state == -2):

		        	if (state != 1):
		        		state = 2
		        		temp_state = 0
	        		else:
	        			print("Movement towards right")
	        			state = 0
	        			payload = {
	            			'time': str(datetime.now()),
	            			'event': "Moving Right"
	        			}
	        			response = requests.post("http://0.0.0.0:5000/post-event", headers = hdr, data = json.dumps(payload))
	        		
	        		







