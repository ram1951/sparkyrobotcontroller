//
//  SkypeController.mm
//  SkypeAPITest
//
//  Created by Janno Teelem on 14/04/2005.
//  Copyright 2005-2006 Skype Limited. All rights reserved.
//

#import "SkypeController.h"
#import "KeyReaderView.h"
#import "Sparky.h"


#import "STJoystick.h"

#import "unistd.h"

NSString* const cMyApplicationName = @"Drive Sparky";
extern SkypeController *MySkypeController;

@implementation SkypeController

//extern SkypeController *MySkypeController;

///INIT///
- (void)init
{
		MySparky = [Sparky new];
	//[MySparky Debug:@"Hello"];
//	MySkypeController = self;
	//[MyKeyReader setMySkypeController:self];
	
	//[MyKeyReader setMySkypeController:self];
	//[self initializeSparkyJoystick:@"initialize from init"];
	mJoystick = NULL;
	
}

//////////////////////////////////////////////////////////////////////////////
// onControlTimer
//  timer for reading joystick and responding by sending robot control commands
//===============
-(void) onControlTimer {
	//printf("CONTROL TIMER\n");
	//if (infoView)
	//	[infoView insertText:@"CONTROL TIMERe\n"];
	if (!mJoystick)
		[self initializeSparkyJoystick:@"FROM control timer\n"];
		
	if (mJoystick)
	{
		static char *lastCommand = NULL;
		static time_t time_command = time(NULL);
		static char command[1024];
		int timeElapsed = time(NULL) - time_command;
		
		if (!lastCommand)
		{
			lastCommand = (char *)calloc(1, 1024);
			strcpy(lastCommand,"foo");
			strcpy(command,"stop");
		}
		
		mJoystick->Update();
		float leftPercent = -(mJoystick->GetAxis(mLeftJoystickAxis) - 128.0) / 128.0;
		float rightPercent = -(mJoystick->GetAxis(mRightJoystickAxis) - 128.0) / 128.0;
		
		//left
		if ((leftPercent - rightPercent) < -0.5)
		{
			strcpy(command,"move_left");
		}
		//right
		else if ((leftPercent - rightPercent) > 0.5) 
		{
			strcpy(command,"move_right");
		}
		//straight
		else if (leftPercent > 0.25 && rightPercent > 0.25)
		{
			strcpy(command,"move_straight");
		}
		//reverse
		else if (leftPercent < -0.25  && rightPercent < -0.25)
		{	
			strcpy(command,"move_reverse");
		}
		else if (timeElapsed > 1)
		{
			strcpy(command,"stop");
		}
		else
		{
			char ckey[1024], cvalue[1024];
			//parse the speed off the last command if it has a speed
			if (sscanf(lastCommand,"%[^_]_%[^_]", ckey, cvalue)==2)
			{
				sprintf(command,"%s_%s",ckey, cvalue);
			}
			
		}
		
		//if the command is not stop, append the left/right axis percentage
		if (strcmp(command,"stop"))
		{
			char new_command[1024];
			int leftPercentInt = leftPercent*100.0;
			int rightPercentInt = rightPercent*100.0;
			
			leftPercentInt -= leftPercentInt % 10; 
			rightPercentInt -= rightPercentInt % 10;
			
			//to be friendlier on chattiness, quantize the percentages
			sprintf(new_command,"%s_L_%.2f_R_%.2f",   command, leftPercentInt/100.0, rightPercentInt/100.0);
			strcpy(command, new_command);
		}
		
		
		if (strcmp(lastCommand, command) || (strcmp(command, "stop") && timeElapsed > 1))
		{
				NSString *skype_command = [NSString stringWithFormat:@"CHATMESSAGE %@ :%s",[MySparky getChatID],command];
				NSLog(skype_command);
				[infoView insertText:[NSString stringWithFormat:@"%@\n", skype_command]];
				
				[SkypeAPI sendSkypeCommand:skype_command];
				time_command = time(NULL);
				strcpy(lastCommand, command);
		}
		
	}
}


/////////////////////////////////////////////////////////////////////////////////////
- (void)awakeFromNib
{
	[SkypeAPI setSkypeDelegate:self];
	[MyKeyReader setMySkypeController:(SkypeController *)self];
	//MySkypeController = self;
	
	//JOYSTICK SUPPORT
	//INITIALIZE JOYSTICK
	//[self initializeSparkyJoystick:@"initialize from awake"];
	/////////////////////////////////////////
	
	[NSTimer scheduledTimerWithTimeInterval:0.10 target:self selector:@selector(onControlTimer) userInfo:nil repeats:YES];
}

//// initialize a joystick using Stanford's STJoystick library
- (void)initializeSparkyJoystick:(NSString *)foo
{
	STJoystick::Initialize();
	printf("There are %d joystick(s) attached to the system\n",STJoystick::NumJoysticks());
	if (STJoystick::NumJoysticks() > 0)
		mJoystick = STJoystick::OpenJoystick(0);
		
	mLeftJoystickAxis = 1;
	mRightJoystickAxis = 3;
	
	//	mJoystick->Update();
	//for (int a=0; a < mJoystick->NumAxes(); a++)
	//{
//		printf("Axis:%d is at:%d\n",a, mJoystick->GetAxis(a));
//	}

}





////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////
// required delegate method
- (NSString*)clientApplicationName
{
	return cMyApplicationName;
}

/////////////////////////////////////////////////////////////////////////////////////
// optional delegate method
- (void)skypeAttachResponse:(unsigned)aAttachResponseCode
{
	switch (aAttachResponseCode)
	{
		case 0:
			[infoView insertText:@"Failed to connect\n"];
			break;
		case 1:
			[infoView insertText:@"Successfully connected to Skype!\n"];
			break;
		default:
			[infoView insertText:@"Unknown response from Skype\n"];
			break;
	}
	
}

/////////////////////////////////////////////////////////////////////////////////////
// optional delegate method
- (void)skypeNotificationReceived:(NSString*)aNotificationString
{
	[infoView insertText:aNotificationString];
	[infoView insertText:@"\n"];
	
	//make sure we are "connected"
	[MyKeyReader setMySkypeController:(SkypeController *)self];
	
	//JJC read the notification string and look for specific notifications
	/*
	
	COMMAND -- ACTION
	
	CALL -- answerCall
	
	*/
	
	NSString *regex = @"CHAT.*";
	NSPredicate *regextest = [NSPredicate
                         predicateWithFormat:@"SELF MATCHES %@", regex];

	if ([regextest evaluateWithObject:aNotificationString] == YES) {
		//[infoView insertText:@"Match\n"];
		[MySparky chatGrabID:aNotificationString];
	}
	
	NSString *regex_msg = @"MESSAGE.*";
	NSPredicate *regextest_msg = [NSPredicate
                         predicateWithFormat:@"SELF MATCHES %@", regex_msg];

	if ([regextest_msg evaluateWithObject:aNotificationString] == YES) {
		[infoView insertText:@"**************HANDLING MESSAGE\n"];
		NSMutableString *new_command = [MySparky handleMessage:aNotificationString];
		//see if we have should send another message to gather the body of the message
		NSString* returnedString = [SkypeAPI sendSkypeCommand:new_command];
		if (returnedString)
		{
			[MySparky handleMessage:returnedString];
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
// optional delegate method
- (void)skypeBecameAvailable:(NSNotification*)aNotification
{
	[infoView insertText:@"Skype became available\n"];
}

/////////////////////////////////////////////////////////////////////////////////////
// optional delegate method
- (void)skypeBecameUnavailable:(NSNotification*)aNotification
{
	[infoView insertText:@"Skype became unavailable\n\n"];
}

/////////////////////////////////////////////////////////////////////////////////////
- (IBAction)onConnectBtn:(id)sender
{
	[SkypeAPI connect];
	[MyKeyReader setMySkypeController:(SkypeController *)self];
	
}

/////////////////////////////////////////////////////////////////////////////////////
- (IBAction)onDisconnectBtn:(id)sender
{
	[SkypeAPI disconnect];
}

/////////////////////////////////////////////////////////////////////////////////////
- (IBAction)onSendBtn:(id)sender
{
	[infoView insertText:[commandField stringValue]];
	[infoView insertText:@"\n"];
	
	NSString* returnedString = [SkypeAPI sendSkypeCommand:[commandField stringValue]];
	if (returnedString)
	{
		[infoView insertText:returnedString];
		[infoView insertText:@"\n"];
	}
}

- (void)Debug:(NSString *)line
{
	NSLog(@"Sparky:");
	NSLog(line);
	NSLog(@"\n");
}

- (IBAction)onMoveLeft:(id)sender
{
	NSString *command = [NSString stringWithFormat:@"CHATMESSAGE %@ :move_left",[MySparky getChatID]];
	NSLog(@"onMoveLeft");
	NSLog(command);
	
	[SkypeAPI sendSkypeCommand:command];
	
}

- (IBAction)onMoveLeftSlow:(id)sender
{
	[self MoveLeft:@"_slow"];
	
	
}

- (IBAction)onMoveRightSlow:(id)sender
{
	[self MoveRight:@"_slow"];
	
	
}

- (IBAction)onMoveStraightSlow:(id)sender
{
	[self MoveStraight:@"_slow"];
	
	
}

- (IBAction)onMoveReverseSlow:(id)sender
{
	[self MoveReverse:@"_slow"];
	
	
}

- (IBAction)onStop:(id)sender
{
	[self Stop:@""];
		
}

- (void)Stop:(NSString *)attr
{
    NSString *command = [NSString stringWithFormat:@"CHATMESSAGE %@ :stop",[MySparky getChatID]];
	NSLog(@"STOP");
	NSLog(command);
	
	[SkypeAPI sendSkypeCommand:command];
}




- (void)MoveLeft:(NSString *)attr
{
	NSString *command = NULL;
	
	if (attr)
	{
		command = [NSString stringWithFormat:@"CHATMESSAGE %@ :move_left%@",[MySparky getChatID],attr];
	}
	else
	{
		command = [NSString stringWithFormat:@"CHATMESSAGE %@ :move_left",[MySparky getChatID]];
	}
	NSLog(@"MoveLeft");
	NSLog(command);
	
	[SkypeAPI sendSkypeCommand:command];
	
}

- (void)MoveRight:(NSString *)attr
{
	NSString *command = NULL;
	
	if (attr)
	{
		command = [NSString stringWithFormat:@"CHATMESSAGE %@ :move_right%@",[MySparky getChatID],attr];
	}
	else
	{
		command = [NSString stringWithFormat:@"CHATMESSAGE %@ :move_right",[MySparky getChatID]];
	}
	NSLog(@"MoveRight");
	NSLog(command);
	
	[SkypeAPI sendSkypeCommand:command];
	
}

- (void)MoveStraight:(NSString *)attr
{
	NSString *command = NULL;
	
	if (attr)
	{
		command = [NSString stringWithFormat:@"CHATMESSAGE %@ :move_straight%@",[MySparky getChatID],attr];
	}
	else
	{
		command = [NSString stringWithFormat:@"CHATMESSAGE %@ :move_straight",[MySparky getChatID]];
	}
	NSLog(@"MoveStraight");
	NSLog(command);
	
	[SkypeAPI sendSkypeCommand:command];
	
}

- (void)MoveReverse:(NSString *)attr
{
	NSString *command = NULL;
	
	if (attr)
	{
		command = [NSString stringWithFormat:@"CHATMESSAGE %@ :move_reverse%@",[MySparky getChatID],attr];
	}
	else
	{
		command = [NSString stringWithFormat:@"CHATMESSAGE %@ :move_reverse",[MySparky getChatID]];
	}
	NSLog(@"MoveReverse");
	NSLog(command);
	
	[SkypeAPI sendSkypeCommand:command];
	
}

- (IBAction)onMoveRight:(id)sender
{
	NSString *command = [NSString stringWithFormat:@"CHATMESSAGE %@ :move_right",[MySparky getChatID]];
	//NSString *command = [NSString stringWithString:@"booboo"];
	NSLog(@"onMoveRight");
	NSLog(command);
		
	[SkypeAPI sendSkypeCommand:command];
}

- (IBAction)onMoveStraight:(id)sender
{
	NSString *command = [NSString stringWithFormat:@"CHATMESSAGE %@ :move_straight",[MySparky getChatID]];
	NSLog(@"onMoveStraight");
	NSLog(command);
	
	[SkypeAPI sendSkypeCommand:command];
}

- (IBAction)onMoveReverse:(id)sender
{
	NSString *command = [NSString stringWithFormat:@"CHATMESSAGE %@ :move_reverse",[MySparky getChatID]];
	NSLog(@"onMoveReverse");
	NSLog(command);
	
	[SkypeAPI sendSkypeCommand:command];
}

- (void)MoveStraight
{
	[self onMoveStraight:NULL];
}

//- (void)keyDown:(NSEvent *)theEvent
//{
 ///   NSString *keyChar = [theEvent characters];
    //if ( [keyChar isEqualToString:@”c”] ) {
      //  [path removeAllPoints];
        //[self setNeedsDisplay:YES];
    //}
	//NSLog(keyChar);
//}

//- (BOOL)acceptsFirstResponder
//{
 //   return YES;
//}

/////////////////////////////////////////////////////////////////////////////////////
@end
