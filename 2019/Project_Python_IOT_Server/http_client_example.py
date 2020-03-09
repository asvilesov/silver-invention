"""EE 250L Lab 10 HTTP Client Example 

Run this file after starting up a local flask server via flask_server.py to
create example HTTP post requests.
"""

import requests
import json
from datetime import datetime
import time

if __name__ == '__main__':

    # This header sets the HTTP request's mimetype to `application/json`. This
    # means the payload of the HTTP message will be formatted as a json ojbect
    hdr = {
        'Content-Type': 'application/json',
        'Authorization': None #not using HTTP secure
    }

    while True:
        # The payload of our message starts as a simple dictionary. Before sending
        # the HTTP message, we will format this into a json object
        payload = {
            'time': str(datetime.now()),
            'event': "Moving Right"
        }

        # Send an HTTP POST message and block until a response is given.
        # Note: requests() is NOT the same thing as request() under the flask 
        # library.
        response = requests.post("http://0.0.0.0:5000/post-event", headers = hdr,
                                 data = json.dumps(payload))

        # Print the json object from the HTTP response
        print(response.json())

        time.sleep(2)

