#include "stereovision.h"

StereoVision::StereoVision()
{

}

void StereoVision::processImages(){
    double alpha = 2.2; // Gain (1.0 - 3.0)
    int beta = 1;      // Bias (0 - 100)

    tempLeft.create( imageLeft.rows, imageLeft.cols, imageLeft.type() );
    tempRight.create( imageRight.rows, imageRight.cols, imageRight.type() );

    imageLeft.convertTo(tempLeft, -1, alpha, beta);
    imageRight.convertTo(tempRight, -1, alpha, beta);

    // Detect Harris Corners
    cv::Mat cornerStrength;
    cv::cornerHarris(imageLeft,cornerStrength,
                  3,     // neighborhood size
                     3,     // aperture size
                     0.01); // Harris parameter


    cv::threshold(tempLeft, imageLeft, 130.0, 255, cv::THRESH_BINARY);
    cv::threshold(tempRight, imageRight, 130.0, 255, cv::THRESH_BINARY);

    //tempLeft.copyTo(imageLeft);
    //tempRight.copyTo(imageRight);

    // Finding

}


void StereoVision::set_image_Left(cv::Mat image){
    imageLeft_original = image;
    imageLeft_original.copyTo(imageLeft);
}

void StereoVision::set_image_Right(cv::Mat image){
    imageRight_original = image;
    imageRight_original.copyTo(imageRight);
}

cv::Mat StereoVision::get_image_Left_original(){
    return imageLeft_original;
}

cv::Mat StereoVision::get_image_Right_original(){
    return imageRight_original;
}

cv::Mat StereoVision::get_image_Left_processed(){
    return imageLeft;
}

cv::Mat StereoVision::get_image_Right_processed(){
    return imageRight;
}
