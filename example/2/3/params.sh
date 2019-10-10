#!/bin/sh

salutation="Hello"

echo $salutation
echo "The program $0 is now running"
echo "The program second param $2"
echo "The program first param $1"
echo "The program param list $*"
echo "The user's home directory is $HOME"

echo "Please enter a now greeting"
read salutation

echo $salutation
echo "The script is now complete"
exit 0