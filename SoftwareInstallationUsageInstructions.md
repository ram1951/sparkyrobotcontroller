Sparky Software Kit 1.0

Author: John Celenza

Project Home:
http://code.google.com/p/sparkyrobotcontroller/

# Installation Instructions for "iRobot Create" Version #
  * You will need to install Sparky\_Server.dmg on the MacMini connected to the robot
    * once installed you will need to also install a serial connection between the MacMini and the iRobot
      * the software has been tested using the Keyspan High Speed USB Serial Adapter
    * after installing the serial adapter, reboot the MacMini
    * upon booting, start Skype and Sparky\_Server (there should be a desktop link)
    * Press the connect button on Sparky\_Server to connect that program to Skype
  * You will need to install Sparky\_Joystick.dmg on any computer that will "drive" the robot
    * once the software is installed connect and turn on a joystick, we have tested with the PS3 controller
    * start Sparky Joystick and Skype
    * connect from Sparky Joystick to Skype using the connect button
    * finally, call the Robot

> Please see http://www.sparkyjr.com/page/instructions-1 for more complete installation instructions.

# Installation Instructions for Make Controller Version #

## Overview ##
There are several pieces of software that need to be installed and configured:

  1. Sparky Skype Server -- runs along with Skype and listens for incoming text messages with motor commands
  1. Sparky MakeBoard Controller -- communicates with the Skype Listener, parses abstract motor commands, sends low-level commands to MakeBoard
  1. Sparky Controller Skype Plugin -- runs along with Skype on any Mac OSX 10.4 and greater machine which you want to use drive your robot

Once you have these software packages installed and configured, you are ready to go

## Sparky Skype Server ##

  * Download/Install Skype onto the robot computer
  * Download the Sparky Skpe Listener Plugin from http://code.google.com/p/sparkyrobotcontroller/.
  * Drop this plugin application anywhere on your system, the Desktop is perfectly fine
  * Whenever you use your robot, you will want to start Skype, then start the listener plugin
  * Once the listener plugin begins running, press the "Connect" button. You should see a success message. If you do not, make sure you have started Skype.

## Sparky MakeBoard Controller ##
### Pre-reqs: ###
  * You will need to know a little about the "Terminal" program to install the listener and prepare it to run automatically on startup
  * Your make board must be connected by ethernet and be configured to run at 192.168.0.200

  * Download the MakeBoard Controller files from http://code.google.com/p/sparkyrobotcontroller
  * There are 3 files to download, put these in /Sparky on your robot system:
    * control.hdf -- confiuration file that maps a control string like "move\_left" to servo actions
    * controld.php -- PHP script that listens for commands from the Skype plugin
    * shell\_check\_sprarky.php -- PHP script the makes sure controld.php is always running
    * OSC.phps -- Open Sound control library to interface with MakeController
    * hdf.php -- Hierarchal Data Format reader (http://clearsilver.org)

  * edit control.hdf to configure the move commands as your wish. By default, these commands are support:
```
  move_straight
  move_straight_slow
  move_left
  move_left_slow
  move_right
  move_right_slow
  move_reverse
  move_reverse_slow
  stop
```
  * add the check script to your cron scheduler so controld.php starts on system startup:
  * open a Terminal (From Applications->Utilities->Terminal)
  * type "crontab -e" -- this will start the VI editor, if you do not know how to use VI, try looking on Google, or just follow these steps:
  * move to the last line of the file with arrow-keys
  * type "i"
  * paste:
```
 * * * * * php /Sparky/shell_check_sparky.php &> /dev/null
```
  * type ":wq" (which should appear at the bottom of the screen)

## Sparky Controller Skype Plugin ##
On the machine from which you want to drive your robot:
  * install Skype.
  * goto http://code.google.com/p/sparkyrobotcontroller and download the "Sparky Controller Skype Plugin".
  * Install this plugin application anywhere on your system, the Desktop is fine.
When you start Skype, start this application second. Then click "Connect", you should see a confirmation that the plugin is connected to your Skype software.


# Opertation Instructions #
Follow these steps to get your robot running once you have installed all of the software.

## On the robot ##
  1. Make sure you have a working wireless network that the robot can join
  1. Start up your robot
  1. Once booted and network connected, start Skype
  1. After Skype fully loads and logs in, start the Sparky Skype Listener Plugin
  1. Press the "Connect" button on the Skype Listener Plugin, you should see a success message if the plugin could connect to Skype.

That should do it**! When someone calls the robot, it'll automatically answer!**

  * WHACKY BUG NOTE -- always click (focus) the text chat window in Skype as the last action after making contact with a human driver. You may need to open a text chat window from the "Windows" menu.

## On the controller ##
  1. Start Skype and log in
  1. Start the "Sparky Controller Skype Plugin" and click connect, you should see a success message if the plugin could connect to Skype.
  1. Call your ready robot and start driving!