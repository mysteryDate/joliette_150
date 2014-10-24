#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // For portability
//    ofSetDataPathRoot("../Resources/data/");
    ofSetFrameRate(60);
    
    font.loadFont("shimmerbold_opentype.ttf", 30, true, true);
    
    textMessageInput.Create();
    textMessageInput.Bind(UDP_PORT);
    textMessageInput.SetNonBlocking(true);
    text_speed = 2;
    message_spacing = 30;
    
    toPython.Create();
    toPython.Connect("127.0.0.1", UDP_PORT+1); // Assuming local machine for now
    toPython.SetNonBlocking(true);
    
    controlMessageInput.Create();
    controlMessageInput.Bind(UDP_PORT+2);
    controlMessageInput.SetNonBlocking(true);
    
    syphon_out.setName("text output");

    controls_on = true;
    running = true;
    
    pythonThread.start();
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // Receive messages if they're waiting
    int message_size = 100000;
    char textMessage[message_size];
    textMessageInput.Receive(textMessage, message_size);
    if (textMessage[0] != 0) {
        Message newMessage;
        newMessage.text = textMessage;
        string beginning = newMessage.text.substr(0,6);
        if (beginning == "NEWEST") { // Terrible hack to get most recent message
            newMessage.newest = true;
            newMessage.text = newMessage.text.substr(6);
            // Hackier and hackier, cried Alice
            for (int i = 0; i < messages.size(); i++) {
                messages[i].newest = false;
            }
        }
        else newMessage.newest = false;
        newMessage.width = font.stringWidth(newMessage.text);
        newMessage.position = WINDOW_WIDTH + newMessage.width;
        if (messages.size() > 0 ) {
            newMessage.position = messages.back().position + newMessage.width + message_spacing;
        }
        messages.push_back(newMessage);
    }
    
    if (messages.size() > 0) {
        if (running) {
            for (int i = 0; i < messages.size(); i++) {
                messages[i].position -= text_speed;
            }
        }
        // Erase messages off the screen
        Message lastMessage = messages.back();
        // Ask for a new message if the final one has cleared the end of the screen
        if (lastMessage.position < WINDOW_WIDTH) {
            string message_to_python = "new message";
            toPython.Send(message_to_python.c_str(), message_to_python.length());
        }
        if (messages[0].position < 0) {
            messages.erase(messages.begin());
        }
    }
    // Ask for a new message if we have none
    else {
        string message_to_python = "new message";
        int success = toPython.Send(message_to_python.c_str(), message_to_python.length());
    }
    
    
}

// This is just for changing font size, so things don't overlap
void ofApp::recalculateMessagePositions() {
    // First, get all the widths
    for (int i = 0; i < messages.size(); i++) {
        messages[i].width = font.stringWidth(messages[i].text);
    }
    // Don't reposition the first message
    for (int i = 1; i < messages.size(); i++) {
        messages[i].position = messages[i-1].position + messages[i].width + message_spacing;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

    // Clear with alpha, so we can capture via syphon and composite elsewhere should we want.
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (int i = 0; i < messages.size(); i++) {
        if (messages[i].newest == true) {
            ofSetColor(255, 0, 0);
        }
        else {
            ofSetColor(255, 255, 255);
        }
        font.drawString(messages[i].text, messages[i].position - messages[i].width, font.getSize());
    }

    syphon_out.publishScreen();

    if (controls_on) {
        stringstream reportStream;
        reportStream
        << "Framerate: " << ofToString((ofGetFrameRate())) << endl
        << "Controls: " << endl
        << "    space:  start/stop" << endl
        << "    M, m:   Increase/decrease message spacing (" << message_spacing << ")" << endl
        << "    T, t:   Increase/decrease scroll speed (" << text_speed << ")" << endl
        << "    +, -:   Increase/decrease text size" << endl
        << "    c:      turn off this display (this text is not syphoned)." << endl;
        ofPushStyle();
        ofSetColor(0, 255, 0);
        ofDrawBitmapString(reportStream.str(), 10, font.getSize() + 30);
        ofPopStyle();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    string message;
    if (controls_on) {
        switch (key) {
                
            case 's':
                message = "save";
                break;
                
            case ' ':
                running = !running;
                break;
                
            case 'l':
                message = "load";
                break;
                
            case OF_KEY_BACKSPACE:
                message = "shutdown";
                break;
            
            case 'M':
                message_spacing++;
                recalculateMessagePositions();
                break;
                
            case 'm':
                message_spacing--;
                recalculateMessagePositions();
                break;
                
            case 'T':
                text_speed+= 0.1;
                break;
                
            case 't':
                text_speed-= 0.1;
                break;
                
            case '=':
            case '+':
                font.loadFont("shimmerbold_opentype.ttf", font.getSize()+1, true, true);
                recalculateMessagePositions();
                break;
                
            case '-':
            case '_':
                int size = font.getSize();
                font.loadFont("shimmerbold_opentype.ttf", font.getSize()-1, true, true);
                recalculateMessagePositions();
                break;
        }
    }
    
    text_speed = ofClamp(text_speed, 0, 70);
    
    if (message.length() > 0) {
        toPython.Send(message.c_str(), message.length());
    }
    if (key == 'c') {
        controls_on = !controls_on;
    }
}

//--------------------------------------------------------------
void ofApp::exit(){
    string message = "shutdown";
    toPython.Send(message.c_str(), message.length());
}
