//
//  KeyReaderView.h
//  SkypeAPITest
//
//  Created by John Celenza on 10/17/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "SkypeController.h"

@interface KeyReaderView : NSView {

	SkypeController *MySkypeController;

}

- (void)keyDown:(NSEvent *)theEvent;
- (void)setMySkypeController:(SkypeController *)in_skype;

@end
