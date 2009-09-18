/** drive.c
 *
 * controld.c
 *   control the Sparky robot by reading /tmp/robot.lock file commands
 * 	commands are written to /tmp/robot.lock by SkypePlugin or SparkyListener TCP
 *   configuration: control.hdf
 *
 * Author John Celenza... Adapted from drive.c
 *
 *  A simple program to drive the create using the keyboard. 
 *
 *  Author: Nathan Sprague
 *
 * 
 *  This file is part of COIL.
 *
 *  COIL is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  COIL is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with COIL.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */


#include <createoi.h>
#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <math.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "string.h"

#include "clearsilver/util/neo_hdf.h"

#define LMAX(a,b)	(a > b? a : b)
#define LMIN(a,b)	(a < b? a : b)
#define SONG_LENGTH 8

void move_dir(char *action, HDF *config);

/* Plays some random notes on the create's speakers. */
void horn() {
  char song[SONG_LENGTH * 2];
  int i;
  for (i =0; i< SONG_LENGTH; i++) {
    song[i*2] = 31 + floor(drand48() * (127-31));
    song[i*2 +1] = 8 + floor(drand48() * 10);
  }
  writeSong(0, SONG_LENGTH, song);
  playSong (0);
}


//WINDOW *win;
int main(int argv, char* argc[])
{
	int key;
	int not_done = 1;



	if (argv < 2) {
	  fprintf(stderr, "Usage: drive DEVICE (e.g. /dev/ttyUSB0)\n");
	  exit(1);
	}
	
	
	//setup ncurses
	//initscr();
	//clear();
	//noecho();
	//cbreak();    	
	//win = newwin(24, 80, 0, 0);
	//keypad(win, TRUE);

     
	startOI_MT (argc[1]);

	//NEW MAIN LOOP
	time_t last_command_epoch = time(NULL);
	char last_action[1024];
	char last_action_hero[1024];

	strcpy(last_action,"stop");

	while (not_done)
	{
		HDF *config = NULL;
		char command[1024];
		char action[1024];
		struct stat mystat;
		FILE *in_action = NULL;
		int time_elapsed = 0;

		hdf_init(&config);
		hdf_read_file(config, "control.hdf");

		//touch the lock file
		sprintf(command,"touch /tmp/SparkyController.lock; chmod a+rw /tmp/SparkyController.lock");
		system(command);

        	*action = 0;

		if (!stat("/tmp/SparkyController.lock", &mystat))
		{
        		in_action=fopen("/tmp/SparkyController.lock","r");
		}

        	if (in_action)
        	{
			fscanf(in_action,"%s",action);
			fscanf(in_action,"%d",&last_command_epoch);
//printf ("READ last_command_epoch: %d\n",last_command_epoch);

                	fclose(in_action);

//printf("last_action:%s action:%s\n", last_action, action);
                	if (strcmp(last_action, action) || mystat.st_mtime < time_elapsed )
                	{
//printf("file changed or command changed\n");
//echo "setting last_command_epoch".time();
				strcpy(last_action, action);
                        	last_command_epoch = time(NULL);
                        	time_elapsed = 0;
				strcpy(last_action_hero,"update");
                	}
                	else
                	{
//printf ("SETTING time_elapsed = %d - %d\n", time(NULL), last_command_epoch);
                        	time_elapsed = time(NULL) - last_command_epoch;
                	}

                	//don't write again
/*
                	if (strncmp(action,"stop",strlen("stop")))
                	{
//printf("setting time_elapsed=0\n");
                        	time_elapsed = 0;
                	}
*/

	//printf("checking control timeout:  time_elapsed: %d last_command_epoch:%d time:%d\n",time_elapsed, last_command_epoch, time(NULL));
                	if (time_elapsed > hdf_get_int_value(config,"control.timeout",1) && strcmp(action,"stop"))
                	{

	//printf("control timeout: setting stop time_elapsed: %d last_command_epoch:%d time:%d\n",time_elapsed, last_command_epoch, time(NULL));
				strcpy(action, "stop");
                        	if (1)
                        	{
					FILE *out_action;
                        		umask(0777);
	
                       			out_action = fopen("/tmp/SparkyController.lock","w");
                        		if (out_action)
                        		{
                                		fprintf(out_action,"%s",action);
                                		fclose(out_action);
                        		}
                		}
	
			}

                }

		int show_ui = 1;
//DO MAIN SUMULATION LOOP STUFF HERE
/*

        if (0 != bumper)                                                        //sensor was tripped
        {
                if (1 == bumper)
                {
                        drive (120, 1);
                        waitAngle (45, 1);
                }
                else if (2 == bumper)
                {
                        drive (120, -1);
                        waitAngle (-45, 1);
                }
                else if (3 == bumper)
                {
                        drive (250, -1);
                        waitAngle (-90, 1);
                }
        }
*/





	//STOP ON BUMPER
        int angle, tol = 20, bumper = 0;
        bumper = getBumpsAndWheelDrops();
	if (bumper!=0 && !strstr(action,"move_reverse"))
	{
printf("bump! drive(0,0)\n");
		horn();
		drive(1, 1);
	}
	else if (strlen(action) && strcmp(action, last_action_hero))
		{
        		show_ui = 0;
			//printf("%s\n",action);
		
			//echo "Sending ACTION $action to SC_move_dir\n";
//printf("Sending action:%s\n");
        		move_dir(action,config);
				//horn();
		}
	strcpy(last_action_hero, action);

//printf("loop! last_action_hero:%s\n", last_action_hero);
        usleep(100000);
	}

	//clrtoeol();
	//refresh();
	//endwin();
	stopOI_MT();

}
		
void move_dir(char *action, HDF *config)
{
		static int velocity = 0;
		static int speed = 0;//store user input.
		static int turn = 0;
	
		static int radius = 0;
	
		static int charge;

		float leftPercent = 0, rightPercent = 0;
	
		erase();
		charge = getCharge();
		printf("Battery Charge: %d%%", charge);
		printf("%d %d", velocity, radius);
		//refresh();

		if (strstr(action,"move_left"))
		{
			//turn =  LMAX(turn + 1, 0);
			radius = -50;	
			velocity = 50;
		}
		else if (strstr(action,"move_right"))
		{
			turn =  LMIN(turn - 1, 0);
			velocity = 50;
			radius = 50;	
		}
		else if (strstr(action,"move_straight"))
		{
			if (speed < 0) {
				speed = 0;
				turn = 0;
			} else {
				speed += 50;
			}

			velocity = 50;
			radius = 0;	
		}
		else if (strstr(action,"move_reverse"))
		{
			if (speed > 0) {
				speed = 0;
				turn = 0;
			} else {
				speed -= 50;
			}
			velocity = -50;
			speed = -50;
			radius = 0;	
		}
		else if (strstr(action,"stop"))
		{
			speed = 1;
			radius = 1;
			velocity = 0;
		}

		//match move_<DIR>_L_<PERCENT>_R_<PERCENT>
//printf("*************************action:%s\n",action);
		if (sscanf(action,"move_%*[^_]_L_%f_R_%f", &leftPercent, &rightPercent)==2)
		{
			if(fabs(leftPercent - rightPercent) > 0.1)
				radius = (-1.0/(leftPercent - rightPercent)) * 256.0;
			velocity = ((fabs(leftPercent) + fabs(rightPercent)) / 2.0) * 256.0;
			if ((leftPercent + rightPercent) < 0)
				velocity = -velocity;
				
				
printf("moving high fidelity radius:%d velocity:%d leftPercent:%.2f rightPercent:%.2f\n",radius, velocity,leftPercent, rightPercent);
		}

#if 0
		key = wgetch(win);
		
		switch(key){
		case KEY_UP:
			if (speed < 0) {
				speed = 0;
				turn = 0;
			} else {
				speed += 50;
			}
			break;
		case KEY_DOWN:
			if (speed > 0) {
				speed = 0;
				turn = 0;
			} else {
				speed -= 50;
			}
			break;
		case KEY_LEFT:
			turn =  LMAX(turn + 1, 0);
			break;
		case KEY_RIGHT:
			turn =  LMIN(turn - 1, 0);
			break;
		case 'h':
			horn();
			break;
		case 'q':
			not_done = 0;
		}
		

		if (speed != 0){
			velocity = speed;
			if (turn != 0)
				radius = (abs(turn) / turn) * 
					LMAX(1000 /pow(2,abs(turn)), 1);
			else 
				radius = 0;
		} else if (turn != 0) { /* turn in place*/
			velocity = abs(turn) * 50;
			if (turn > 0) 
				radius = 1;
			if (turn < 0)
				radius = -1;
		} else {
			velocity = 0;
			radius = 0;
		}
		
#endif
printf("drive (velocity=%d, radius=%d\n",velocity, radius);
		drive(velocity, radius);
		
		
}


#if 0

<?

/*

controld.php

control sparky robot servos through makeboard and osc

Author: John Celenza

*/

require("hdf.php");
require("OSC.phps");


/* get_control_arg

	take control argument in the form "_reverse0_<value>"

	returns value
*/
function get_control_arg($value_str)
{
	$value = "";

	$args = explode("_",$value_str);
	$value = $args[2];

	echo "get_control_arg value_str:$value_str value:$value\n";

	return $value;
}

function sc_move_dir($c,$dir_with_args,$config)
{
	echo "$dir\n";

	$c->send(new OSCMessage("/appled/1/state", array(1)));

	$dir_args = explode("_",$dir_with_args);

	if (strstr($dir_with_args,"_slow"))
		$slow_str = "_slow";
	else
		$slow_str = "";

	$sleep_length = 2;
	//_time_<seconds>
	if (($time_val_str = strstr($dir_with_args,"_time")))
	{
		$sleep_length = substr($time_val_str,6,1);
		echo "Got a time $time_val_str sleep_length:$sleep_length\n";
	}

	$dir = "$dir_args[0]_$dir_args[1]$slow_str";

	echo "primary_command:$dir\n";

	$servo_0 = -1;
	$servo_1 = -1;

	if ($val_str = strstr($dir_with_args,"_straight0"))
		$servo_0 = get_control_arg($val_str);
	else if ($val_str = strstr($dir_with_args,"_reverse0"))
		$servo_0 = get_control_arg($val_str);
	if ($val_str = strstr($dir_with_args,"_straight1"))
		$servo_1 = get_control_arg($val_str);
	else if ($val_str = strstr($dir_with_args,"_reverse1"))
		$servo_1 = get_control_arg($val_str);

	if ($servo_0 < 0)
		$servo_0 = hdf_get_value($config,"movement.$dir.0",0);
	if ($servo_1 < 0)
		$servo_1 = hdf_get_value($config,"movement.$dir.1",0);

		

	$stop_0 = hdf_get_value($config,"movement.stop.0",0);
	$stop_1 = hdf_get_value($config,"movement.stop.1",0);

	if ($dir_with_args=="stop")
	{
		$servo_0 = $stop_0;
		$servo_1 = $stop_1;
	}

echo "sc_move_dir:CONTROL::::::::::::::::::::::::::::::::::::::\n";
echo "sc_move_dir:servo_0:".intval($servo_0)."\n";
echo "sc_move_dir:servo_1:".intval($servo_1)."\n";
echo "sc_move_dir:stop_0: $stop_0\n";
echo "sc_move_dir:stop_1: $stop_1\n";
echo "sleep_length:$sleep_length\n";

	$c->send(new OSCMessage("/servo/1/position", array(intval($servo_0))));
	$c->send(new OSCMessage("/servo/0/position", array(intval($servo_1))));
	//sleep($sleep_length);
	//$c->send(new OSCMessage("/servo/0/position", array(intval($stop_0))));
	//$c->send(new OSCMessage("/servo/1/position", array(intval($stop_1))));

	//$c->send(new OSCMessage("/appled/1/active", array(0)));
	$c->send(new OSCMessage("/appled/1/state", array(0)));

	return;
}

////////////////////////////
////MAIN
////////////////////////////

$config = hdf_read_file("control.hdf");
$last_command_epoch = time();
$last_action = "stop";
while (1)
{

	`touch /tmp/SparkyController.lock; chmod a+rw /tmp/SparkyController.lock`;

$c = new OSCClient();
$c->set_destination("192.168.0.200", 10000);
	$action = "";

	$mystat = 0;
	$mystat = stat("/tmp/SparkyController.lock");

	$in_action=fopen("/tmp/SparkyController.lock","r");
	

	if ($in_action)
	{
		$action = ltrim(rtrim(fgets($in_action)));
		$last_command_epoch = fgets($in_action);
echo "READ last_command_epoch: $last_command_epoch\n";

		if (!strlen($last_command_epoch))
			$last_command_epoch = 0;

		fclose($in_action);

		if ($last_action != $action)
		{
//echo "setting last_command_epoch".time();
			$last_action = $action;
			$last_command_epoch = time();
			$time_elapsed = 0;
		}
		else
		{
			$time_elapsed = time(NULL) - $last_command_epoch;
		}

		//don't write again
		if ($action=="stop")
		{
			$time_elapsed = 0;
		}

		if ($time_elapsed > hdf_get_value($config,"control.timeout",1) && $action!="stop")
		{
//echo "control timeout: setting stop time_elapsed: $time_elapsed last_command_epoch:$last_command_epoch time:".time()."\n";
			$action = "stop";
			if (1)
			{
			umask(0777);
			$out_action = fopen("/tmp/SparkyController.lock","w");
			if ($out_action)
			{
				fputs($out_action,"$action");
				fclose($out_action);
			}
			}
		}
	}

$show_ui = 1;

if (strlen($action) && $action!=$last_action_hero)
{
	$show_ui = 0;
	echo "$action\n";

/*
	if ($action=="stop")
	{
//echo "Sending STOP 2x\n";
		SC_move_dir($c,$action,$config);
		usleep(100000);
	}
*/
	
//echo "Sending ACTION $action to SC_move_dir\n";
	SC_move_dir($c,$action,$config);
}


	$last_action_hero = $action;
	usleep(100000);

};

#endif
