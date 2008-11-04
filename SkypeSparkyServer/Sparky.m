//
//  Sparky.m
//  SkypeAPITest
//
//  Created by John Celenza on 9/23/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import <Skype/Skype.h>
#import "Sparky.h"
#import "SkypeController.h"


@implementation Sparky

-(void) init
{
	chat_id = [NSMutableString new];
	call_id = [NSMutableString new];
	partner_handle = [NSMutableString new];
	
	// these somehow break the object
	//[chat_id init];
	//[call_id init];
	//[partner_handle init];
	
	//[chat_id setString:@""];
	//[call_id setString:@""];
	//[partner_handle setString:@""];
	//////////////////
}


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
			[self http_sendMoveCommand:tstr];
			//[tstr delete];
		}
	}
	return NULL;
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
