#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // For portability
    ofSetDataPathRoot("../Resources/data/");
    
    controlWindow.setup("Control Window", 50, 50, 1024, 768, false);
    
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

    controls_on = false;
    
    //    string command = "python "+ofToDataPath("of_controller.py",true);
    //    cout << command;
    //    system(command.c_str());
}

//--------------------------------------------------------------
void ofApp::update(){
    
    int message_size = 100000;
    char textMessage[message_size];
    textMessageInput.Receive(textMessage, message_size);
    
    for (int i = 0; i < messages.size(); i++) {
        messages[i].position -= text_speed;
        // Get width of the text
        float width = font.stringWidth(messages[i].text);
        
        if (messages[i].position + width < 0) {
            messages[i].position = WINDOW_WIDTH;
        }
    }
    
//    int currentTime = ofGetElapsedTimeMillis()/1000;
//    if (currentTime % 3 == 0 and message_trigger and currentTime > 1) {
//        string message = "new message";
//        int success = toPython.Send(message.c_str(), message.length());
//        message_trigger = false;
//    }
//    if (currentTime % 2 == 0 and currentTime % 3 != 0){
//        message_trigger = true;
//    }
//    
//    if (textMessage[0] != 0) {
//        message = textMessage;
//    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    controlWindow.begin();
    ofClear(0, 0, 0);
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
        ofDrawBitmapString(reportStream.str(), 50,150);
    }
    controlWindow.end();
    // Clear with alpha, so we can capture via syphon and composite elsewhere should we want.
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //    int imgWidth = current_picture.width;
    //    int imgHeight = current_picture.height;
    //    float widthRatio = WINDOW_WIDTH / imgWidth;
    //    float heightRatio = WINDOW_HEIGHT / imgHeight;
    //    if (widthRatio < heightRatio) {
    //        widthRatio = heightRatio;
    //    }
    //    else
    //        heightRatio = widthRatio;
    //    current_picture.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
    font.drawString(messages[0].text, messages[0].position, 50);

    syphon_out.publishScreen();

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
                message = "start-stop";
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
    if (key == 's') {
        string message = "Shutdown";
        int success = toPython.Send(message.c_str(), message.length());
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
