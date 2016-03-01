#Troubleshooting Tips

# This Wiki will expand as I discover where folks are having difficulty. Let me know if you are having a problem! You can leave a message at #

## Mac not communicating with iRobot, or you want to use a non-KeySpan USB to Serial adapter ##
You are using a serial-to-USB device that is not the Keyspan model, or just having trouble getting the iRobot to move, even when everything seems to be working in Skype.

### Here is how to proceed: ###
  1. Download a fresh version of the Sparky Server software. I keep finding significant    bugs
> > http://sparkyrobotcontroller.googlecode.com/files/Sparky%20Server.pkg
  1. We need to make a link in /dev from your serial device to /dev/cu.KeySerial1 (that is the device the Sparky program /Sparky/controld uses to send commands from Skype to the iRobot).


> open Terminal
```
 cd /dev
 sudo ln -s /dev/YOURSERIALDEVICE /dev/cu.KeySerial1
```
> To find your serial device, I would:
> turn on iRobot, but unplug serial cable
> open Terminal
```
  cd /dev
  ls -ltr 
```
> now plug in the serial cable
```
  ls -ltr
```
At the bottom of the directory listing, hopefully a new file or files has appeared. For the KeySpan there were two devices, but the cu.KeySerial1 one actually works.

### Testing: ###
After you make the link...
Turn on the iRobot and connect it
```
 cd /Sparky
 sudo killall controld
 sudo ./shell_check_sparky_irobot.php
 ps ax | grep controld
```
after the last command, you should see an instance of controld running
pick up Sparky (off the floor), he/she should start to beep (since the robot is scared)
if you hear no beeps, then try changing the link and trying these testing steps again.

Note, you can also edit /Sparky/shell\_check\_sparky\_irobot.php and change the device passed to controld.