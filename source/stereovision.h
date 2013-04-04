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

    // Getters and Setters
    void set_image1(cv::Mat image);
    void set_image2(cv::Mat image);
    cv::Mat get_image1_original();
    cv::Mat get_image2_original();
    cv::Mat get_image1_processed();
    cv::Mat get_image2_processed();

private:
    cv::Mat imageCam1_original;
    cv::Mat imageCam1;
    cv::Mat imageCam2_original;
    cv::Mat imageCam2;
    cv::Mat temp1;
    cv::Mat temp2;
};

#endif // STEREOVISION_H
