#pragma once

#include "ofMain.h"
#include "ofxNetwork.h"
#include "ofxSyphon.h"
#include "ofxTrueTypeFontUC.h"
#include "pythonThread.h"


#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 300

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);

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
    bool message_trigger;

    //Syphon
    ofxSyphonServer syphon_out;
    
    bool controls_on;
    bool running;
  
    PythonThread pythonThread;

};
