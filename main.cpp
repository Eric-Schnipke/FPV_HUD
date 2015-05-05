//
//  main.cpp
//  Blinking Light Tracker
//
//  Created by Eric Schnipke on 5/1/15.
//  Copyright (c) 2015 Eric Schnipke. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "Camera.h"

/* Camera feed frame titles */
const cv::String FRAME_TITLE1 = "BGR Frame";
const cv::String FRAME_TITLE2 = "HLS Frame";
const cv::String FRAME_TITLE3 = "Tracking Frame";

const unsigned short int CAMERA_ADDRESS = 0;
const unsigned short int FRAME_DELAY = 1;

int main(int argc, const char * argv[])
{
    /* camera object with which will track */
    camera thisCamera;
    
    /* matrix in which to store our video frame */
    cv::Mat frame;
    
    /* user control character */
    char controlChar;   
    
    /* create a new camera object and initialize to default webcam address */
    thisCamera.initialize(CAMERA_ADDRESS);
    
    /* enter main loop */
    while(controlChar != 'q')
    {
        /* advance the camera one frame */
        thisCamera.getFrame(frame);
        thisCamera.blurFrame(frame);
        thisCamera.showFrame(FRAME_TITLE1, frame);
        thisCamera.convertFrameToHls(frame);
        thisCamera.showFrame(FRAME_TITLE2, frame);
        thisCamera.filterHlsFrameToBinary(frame);
        thisCamera.reduceStatic(frame);
        thisCamera.showFrame("Binary Image", frame);
        thisCamera.trackObjects(frame);
        thisCamera.showFrame(FRAME_TITLE3, frame);
        
        /* capture program control character */
        controlChar = cv::waitKey(FRAME_DELAY);
    }
    
    /* return camera resources back to system */
    thisCamera.destroy();
    
    return 0;
}
