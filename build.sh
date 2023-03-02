#!/bin/bash
# copy the file to raspberry pi, and compile it, then run it
#
# Define program name here
PROGNAME=rtcapp

# Define Raspberry Pi credentials
RPI_USER=pi
RPI_IP=raspberrypi.lan
RPI_PATH=/home/pi/ee513

# Define local project directory
LOCAL_DIR=$(dirname "$(readlink -f $0)")/

function upload() {
	echo '--------------------- Transfering files to raspberry pi ---------------------'
	# copy current folder to home dir of pi@raspberrypi.lan via ssh
	rsync -avz --exclude '.git' $LOCAL_DIR $RPI_USER@$RPI_IP:$RPI_PATH
}

function compile() {
	echo '--------------------- Compiling on raspberry pi ---------------------'
	# compile the file on raspberry pi via ssh
	ssh $RPI_USER@$RPI_IP "cd $RPI_PATH;make clean; make"
}

function run() {
	echo '--------------------- Running on raspberry pi ---------------------'
	# run the file on raspberry pi via ssh
	ssh $RPI_USER@$RPI_IP "cd $RPI_PATH; build/$PROGNAME"
}

function help() {
	echo 'Usage: ./build.sh [options] [RPI_IP]'
	echo 'Options:'
	echo '  -u: upload'
	echo '  -c: compile'
	echo '  -r: run'
	echo '  -h: help'
	echo 'Example:'
	echo '  ./build.sh -ucr'
}

# Check if there is any argument
if [ $# -eq 0 ]; then
	help
	exit 1
fi

# Last argument is the RPi IP address
# If no argument is given, use the default value
tmp=${@: -1}
if [[ $tmp != -* ]]; then
	RPI_IP=$tmp
fi

while getopts ":ucrh" opt; do
	case $opt in
	u)
		upload
		;;
	c)
		compile
		;;
	r)
		run
		;;
	h)
		help
		;;
	\?)
		echo "Invalid option: -$OPTARG" >&2
		;;
	esac
done
