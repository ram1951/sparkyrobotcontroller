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

}

/////////////////////////////////////////////////////////////////////////////////////
- (void)awakeFromNib
{
	[SkypeAPI setSkypeDelegate:self];
	[MyKeyReader setMySkypeController:self];
	//MySkypeController = self;
}

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
	[MyKeyReader setMySkypeController:self];
	
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
	[MyKeyReader setMySkypeController:self];
	
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
