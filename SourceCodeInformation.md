#discussions on the source code

### Many thanks to Jesse DeGuire, Nathan Sprague for the library COIL that allows simple programmatic control of the iRobot ###

## Getting the source code ##
The source code is the SVN tree linked from:
<a href='http://code.google.com/p/sparkyrobotcontroller/source/checkout'><a href='http://code.google.com/p/sparkyrobotcontroller/source/checkout'>http://code.google.com/p/sparkyrobotcontroller/source/checkout</a></a>

## Getting familiar with the code ##

  * There are several programs for making Sparky work. They use the Skype API and the serial interface to the iRobot.

The project is partly written in:
  1. C for the program that talks directly from the robot's MacMini over serial cable to the iRobot (controld.c)
  1. Objective-C (Xcode) for the program that talks to the Skype API on the robot's MacMini and conveys messages to controld
  1. Objective-C (Xcode) for the program that talks to the Skype API on the controller's computer that convey's joystick commands to Skype (to then send to the MacMini on the robot)

Once you download the source:

  * goto the sparkyrobotcontroller/irobot directory for the C source code for item 1 above (controld)
  * goto sparkyrobotcontroller/SkypeSparkyServer for the Objective-C code for item 2 above (Sparky Server). This is an Xcode project
  * goto sparkyrobotcontroller/SparkyControllerSkypePlugin for the Objective-C for item 3 above (Sparky Joystick). This is an Xcode project


## more about controld ##
Once you have downloaded the code, go to:
/sparkyrobotcontroller/irobot/sparky

This is where controld.c lives.

### Supporting more movements / beeps /etc ###
You will want to edit the function move\_dir(char **action, HDF**config)

Here is where your code reading abilities will be put to the test since I have been sloppy.

When a text message arrives on the robot, the contents of the command are the "action" argument passed to move\_dir.

move\_dir originally (in older versions with the MakeBoard) would match the "action" string with the movement commands in config.hdf, then set the left and right servos to the values of:
```
movement..0
movement..1 respectively.
```

However, with the iRobot and the joystick, we wanted fine tune control of the motors. So now, SparkyJoystick (which you are also free to modify) sends movement commands like this:
```
move_DIRECTION_L_VALUE_R_VALUE
```
for example:
```
move_straight_L_50_R_75
```
means crank the left motor to 50% and the right motor to 75% and set the turning radius to 0.

At the end of move\_dir, the call
```
drive(velocity, radius);
```
actually tells the robot to move.

Now the question is how to add commands. Maybe add support for radius to be sent like:
```
move_straight_L_50_R_75_RA_1
```

or add some commands that beep or play music :-)

Allowing customization via control.hdf is tricky since support needs to exist in the controld and SparkyJoystick software anyhow.


## SparkyJoystick ##
SparkyJoystick is an XCode project and is also located in the SVN tree.