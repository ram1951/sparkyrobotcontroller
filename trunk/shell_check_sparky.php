#!/bin/bash
#check if controld.php is running
#
# should probably be executed from cron in a crontab entry like:
#
# * * * * * php /Sparky/shell_check_sparky.php &> /dev/null
#

NUM=`ps ax | grep controld.php | grep -v grep | wc -l`

if [ "$NUM" -lt 1 ]
then
	echo "`date` Restarting controld.php" > /tmp/controld.php.log
	cd /Sparky/
	php /Sparky/controld.php &> /tmp/controld.php.run.log &
fi
