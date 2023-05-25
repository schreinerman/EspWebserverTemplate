# ESP Webserver Template

This project generates a new Arduino project containing everything for a simple WebServer application.

You can use ESP32, ESP8266 or Raspberry Pi Pico W

In the file named `example_app.json` is containing already some hints, how to specify variables which can be configured via webinterface.

To create a new Arduino project `myfirstwebserver` following steps are required:
```
python -m pip install -r requirements.txt
python wifimcu-genproject.py --name "myfirstwebserver" --conf "example_app.json"
```

After putting your website into the html folder, you need to navigate into the projects root folder and run `python create_web_store.py` before building and uploading the project into the ESP8266 / EPS32.


