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
    void set_image_izq(cv::Mat image);
    void set_image_der(cv::Mat image);
    cv::Mat get_image_izq_original();
    cv::Mat get_image_der_original();
    cv::Mat get_image_izq_processed();
    cv::Mat get_image_der_processed();

private:
    cv::Mat tempIzq;
    cv::Mat imageIzq;
    cv::Mat imageIzq_original;
    cv::Mat tempDer;
    cv::Mat imageDer;
    cv::Mat imageDer_original;


};

#endif // STEREOVISION_H
