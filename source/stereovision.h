/*
    Creator: Juan Manuel Ruiz
    Date: April 3th 2013

    This class handles the attributes of the camera and capture method that returns an image.
*/

#ifndef STEREOVISION_H
#define STEREOVISION_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <iostream>

class StereoVision{

public:
    StereoVision();

    void processImages();
    void improve_ContrastBright();
    void detectCorners();

    // Getters and Setters
    void set_image_Left(cv::Mat image);
    void set_image_Right(cv::Mat image);
    cv::Mat get_image_Left_original();
    cv::Mat get_image_Right_original();
    cv::Mat get_image_Left_processed();
    cv::Mat get_image_Right_processed();

private:
    cv::Mat tempLeft;
    cv::Mat imageLeft;
    cv::Mat imageLeft_original;
    cv::Mat tempRight;
    cv::Mat imageRight;
    cv::Mat imageRight_original;


};

#endif // STEREOVISION_H
