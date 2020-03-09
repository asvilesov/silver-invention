"""EE 250L Lab 10 Local Flask Server

Run this file to start a local HTTP web server (i.e. it is only available to 
processes running on your local machine). The web server simply saves any 
JSON objects that are sent via an HTTP POST request. If you point a web browser 
to 0.0.0.0:5000/log, which sends a HTTP GET request, the web server will reply
with all the stored JSON objects. Run http_client_example.py to generate example
HTTP POST requests and then open up your browser to the URL to see the output of
the web server.

"""

from flask import Flask
from flask import jsonify
from flask import request
from datetime import datetime
import json

app = Flask('Signal Processing Event Log Server')

@app.route('/log', methods=['GET'])
def print_log_callback():
    # Get our current log and use flask's `jsonify()` method to format the log 
    # in a neat manner for displaying on a web browser
    response = jsonify(log.get())

    # the object returned will be sent back as an http message to the requester
    return response 

# This callback will be run when an HTTP POST request is sent to the URL
# "http://0.0.0.0:5000/post-event"
@app.route('/post-event', methods=['POST'])
def add_event_callback():
    # the object that stores all the http message data (header, payload, etc.). .
    payload = request.get_json()
    print(payload)

    # assuming the payload is formatted exactly as we expect, we will append the
    # event to our log. The event object should be in a dictionary-like format 
    # with the key value pairs like in the example below.
    # 
    # payload = {
    #     'time': str(datetime.now()),
    #     'event': "Moving Right"
    # }
    log.append_event(payload)

    response = {
        'Return': "Event logged."
    }

    # the object returned will be sent back as an http response message to the requester
    return json.dumps(response)

# A class to manage a list to avoid using a global list
# warning: this may not be thread-safe, although this should work for our assignment
class SignalProcessingEventLog(object):
    def __init__(self):
        self._signal_processing_events = []
    def append_event(self, event):
        self._signal_processing_events.append(event)
    def get(self):
        return self._signal_processing_events

if __name__ == '__main__':
    # create an instance of the event log
    # warning: we are not limiting the size of this log
    log = SignalProcessingEventLog()

    """Run the flask web application on '0.0.0.0'. The default port is 5000. 
    Flask is not multi-threaded by nature and will hang when many HTTP messages
    are received, so we also ask flask to enable threading."""
    app.run(threaded = True, host = '0.0.0.0') 

    """Note: Because flask is binding to 0.0.0.0, you will not be able to 
    connect to this web server from outside your local machine. We do not need
    to do this in lab 08, but we will explore this in your """
