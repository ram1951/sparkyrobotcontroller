//
//  SkypeController.mm
//  SkypeAPITest
//
//  Created by Janno Teelem on 14/04/2005.
//  Copyright 2005-2006 Skype Limited. All rights reserved.
//

#import "SkypeController.h"
//#import "Sparky.h"

NSString* const cMyApplicationName = @"My Skype API Tester";

@implementation SkypeController

///INIT///
- (id)init
{
	//int foo;
//	char *bar=NULL;
	
//	sscanf(bar,"%d",&foo);
	//MySparky = [Sparky new];
	//[MySparky init];
	//[MySparky Debug:@"Hello"];
	
	//state_get_handle=0;
	my_partner_handle[0] = 0;
}

/////////////////////////////////////////////////////////////////////////////////////
- (void)awakeFromNib
{
	[SkypeAPI setSkypeDelegate:self];
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

	if (!call_id)
	{
		chat_id = [NSMutableString new];
		call_id = [NSMutableString new];
		partner_handle = [NSMutableString new];
		my_partner_handle[0] = 0;
	}
	
	//JJC read the notification string and look for specific notifications
	/*
	
	COMMAND -- ACTION
	
	CALL -- answerCall
	
	*/
	
	
//	NSString *regex = @"CHAT.*";
//	NSPredicate *regextest = [NSPredicate
 //                        predicateWithFormat:@"SELF MATCHES %@", regex];

//	if ([regextest evaluateWithObject:aNotificationString] == YES) {
		//[infoView insertText:@"Match\n"];
//		NSString *chat_id = [MySparky getChatID];
//		if ([chat_id length] > 0)
//		{
//			NSString *create_chat = [NSString stringWithFormat:@"OPEN CHAT %@", chat_id];
//			[SkypeAPI sendSkypeCommand:create_chat];
//			NSString *create_chat_hello = [NSString stringWithFormat:@"CHATMESSAGE %@ Hello, this is Sparky", chat_id];
//			[SkypeAPI sendSkypeCommand:create_chat_hello];
//		}
//	}
	
	
	
	NSString *regex_msg = @"MESSAGE.*";
	NSPredicate *regextest_msg = [NSPredicate
                         predicateWithFormat:@"SELF MATCHES %@", regex_msg];

	if ([regextest_msg evaluateWithObject:aNotificationString] == YES) {
		[infoView insertText:@"**************HANDLING MESSAGE\n"];
		NSMutableString *new_command = [self handleMessage:aNotificationString];
		//see if we have should send another message to gather the body of the message
		NSString* returnedString = [SkypeAPI sendSkypeCommand:new_command];
		if (returnedString)
		{
			[self handleMessage:returnedString];
		}
	}
	
	char aNotificationString_c[1024];
	
	strcpy(aNotificationString_c,[aNotificationString cString]);
	
	//Handle VIDEO, start video once see "INPROGRESS"
	if (strstr(aNotificationString_c,"INPROGRESS")) {
		[infoView insertText:@"**************HANDLING VIDEO"];
		NSString *start_the_video = [NSString stringWithFormat:@"ALTER CALL %@ START_VIDEO_SEND", [self getCallID]];
		[SkypeAPI sendSkypeCommand:start_the_video];
		
		//create a chat if there is no chat_id yet//////////
		if ([[self getChatID] length] <=0)
		{
			NSString *create_chat = [NSString stringWithFormat:@"CHAT CREATE %s", my_partner_handle];
			NSString *create_response = [SkypeAPI sendSkypeCommand:create_chat]; 	
			[self chatGrabID:create_response];		
			
			//NSString *maxit = [NSString stringWithFormat:@"OPEN IM %s openned...",my_partner_handle];
		//	NSString *maxit = [NSString stringWithFormat:@"OPEN LIVETAB"];
			
			
		//	NSString *response = [SkypeAPI sendSkypeCommand:maxit];
			NSString *create_chat_hello = [NSString stringWithFormat:@"CHATMESSAGE %@ Hello, this is Sparky", [self getChatID]];
			[SkypeAPI sendSkypeCommand:create_chat_hello];
			int i=0;
		}
		
		sleep(2);
		//state_get_handle=1;
		//[SkypeAPI sendSkypeCommand:@"ALTER CALL 229 START_VIDEO_SEND"];
	}
	
	//Handle ANSWER A CALL, START VIDEO, ASK FOR PARTNER_HANDLE
	
	if (strstr(aNotificationString_c,"RINGING")) {
		[infoView insertText:@"**************HANDLING ANSWER\n"];
		NSMutableString *my_call_id = [self handleAnswer:aNotificationString];
		
		NSString *ask_partner_handle = [NSString stringWithFormat:@"GET CALL %@ PARTNER_HANDLE", my_call_id];
		[SkypeAPI sendSkypeCommand:ask_partner_handle]; 
		
		NSString *answer_the_phone = [NSString stringWithFormat:@"ALTER CALL %@ ANSWER", my_call_id];
		[SkypeAPI sendSkypeCommand:answer_the_phone]; 
		
		NSString *get_partner_handle = [NSString stringWithFormat:@"GET CALL %@ PARTNER_HANDLE", [self getCallID]];
		NSString *handle_notification = [SkypeAPI sendSkypeCommand:get_partner_handle];
		
		char read_partner_handle[1024];
		if (sscanf([handle_notification cString],"CALL %*s PARTNER_HANDLE %s",read_partner_handle)==1)
			{
				strcpy(my_partner_handle,read_partner_handle);
				[self setPartnerHandle:[NSString stringWithCString:read_partner_handle]];
			}

		
		//sleep(5);
		
		//MOVED
//		NSString *start_the_video = [NSString stringWithFormat:@"ALTER CALL %@ START_VIDEO_SEND", call_id];
//		[SkypeAPI sendSkypeCommand:start_the_video]; 		
		//sleep(2);
		
		//MOVED
		//Get the USERNAME of the caller
		//NSString *get_partner_handle = [NSString stringWithFormat:@"GET CALL %@ PARTNER_HANDLE", call_id];
		//[SkypeAPI sendSkypeCommand:get_partner_handle]; 
	}
	
	//If we have a call_id but no partner_handle, keep sending the command to get it
	
	//int call_id_len = [[self getCallID] length];
	//int partner_handle_len = [[self getPartnerHandle] length];
	//if (call_id_len > 0 && partner_handle_len <= 0 && state_get_handle==1)
	//{
	//	NSString *get_partner_handle = [NSString stringWithFormat:@"GET CALL %@ PARTNER_HANDLE", [self getCallID]];
	//	[SkypeAPI sendSkypeCommand:get_partner_handle];
	//	state_get_handle = 0;
	//}
	
	
	//Handle CALL notifications
	if (strstr(aNotificationString_c, "CALL"))
	{
//		if (strstr(aNotificationString_c,"PARTNER_HANDLE"))
//		{
//			char partner_handle[1024];
//			
//			if (sscanf(aNotificationString_c,"CALL %*s PARTNER_HANDLE %s",partner_handle)==1)
//			{
//				strcpy(my_partner_handle,partner_handle);
//				[self setPartnerHandle:[NSString stringWithCString:my_partner_handle]];
//				
//				//create a chat if there is no chat_id yet//////////
//				if ([[self getChatID] length] <=0)
//				{
//					NSString *create_chat = [NSString stringWithFormat:@"CHAT CREATE %@", my_partner_handle];
//					[SkypeAPI sendSkypeCommand:create_chat]; 			
//				}
//			}
//		}
	}
		
		//Create a chat
		//------------------------------------------------------------------
// Creating chat with one target
//-> CHAT CREATE anappo5
//<- CHAT #anappo/$anappo5;2e4e763a2fc121ed STATUS DIALOG
//-> OPEN CHAT #anappo/$anappo5;2e4e763a2fc121ed
//<- OPEN CHAT #anappo/$anappo5;2e4e763a2fc121ed
		
	
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


/////////////////////////////////////////////////////////////////////////////////////

- (void)chatGrabID:(NSString*)APILine
{
	char *APILine_c = (char *)[APILine cString];
	char chat_id_c[1024]; 
	NSString *debug_str = [NSString new];
	sscanf(APILine_c,"%*s %s", chat_id_c);

	[chat_id setString:[NSString stringWithCString:chat_id_c]];
	
	debug_str = [debug_str stringByAppendingString:@"chat_id:"];
	debug_str = [debug_str stringByAppendingString:chat_id];
	[self Debug:debug_str];
	
	//[debug_str free];
}

- (NSMutableString*)getChatID
{
	return chat_id;
}

- (NSMutableString*)getCallID
{
	return call_id;
}

- (NSMutableString*)getPartnerHandle
{
	return partner_handle;
}


-(void)setPartnerHandle:(NSString *)in_partner_handle
{
	[partner_handle setString:in_partner_handle];
}

- (NSMutableString *)handleAnswer:(NSString*)APILine
{
	char *APILine_c = (char *)[APILine cString];
	char call_id_c[1024];
	
	int numread = 0;
	
	numread = sscanf(APILine_c,"CALL %s",call_id_c);
	
	if (numread)
	{
		[call_id setString:[NSString stringWithCString:call_id_c]];
		return call_id;
	}	
	return NULL;
}

- (NSMutableString *)handleMessage:(NSString*)APILine
{
	char *APILine_c = (char *)[APILine cString];
	int message_number;
	
	//if we received a message
	if (strstr(APILine_c,"READ") && (sscanf(APILine_c,"MESSAGE %d STATUS READ",&message_number)==1))
	{
		char body_command[1024];
		NSMutableString *tstr = [NSMutableString new];
		//send the API command GET CHATMESSAGE %d BODY
		sprintf(body_command,"GET CHATMESSAGE %d BODY",message_number);
		[tstr setString:[NSString stringWithCString:body_command]];
		
		//[SkypeAPI sendSkypeCommand:tstr];
		return tstr;
		
	}
	else if (strstr(APILine_c,"BODY"))
	{
		char message[1024];
		
		if ((sscanf(APILine_c,"MESSAGE %*d BODY :%[^\n]",message))==1)
		{
			NSMutableString *tstr = [NSMutableString new];
			[tstr setString:[NSString stringWithCString:message]];
			[self file_sendMoveCommand:tstr];
			//[tstr delete];
		}
	}
	return NULL;
}

-(NSMutableString *)file_sendMoveCommand:(NSString*)action
{
	FILE *out_command = fopen("/tmp/SparkyController.lock","w");
	if (out_command)
	{
		fprintf(out_command,"%s",[action cString]);
		NSLog(@"WRITNG\n");
		NSLog(action);
		NSLog(@"\n");
		fclose(out_command);
	}
	else
	{
	  NSLog(@"Can not open /tmp/SparkyController.lock for writing\n");
	}
	return;
}

-(NSMutableString *)http_sendMoveCommand:(NSString*)action
{
	NSMutableURLRequest * request = [[NSMutableURLRequest alloc] init];

    // config
    [request setHTTPMethod:@"POST"];
    [request setURL:[NSURL URLWithString:@"http://localhost/~Sparky/control.php"]];
   // [request setURL:[NSURL URLWithString:@"http://dev.wunderground.com/john/test.php"]];
   
   
    // set the headers
    [request addValue:@"localhost" forHTTPHeaderField:@"Host"];
    [request addValue:@"application/x-www-form-urlencoded" forHTTPHeaderField:@"Content-Type"];
    [request addValue:@"en" forHTTPHeaderField:@"Accept-Language"];
    //[request addValue:@@"gzip, deflate" forHTTPHeaderField:@@"Accept-Encoding"];
    //[request addValue:@@"http://www.wunderground.com/" forHTTPHeaderField:@@"Referer"];
    //[request addValue:@@"Mozilla/5.0 (Macintosh; U; PPC Mac OS X; en) AppleWebKit/522.11 (KHTML, like Gecko) Version/3.0.2 Safari/522.12" forHTTPHeaderField:@@"User-Agent"];
    //[request addValue:@@"keep-alive" forHTTPHeaderField:@@"Connection"];
    //[request addValue:@@"text/xml,application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5" forHTTPHeaderField:@@"Accept"];

    // set the body and Content-Length header
    //NSString * body = @@"";
    NSString * body = [NSString stringWithFormat: @"action=%@", action];
    //NSLog(@@"request body: %@@", body);
    [request setHTTPBody:[body dataUsingEncoding:NSUTF8StringEncoding]];
    [request addValue:[NSString stringWithFormat:@"%d", [body length]] forHTTPHeaderField:@"Content-Length"];

    // make the actual request
    NSURLResponse *resp = nil;
    NSError *err = nil;
    NSData * data = [NSURLConnection sendSynchronousRequest:request returningResponse:&resp error:&err];
}

- (void)Debug:(NSString *)line
{
	NSLog(@"Sparky:");
	NSLog(line);
	NSLog(@"\n");
}



@end
