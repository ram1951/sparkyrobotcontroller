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

//TODO support control.hdf file
/*
	How to get a value from the control.hdf file

	char *value = hdf_get_value(config,"KEY NAME", "DEFAULT KEY VALUE IF KEY DOES NOT EXIST");

*/
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


