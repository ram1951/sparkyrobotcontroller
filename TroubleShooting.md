## For Trouble Shooting communication between a human controller and the robot  MAKE CONTROLLER VERSION ##
A list of items to check. This list starts from the human controller size and works through to the MakeController.


  * when you click a command button like "straight", do you see a text message get sent?
> Something like:
> `:move_straight`
> should arrive on the Skype client running on the robot. If that works, you are sending commands successfully.

  * if you see that text message, do you see an LED on the MakeController light up? It should light up just after sending the command. If you see this light, the MakeController is receiving commands from the Skype Plugin on the robot, which means the PHP script is running too.

  * If you do no see the LED light:
  * make sure the robot Skype plugin is connected and receiving messages. I think it will print something in its log window when a command comes in. There is a strange bug sometimes where you have to focus the Skype chat window on the robot by clicking it with the mouse
  * make sure the PHP script is running by bringing up a terminal, type:

> `ps ax | grep controld.php`

> if you get nothing back, then the script that listens for commands is not running. You can run it manually by trying:

> `php /Sparky/controld.php"`

> that should start the listener that listens to commands written to /tmp/SparkyController.lock and conveys them to the MakeController on 192.168.0.200

> also see if commands are being sent:

> `cat /tmp/SparkyController.lock`