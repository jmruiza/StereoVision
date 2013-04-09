/*
    Creator: Juan Manuel Ruiz
    Date: April 3th 2013

    This class handles the attributes of the camera and capture method that returns an image.
*/

#ifndef STEREOVISION_H
#define STEREOVISION_H

#include <opencv2/core/core.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/nonfree/features2d.hpp>

#include <iostream>

class StereoVision{

public:
    StereoVision();

    void processImages();
    void improve_ContrastBright();
    void detectCorners();
    void matchesDetector();

    void generate_image_Fringes(int n=1);
    cv::Mat convertToInterger(const cv::Mat &image);
    void normalizateMatrix(cv::Mat &img, int min, int max);
    void getImageRangeDouble(const cv::Mat &image, double &min, double &max);

    // Getters and Setters
    void set_number_of_images(int n);
    void set_image_Left(cv::Mat image);
    void set_image_Right(cv::Mat image);
    void set_image_Fringes(cv::Mat image);

    int get_number_of_images();
    cv::Mat get_image_Left_original();
    cv::Mat get_image_Right_original();
    cv::Mat get_image_Left_processed();
    cv::Mat get_image_Right_processed();
    cv::Mat get_image_Matches();  
    cv::Mat get_image_Fringes();

private:
    double w;
    double alpha;
    int number_of_images;

    cv::Mat tempLeft;
    cv::Mat imageLeft;
    cv::Mat imageLeft_original;
    cv::Mat tempRight;
    cv::Mat imageRight;
    cv::Mat imageRight_original;
    cv::Mat imageMatches;
    cv::Mat imageFringes;
};

#endif // STEREOVISION_H
