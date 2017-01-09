# !/bin/bash

COUNTER=0

while [ $COUNTER -lt 1 ]; do
	ls -l media/ | wc -l
	sleep 1
done
