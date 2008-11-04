//
//  KeyReaderView.m
//  SkypeAPITest
//
//  Created by John Celenza on 10/17/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import "KeyReaderView.h"
#import "SkypeController.h"

//extern SkypeController *MySkypeController;

@implementation KeyReaderView

- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
		
		//MySkypeController = [SkypeController new];
    }
    return self;
}

//- (void)destroy {
//  /* Release the pool */
//  RELEASE(pool);
//}

- (void)drawRect:(NSRect)rect {
    // Drawing code here.
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}

- (void)awakeFromNib
{
	//[SkypeAPI setSkypeDelegate:self];
	
}

- (void)keyDown:(NSEvent *)theEvent
{
    NSString *keyChar = [theEvent charactersIgnoringModifiers];
	const char *keyString = [keyChar cString];
	
	
	
	
//    if ( [keyChar isEqualToString:@"c"] ) {
//	[path removeAllPoints];
//	[self setNeedsDisplay:YES];
//    }
	fprintf(stderr,"pressed %s MySkypeController:%x\n",[keyChar cString],MySkypeController);
	
	if (MySkypeController)
	{
		if (!strcmp(keyString,"w"))
		{
			SkypeController *test = MySkypeController;
		
			[MySkypeController MoveStraight:NULL];
		
			fprintf(stderr,"test:%x\n",test);
		}
		else if (!strcmp(keyString,"a"))
		{
			SkypeController *test = MySkypeController;
		
			[MySkypeController MoveLeft:NULL];
		
			fprintf(stderr,"test:%x\n",test);
		}
		else if (!strcmp(keyString,"s"))
		{
			SkypeController *test = MySkypeController;
		
			[MySkypeController MoveReverse:NULL];
		
			fprintf(stderr,"test:%x\n",test);
		}
		else if (!strcmp(keyString,"d"))
		{
			SkypeController *test = MySkypeController;
		
			[MySkypeController MoveRight:NULL];
		
			fprintf(stderr,"test:%x\n",test);
		}
		
		if (!strcmp(keyString,"i"))
		{
			SkypeController *test = MySkypeController;
		
			[MySkypeController MoveStraight:@"_slow"];
		
			fprintf(stderr,"test:%x\n",test);
		}
		else if (!strcmp(keyString,"j"))
		{
			SkypeController *test = MySkypeController;
		
			[MySkypeController MoveLeft:@"_slow"];
		
			fprintf(stderr,"test:%x\n",test);
		}
		else if (!strcmp(keyString,"k"))
		{
			SkypeController *test = MySkypeController;
		
			[MySkypeController MoveReverse:@"_slow"];
		
			fprintf(stderr,"test:%x\n",test);
		}
		else if (!strcmp(keyString,"l"))
		{
			SkypeController *test = MySkypeController;
		
			[MySkypeController MoveRight:@"_slow"];
		
			fprintf(stderr,"test:%x\n",test);
		}
		else if (!strcmp(keyString," "))
		{
			SkypeController *test = MySkypeController;
		
			[MySkypeController Stop:@""];
		
			fprintf(stderr,"test:%x\n",test);
		}


	}
}

- (void)setMySkypeController:(SkypeController *)in_skype
{
	MySkypeController = in_skype;
}


@end
