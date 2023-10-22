#!/bin/sh
case $1 in
    "prepare")
        if [ "$(whoami)" != "root" ]
        then
            sudo apt-get update && sudo apt-get install -yq curl wget bzip2 tar ca-certificates make python3 python3-pip 
            sudo curl "https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh" -o arduino-cli-install.sh
            sudo bash -c "BINDIR=/usr/bin sh arduino-cli-install.sh"
            mkdir ~/.arduino15
            touch ~/.arduino15/package_esp8266com_index.json
            arduino-cli config init
            arduino-cli config add board_manager.additional_urls "http://arduino.esp8266.com/stable/package_esp8266com_index.json"
            arduino-cli config add board_manager.additional_urls "https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json"
            arduino-cli config add board_manager.additional_urls "https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json"
            arduino-cli update
            pip3 install -r requirements.txt
            sudo chmod +x ./build.sh
        else
            apt-get update && apt-get install -yq curl wget bzip2 tar ca-certificates make python3 python3-pip 
            curl "https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh" -o arduino-cli-install.sh
            bash -c "BINDIR=/usr/bin sh arduino-cli-install.sh"
            mkdir /root/.arduino15
            touch /root/.arduino15/package_esp8266com_index.json
            arduino-cli config init
            arduino-cli config add board_manager.additional_urls "http://arduino.esp8266.com/stable/package_esp8266com_index.json"
            arduino-cli config add board_manager.additional_urls "https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json"
            arduino-cli config add board_manager.additional_urls "https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json"
            arduino-cli update
            pip3 install -r requirements.txt
            chmod +x ./build.sh
        fi
        ;;
    "build")
        make all
        ;;

    "test")
        echo nothing to do for test...
        ;;

    *)
        echo unknown stage $1...
        ;;

esac
