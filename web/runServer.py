from flask import Flask, render_template
from jinja2 import Undefined
from handleFunctionRequest import HandleFunctionRequest
import sys
import serial
import time
import threading

if sys.version_info[0] < 3:
    raise Exception("Python 3 or a more recent version is required.")

app = Flask(__name__)

@app.route("/")
def index():
    return render_template("index.html")

# Disable caching
@app.after_request
def add_header(r):
    r.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    r.headers["Pragma"] = "no-cache"
    r.headers["Expires"] = "0"
    r.headers['Cache-Control'] = 'public, max-age=0'
    return r

@app.route("/ExecuteFunction/", methods=['POST'])
def execute_function():
    print("")
    #handlePySAIWebRequest = HandleFunctionRequest("someFunction", [1]); 

if __name__ == '__main__':
    app.run(debug=True, port=2223, host='0.0.0.0')