#!/bin/sh
case $1 in

    "build")
        make esp8266
        ;;

    "test")
        echo nothing to do for test...
        ;;

    *)
        echo unknown stage $1...
        ;;

esac
