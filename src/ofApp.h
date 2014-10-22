#pragma once

#include "ofMain.h"
#include "ofxNetwork.h"
#include "ofxSyphon.h"
#include "ofxTrueTypeFontUC.h"
#include "pythonThread.h"

#define UDP_PORT 7011

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 300

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void recalculateMessagePositions();

    ofxTrueTypeFontUC font;
    
    //  A message to be displayed
    struct Message {
        string text;
        // Right hand boundary of the message
        float position;
        int width;
    };
    vector<Message> messages;
    float text_speed;
    int     message_spacing;
    
    ofxUDPManager textMessageInput;
    ofxUDPManager toPython;
    ofxUDPManager controlMessageInput;

    //Syphon
    ofxSyphonServer syphon_out;
    
    bool controls_on;
    bool running;
  
    PythonThread pythonThread;

};
