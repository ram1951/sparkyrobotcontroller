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
}

- (void)chatGrabID:(NSString*)APILine;
- (NSMutableString *)handleMessage:(NSString*)APILine;
-(NSMutableString *)http_sendMoveCommand:(NSString*)action;
//- (void)Debug:(NSString*)line;
- (NSMutableString *)getChatID;


@end
