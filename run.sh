#!/bin/bash
# copy the file to raspberry pi, and compile it, then run it
###
dir_name=$(basename "$(pwd)")
echo '--------------------- Transfering files to raspberry pi ---------------------'
ssh pi@raspberrypi "rm -rf /home/pi/$dir_name"
# copy current folder to home dir of pi@raspberrypi.lan via ssh
scp -r ../$dir_name pi@raspberrypi.lan:/home/pi/

echo '--------------------- Compiling on raspberry pi ---------------------'
# compile the file on raspberry pi via ssh
ssh pi@raspberrypi.lan "cd /home/pi/$dir_name; g++ -o test src/*.cpp"

echo '--------------------- Running on raspberry pi ---------------------'
# run the file on raspberry pi via ssh
ssh pi@raspberrypi.lan "cd /home/pi/$dir_name; ./test"
