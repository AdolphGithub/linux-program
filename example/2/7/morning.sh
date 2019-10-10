#!/bin/sh

echo "Is it morning? Please answer yes or no"
read timeofday

case "$timeofday" in
    yes | y | Yes | YES) 
        echo "Good morning"
        ;;
    [nN]*)  
        echo "Good afternoon"
        ;;
    *)   
        echo "Sorry, answer not recognized"
        exit 1
        ;;
esac

exit 0