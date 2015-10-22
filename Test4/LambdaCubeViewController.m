//
//  LambdaCubeViewController.m
//  Test4
//
//  Created by Csaba Hruska on 02/10/15.
//  Copyright © 2015 Csaba Hruska. All rights reserved.
//

#import "LambdaCubeViewController.h"
#import "LCDriver.h"

@interface LambdaCubeViewController () {
    float _curRed;
    BOOL _increasing;
    LCDriver * _lcDriver;
}
@property (strong, nonatomic) EAGLContext *context;

@end

@implementation LambdaCubeViewController
@synthesize context = _context;

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    
    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }

    GLKView *view = (GLKView *)self.view;
    view.context = self.context;

    [view bindDrawable];
    GLint screenTarget;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING,&screenTarget);
    _lcDriver = [LCDriver new];
    int w = self.view.frame.size.width;
    int h = self.view.frame.size.height;
    [_lcDriver sayHello:screenTarget width:w height:h];
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
    self.context = nil;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

#pragma mark - GLKViewDelegate

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
    [_lcDriver render];
}

#pragma mark - GLKViewControllerDelegate

- (void)update {
    if (_increasing) {
        _curRed += 1.0 * self.timeSinceLastUpdate;
    } else {
        _curRed -= 1.0 * self.timeSinceLastUpdate;
    }
    if (_curRed >= 1.0) {
        _curRed = 1.0;
        _increasing = NO;
    }
    if (_curRed <= 0.0) {
        _curRed = 0.0;
        _increasing = YES;
    }
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    self.paused = !self.paused;
}


@end
