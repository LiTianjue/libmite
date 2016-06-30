#!/bin/bash

max=30
log_dir=/home/andy/tmp
file_type="*.log"
#del_args1=-mtime
#del_args2=+2

percent=`df /var | tail -1 | awk '{print $5 }' | cut -d'%' -f1`

echo "disk usage : $percent%"

if [ "${percent}" -ge "${max}" ]; then
	echo "need to remove something"
	find $log_dir -name $file_type $del_args1 $del_args2 -exec rm -rf {} \;

else
	echo "disk run OK"
fi;
