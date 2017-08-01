#!/bin/bash
couchdb &> /dev/null &
while true; do
   python /root/I2C/bin/update.py &> /dev/null & sleep 5
done
