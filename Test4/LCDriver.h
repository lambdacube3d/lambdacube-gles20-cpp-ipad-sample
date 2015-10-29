//
//  LCDriver.h
//  Test4
//
//  Created by Csaba Hruska on 02/10/15.
//  Copyright © 2015 Csaba Hruska. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface LCDriver : NSObject

- (void)sayHello:(unsigned int)screenTarget width:(unsigned int)w height:(unsigned int) h;
- (void)render:(float) t;
- (void)render2:(float) t;


@end

