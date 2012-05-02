//
//  ViewController.m
//  GLESlib_iOS
//
//  Created by Lion User on 01/05/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "ViewController.h"
#include "../Game.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define GAME_WIDTH 320
#define GAME_HEIGHT 480

@interface ViewController () {
    Game* gameMain;
}
@property (strong, nonatomic) EAGLContext *context;
- (void)setupGL;
- (void)tearDownGL;

@end

@implementation ViewController

@synthesize context = _context;

- (void)dealloc
{
    [_context release];
    [super dealloc];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    NSLog(@"Init");
    
    self.context = [[[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1] autorelease];

    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    
    [self setupGL];
}

- (void)viewDidUnload
{    
    [super viewDidUnload];
    
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
    self.context = nil;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Release any cached data, images, etc. that aren't in use.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
        return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
    } else {
        return YES;
    }
}

- (void)setupGL
{
    NSLog(@"SETUP");
    gameMain = new Game();
    
    [EAGLContext setCurrentContext:self.context];
    glViewport(0, 0, GAME_WIDTH, GAME_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof(0, GAME_WIDTH, GAME_HEIGHT, 0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    
    gameMain->init();
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
}

- (void)update
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.6, 0.8, 1.0, 1.0);
    gameMain->render();
    gameMain->loop();
}


@end
