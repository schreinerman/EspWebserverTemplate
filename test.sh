#!/bin/sh

port_esp8266=/dev/tty.usbmodem-14124
port_esp32=/dev/tty.usbmodem-14123

SUCCESS=1

if [ -d "./myfirstwebserver" ]
then
  rm -R myfirstwebserver
fi

python wifimcu-genproject.py --name "myfirstwebserver" --conf "example_app.json"
cd myfirstwebserver
make all

if [ "$?" == "0" ]
then
  echo "make all: Success"
else
  echo "make all: Error"
  SUCCESS=0
fi

echo ""
echo ""
echo "------------------------------------------------"
echo "  REPORT"
echo "------------------------------------------------"
echo ""
echo ""

if [ -f "./release/firmware-v1.0.0_esp8266_example-app.bin" ]
then
  echo ESP8266 binary: Success
  if [ -d $port_esp8266 ]
  then
    echo "  -> Copy to target..."
    python3 -m esptool --port $port_esp8266 --baud 115200 write_flash 0x0 ./release/firmware-v1.0.0_esp8266_example-app.bin
  fi
else
  echo ESP8266 binary: Error
  SUCCESS=0
fi

if [ -f "./release/firmware-v1.0.0_esp32_example-app.bin" ]
then
  echo ESP32 binary: Success
  if [ -d $port_esp32 ]
  then
    echo "  -> Copy to target..."
    python3 -m esptool --port $port_esp32 --baud 115200 write_flash -z 0x1000 ./release/firmware-v1.0.0_esp32_example-app.bin
  fi
else
  echo ESP32 binary: Error
  SUCCESS=0
fi

if [ -f "./release/firmware-v1.0.0_rp2040_example-app.uf2" ]
then
  echo RP2040 binary: Success
  if [ -d /Volumes/RPI-RP2 ]
  then
    echo "  -> Copy to target..."
    cp ./release/firmware-v1.0.0_rp2040_example-app.uf2 /Volumes/RPI-RP2/
  fi
else
  echo RP2040 binary: Error
  SUCCESS=0
fi

cd ..

if [ "$SUCCESS" == "0" ]
then
  echo Overall: Failed
else
  echo Overall: Successful compiled
fi

echo "Waiting 2 mins before deleting test-artifacts..."
sleep 120

if [ "$SUCCESS" == "1" ]
then
    if [ -d "./myfirstwebserver" ]
    then
    rm -R myfirstwebserver
    fi
fi