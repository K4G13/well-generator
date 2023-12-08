from flask import Flask, jsonify, request
import subprocess

app = Flask(__name__)

@app.route('/', methods=['GET'])
def home():
    return '''Try in console: <br><br>
                &emsp;fetch('http://127.0.0.1:2137/api/data', { <br>
                    &emsp;&emsp;method: 'POST',<br>
                    &emsp;&emsp;headers: {<br>
                        &emsp;&emsp;&emsp;'Content-Type': 'application/json',<br>
                    &emsp;&emsp;},<br>
                    &emsp;&emsp;body: JSON.stringify({
                        "args": "4 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32"})<br>
                    &emsp;}) '''

@app.route('/api/data', methods=['POST'])
def receive_data():
    data = request.get_json()
    # TODO walidacja danych wejściowych
    args = data["args"].split()
    try:
        result = subprocess.check_output( ["./well1024a"] + args ).decode('utf-8').split()
        return jsonify(result)
    except subprocess.CalledProcessError as e:
        return jsonify(
            status="error",
            returncode=str(e.returncode),
            output=str(e.output)
        ), 500

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=2137)