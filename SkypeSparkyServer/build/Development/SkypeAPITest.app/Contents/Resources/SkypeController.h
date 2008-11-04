//
//  SkypeController.h
//  SkypeAPITest
//
//  Created by Janno Teelem on 14/04/2005.
//  Copyright 2005-2006 Skype Limited. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <Skype/Skype.h>
//#import "Sparky.h"

@interface SkypeController : NSObject <SkypeAPIDelegate>
{
    IBOutlet id commandField;
    IBOutlet id infoView;
	//Sparky *MySparky;
	char my_partner_handle[1024];

        NSMutableString *chat_id;
        NSMutableString *call_id;
        NSMutableString *partner_handle;

	//int state_get_handle;
}

- (id)init;
- (IBAction)onConnectBtn:(id)sender;
- (IBAction)onDisconnectBtn:(id)sender;
- (IBAction)onSendBtn:(id)sender;


- (void)chatGrabID:(NSString*)APILine;
- (NSMutableString*)getChatID;
- (NSMutableString*)getCallID;
- (NSMutableString*)getPartnerHandle;
- (NSMutableString *)handleMessage:(NSString*)APILine;
-(void)setPartnerHandle:(NSString *)in_partner_handle;
- (NSMutableString *)handleAnswer:(NSString*)APILine;
-(NSMutableString *)http_sendMoveCommand:(NSString*)action;
- (void)Debug:(NSString*)line;


@end
