//
//  pythonThread.h
//  joliette_150
//
//  Created by Aaron Krajeski on 2014-10-22.
//
//

#pragma once
#include "ofThread.h"


class PythonThread : public ofThread {
public:
    PythonThread(){}
    void start() {
        startThread();
    }
    void threadedFunction() {
        string command = "python "+ofToDataPath("of_controller.py")+" "+ofToDataPath("");
        cout << command << endl;
        system(command.c_str());
    }
    void stop() {
        stopThread();
    }
};