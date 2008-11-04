//
//  SkypeController.h
//  SkypeAPITest
//
//  Created by Janno Teelem on 14/04/2005.
//  Copyright 2005-2006 Skype Limited. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <Skype/Skype.h>
#import "Sparky.h"
//#import "KeyReaderView.h"




@interface SkypeController : NSObject <SkypeAPIDelegate>
{
    IBOutlet id commandField;
    IBOutlet id infoView;
	IBOutlet id button_w;
	Sparky *MySparky;
	IBOutlet id MyKeyReader;
	//KeyReaderView MyKeyReader;

	
}

- (void)init;
- (IBAction)onConnectBtn:(id)sender;
- (IBAction)onDisconnectBtn:(id)sender;
- (IBAction)onSendBtn:(id)sender;

//for the move buttons
- (IBAction)onMoveLeft:(id)sender;
- (IBAction)onMoveRight:(id)sender;
- (IBAction)onMoveStraight:(id)sender;
- (IBAction)onMoveReverse:(id)sender;

- (IBAction)onMoveLeftSlow:(id)sender;
- (IBAction)onMoveRightSlow:(id)sender;
- (IBAction)onMoveStraightSlow:(id)sender;
- (IBAction)onMoveReverseSlow:(id)sender;
- (IBAction)onStop:(id)sender;


- (void)MoveStraight:(NSString *)attr;
- (void)MoveRight:(NSString *)attr;
- (void)MoveLeft:(NSString *)attr;
- (void)MoveReverse:(NSString *)attr;
- (void)Stop:(NSString *)attr;



@end
