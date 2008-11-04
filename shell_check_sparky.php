#!/bin/bash

#check if controld.php is running

NUM=`ps ax | grep controld.php | grep -v grep | wc -l`

if [ "$NUM" -lt 1 ]
then
	echo "`date` Restarting controld.php" > /tmp/controld.php.log
	cd /Sparky/
	php /Sparky/controld.php &> /tmp/controld.php.run.log &
fi
