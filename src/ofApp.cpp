#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // For portability
    ofSetDataPathRoot("../Resources/data/");
    ofSetFrameRate(60);
    
    font.loadFont("shimmerbold_opentype.ttf", 30, true, true);
    
    textMessageInput.Create();
    textMessageInput.Bind(7011);
    textMessageInput.SetNonBlocking(true);
    Message firstMessage;
    firstMessage.text = "Hello";
    firstMessage.position = WINDOW_WIDTH;
    messages.push_back(firstMessage);
    text_speed = 1;
    
    toPython.Create();
    toPython.Connect("127.0.0.1", 7012); // Assuming local machine for now
    toPython.SetNonBlocking(true);
    
    message_trigger = false;
    
    syphon_out.setName("text output");

    controls_on = true;
    running = true;
    
    //    string command = "python "+ofToDataPath("of_controller.py",true);
    //    cout << command;
    //    system(command.c_str());
}

//--------------------------------------------------------------
void ofApp::update(){
    
    int message_size = 100000;
    char textMessage[message_size];
    textMessageInput.Receive(textMessage, message_size);
    
    if (running) {
    for (int i = 0; i < messages.size(); i++) {
        messages[i].position -= text_speed;
        
        if (messages[i].position < 0) {
            messages.erase(messages.begin());
        }
    }
    }
    
    if (messages.size() > 0) {
    Message lastMessage = messages.back();
    if (lastMessage.position < WINDOW_WIDTH) {
        string message_to_python = "new message";
        int success = toPython.Send(message_to_python.c_str(), message_to_python.length());
    }
    }
    if (textMessage[0] != 0) {
        Message newMessage;
        newMessage.text = textMessage;
        newMessage.width = font.stringWidth(textMessage);
        newMessage.position = WINDOW_WIDTH + newMessage.width;
        if (messages.size() > 0 ) {
            newMessage.position = messages.back().position + newMessage.width;
        }
        messages.push_back(newMessage);
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    // Clear with alpha, so we can capture via syphon and composite elsewhere should we want.
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (int i = 0; i < messages.size(); i++) {
        font.drawString(messages[i].text, messages[i].position - messages[i].width, 50);
    }

    syphon_out.publishScreen();

    if (controls_on) {
        stringstream reportStream;
        reportStream
        << "Framerate: " << ofToString((ofGetFrameRate())) << endl
        << "Time: " << ofToString(ofGetElapsedTimeMillis()/1000) << endl
        << "Controls: " << endl
        << "    f:      toggle fullscreen" << endl
        << "    space:  start/stop monitor" << endl
        << "    s:      save message database" << endl
        << "    l:      load message database" << endl
        << "    backspace:  shutdown" << endl;
//        Message newest = messages.back();
//        int width = font.stringWidth(newest.text);
//        reportStream << "Message text: " << newest.text << endl
//        << "Width: " << width << endl
//        << "Position: " << newest.position << endl
//        << "Pix left: " << newest.position + width << endl;
        ofDrawBitmapString(reportStream.str(), 50,150);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    string message;
    if (controls_on) {
        switch (key) {
                
            case 'f':
                ofToggleFullscreen();
                break;
                
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
                
        }
    }
    if (message.length() > 0) {
        toPython.Send(message.c_str(), message.length());
    }
    if (key == 'c') {
        controls_on = !controls_on;
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
    //    current_picture_index++;
    //    if (current_picture_index == picture_directory.numFiles()) {
    //        current_picture_index = 0;
    //    }
    //    int next_picture_index = current_picture_index + 1;
    //    if (next_picture_index == picture_directory.numFiles()) {
    //        next_picture_index = 0;
    //    }
    //    current_picture = next_picture;
    //    next_picture.loadImage(picture_directory.getPath(current_picture_index));
}
