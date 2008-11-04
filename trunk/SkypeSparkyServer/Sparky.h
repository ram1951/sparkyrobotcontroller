//
//  Sparky.h
//  SkypeAPITest
//
//  Created by John Celenza on 9/23/07.
//  Copyright 2007 CornblattCelenzaWare. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface Sparky : NSObject {
	
	NSMutableString *chat_id;
	NSMutableString *call_id;
	NSMutableString *partner_handle;
}

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
