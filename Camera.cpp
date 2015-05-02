//
//  Camera.cpp
//  Blinking Light Tracker
//
//  Created by Eric Schnipke on 5/1/15.
//  Copyright (c) 2015 Eric Schnipke. All rights reserved.
//

#include "Camera.h"
#include <opencv2/opencv.hpp>

/* default frame width and height */
const int FRAME_WIDTH = 1280;
const int FRAME_HEIGHT = 720;

/********************************************************************\
 * Summary: Initializes capturing capabilities of this camera,      *
 *          including camera address and frame size.                *
\********************************************************************/
void camera::initialize(unsigned short usAddress)
{
    /* open capture object at address location (zero is default webcam location) */
    capture.open(usAddress);

    /* set height and width of capture frame */
    capture.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);
}

/*****************************************************\
 * Summary: Returns camera resources back to system. *
\*****************************************************/
void camera::destroy()
{
    capture.release();
}

/*************************************************\
 * Summary: Stores next camera frame into memory *
\*************************************************/
void camera::getFrame(cv::Mat &frame)
{
    capture.read(frame);
}

/*************************************************\
 * Summary: Converts camera feed from BGR to HSV *
\*************************************************/
void camera::convertFrameToHsv(cv::Mat &frame)
{
    /* convert frame from BGR to HSV colorspace */
    cv::cvtColor(frame, frame, cv::COLOR_BGR2HSV);
}

/****************************************************\
 * Summary: Converts camera feed from HSV to Binary *
\****************************************************/
void camera::convertHsvFrametoBinary(cv::Mat &frame)
{
    /* filter HSV image between values and store filtered image to threshold matrix */
    cv::inRange(frame,
                cv::Scalar(0, 0, 225),
                cv::Scalar(75, 50, 255),
                frame);
}

/********************************\
 * Summary: Inverts camera feed *
\********************************/
void camera::invertFrame(cv::Mat &frame)
{
    cv::bitwise_not(frame, frame);
}

/********************************\
 * Summary: Blurs camera feed *
\********************************/
void camera::blurFrame(cv::Mat &frame)
{
    cv::GaussianBlur(frame, frame, cv::Size(25, 25), 50);
}

/****************************************************\
 * Summary: Displays buffered camera frame onscreen *
\****************************************************/
void camera::showFrame(cv::String frameTitle, cv::Mat &frame)
{
    imshow(frameTitle, frame);
}

/* video capture object to acquire webcam feed */
cv::VideoCapture capture;
