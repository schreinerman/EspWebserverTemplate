#!/bin/sh
case $1 in

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
