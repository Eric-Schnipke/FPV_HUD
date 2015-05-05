//
//  Camera.h
//  Blinking Light Tracker
//
//  Created by Eric Schnipke on 5/1/15.
//  Copyright (c) 2015 Eric Schnipke. All rights reserved.
//

#ifndef __Blinking_Light_Tracker__Camera__
#define __Blinking_Light_Tracker__Camera__

#include <opencv2/opencv.hpp>

#endif /* defined(__Blinking_Light_Tracker__Camera__) */

class camera
{
    
public:
    /********************************************************************\
     * Summary: Initializes capturing capabilities of this camera,      *
     *          including camera address and frame size.                *
    \********************************************************************/
    void initialize(unsigned short);
    
    /*****************************************************\
     * Summary: Returns camera resources back to system. *
    \*****************************************************/
    void destroy();
    
    /*************************************************\
     * Summary: Stores next camera frame into memory *
    \*************************************************/
    void getFrame(cv::Mat&);
    
    /*************************************************\
     * Summary: Converts camera feed from BGR to HLS *
    \*************************************************/
    void convertFrameToHls(cv::Mat&);
    
    /****************************************************\
     * Summary: Converts camera feed from HLS to Binary *
    \****************************************************/
    void filterHlsFrameToBinary(cv::Mat&);
    
    /********************************\
     * Summary: Inverts camera feed *
    \********************************/
    void invertFrame(cv::Mat&);
    
    /********************************\
     * Summary: Blurs camera feed *
    \********************************/
    void blurFrame(cv::Mat&);
    
    /*******************************************************************\
     * Summary: Reduces static in frame using morphological operations *
    \*******************************************************************/
    void reduceStatic(cv::Mat&);
    
    /*****************************\
     * Summary: Tracks an object *
    \*****************************/
    void trackObjects(cv::Mat&);
    
    /****************************************************\
     * Summary: Displays buffered camera frame onscreen *
    \****************************************************/
    void showFrame(cv::String, cv::Mat&);
    
private:
    /* video capture object to acquire webcam feed */
    cv::VideoCapture capture;
};
