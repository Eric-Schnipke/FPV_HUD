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
const int FRAME_WIDTH = 512;
const int FRAME_HEIGHT = 256;

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
 * Summary: Converts camera feed from BGR to HLS *
\*************************************************/
void camera::convertFrameToHls(cv::Mat &frame)
{
    /* convert frame from BGR to HLS colorspace */
    cv::cvtColor(frame, frame, cv::COLOR_BGR2HLS_FULL);
}

/****************************************************\
 * Summary: Converts camera feed from HLS to Binary *
\****************************************************/
void camera::filterHlsFrameToBinary(cv::Mat &frame)
{    
    /* filter HLS image between values and store filtered image to threshold matrix */
    cv::inRange(frame,
                cv::Scalar(0, 225, 0),
                cv::Scalar(25, 255, 40),
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
    cv::GaussianBlur(frame, frame, cv::Size(15, 15), 20);
}

/*******************************************************************\
 * Summary: Reduces static in frame using morphological operations *
\*******************************************************************/
void camera::reduceStatic(cv::Mat &frame)
{
    /* erode and dilate elements for morphological operations */
    cv::Mat erodeElement    = getStructuringElement( cv::MORPH_RECT,cv::Size(3,3));
    cv::Mat dilateElement   = getStructuringElement( cv::MORPH_RECT,cv::Size(3,3));
    
    /* remove detached whitespace */
    cv::erode(frame, frame, erodeElement);
    cv::erode(frame, frame, erodeElement);
    
    /* grow remaining whitespace */
    cv::dilate(frame, frame, dilateElement);
    cv::dilate(frame, frame, dilateElement);
}

/*****************************\
 * Summary: Tracks an object *
\*****************************/
void camera::trackObjects(cv::Mat &frame)
{    
    cv::vector<cv::vector<cv::Point> > contours;
    cv::vector<cv::Vec4i> hierarchy;
    
    /// Find contours
    cv::findContours( frame, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );
    
    /// Approximate contours to polygons + get bounding rects and circles
    cv::vector<cv::vector<cv::Point> > contours_poly( contours.size() );
    cv::vector<cv::Rect> boundRect( contours.size() );
    cv::vector<cv::Point2f>center( contours.size() );
    cv::vector<float>radius( contours.size() );
    
    for( int i = 0; i < contours.size(); i++ )
    { cv::approxPolyDP( cv::Mat(contours[i]), contours_poly[i], 13, true );
        boundRect[i] = boundingRect( cv::Mat(contours_poly[i]) );
        cv::minEnclosingCircle( (cv::Mat)contours_poly[i], center[i], radius[i] );
    }
    
    
    /// Draw polygonal contour + bonding rects + circles
    cv::Mat drawing = cv::Mat::zeros( frame.size(), CV_8UC3 );
    cv::Scalar color = cv::Scalar( 0, 255, 0 );
    for( int i = 0; i< contours.size(); i++ )
    {
        cv::drawContours( drawing, contours_poly, i, color, 1, 8, cv::vector<cv::Vec4i>(), 0, cv::Point() );
        cv::rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
        cv::circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
    }

    frame = drawing;
}

/****************************************************\
 * Summary: Displays buffered camera frame onscreen *
\****************************************************/
void camera::showFrame(cv::String frameTitle, cv::Mat &frame)
{
    cv::imshow(frameTitle, frame);
}

/* video capture object to acquire webcam feed */
cv::VideoCapture capture;
