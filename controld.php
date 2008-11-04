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

