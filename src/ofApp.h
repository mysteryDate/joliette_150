#pragma once

#include "ofMain.h"
#include "ofxNetwork.h"
#include "ofxSyphon.h"
#include "ofxSecondWindow.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 300

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void mousePressed(int x, int y, int button);
    
    ofxSecondWindow controlWindow;
    
    //    ofImage current_picture;
    //    ofImage next_picture;
    //    int current_picture_index;
    //    ofDirectory picture_directory;
    
    ofTrueTypeFont font;
    
    //  A message to be displayed
    struct Message {
        string text;
        int position;
    };
    vector<Message> messages;
    float text_speed;
    
    ofxUDPManager textMessageInput;
    ofxUDPManager toPython;
    bool message_trigger;

    //Syphon
    ofxSyphonServer syphon_out;
    
    bool controls_on;
};
