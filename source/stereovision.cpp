#include "stereovision.h"
#include "harrisDetector.h"

StereoVision::StereoVision(){
}

void StereoVision::processImages(){
    double alpha = 2.2; // Gain (1.0 - 3.0)
    int beta = 1;      // Bias (0 - 100)

    tempLeft.create( imageLeft.rows, imageLeft.cols, imageLeft.type() );
    tempRight.create( imageRight.rows, imageRight.cols, imageRight.type() );

    imageLeft.convertTo(tempLeft, -1, alpha, beta);
    imageRight.convertTo(tempRight, -1, alpha, beta);

    tempLeft.copyTo(imageLeft);
    tempRight.copyTo(imageRight);

    // Create a HarrisDetector Instance
    HarrisDetector harris;
    std::vector<cv::Point> pts;

    // Compute Harris Values to left image;
    harris.detect(imageLeft);
    // Detect Harris Corners
    harris.getCorners(pts, 0.01);
    // Draw Harris corners
    imageLeft_original.copyTo(imageLeft);
    harris.drawOnImage(imageLeft, pts);
    pts.erase(pts.begin(), pts.end());

    // Compute Harris Values to Right image;
    harris.detect(imageRight);
    // Detect Harris Corners
    harris.getCorners(pts, 0.01);
    // Draw Harris corners
    imageRight_original.copyTo(imageRight);
    harris.drawOnImage(imageRight, pts);
    pts.erase(pts.begin(), pts.end());
}


void StereoVision::set_image_Left(cv::Mat image){
    imageLeft_original = image;
    if(image.channels() > 1)
        cv::cvtColor(image, imageLeft, cv::COLOR_BGR2GRAY);
    else
        imageLeft_original.copyTo(imageLeft);
}

void StereoVision::set_image_Right(cv::Mat image){
    imageRight_original = image;
    if(image.channels() > 1)
        cv::cvtColor(image, imageRight, cv::COLOR_BGR2GRAY);
    else
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
