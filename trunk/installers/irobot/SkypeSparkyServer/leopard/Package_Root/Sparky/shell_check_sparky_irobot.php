#!/bin/bash
#check if controld.php is running
#
# should probably be executed from cron in a crontab entry like:
#
# * * * * * php /Sparky/shell_check_sparky.php &> /dev/null
#

NUM=`ps ax | grep controld | grep -v grep | wc -l`

if [ "$NUM" -lt 1 ]
then
	echo "`date` Restarting controld" > /tmp/controld.log
	cd /Sparky/
	./controld &> /tmp/controld.run.log &
fi
