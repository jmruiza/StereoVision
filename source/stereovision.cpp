#include "stereovision.h"

StereoVision::StereoVision()
{

}

void StereoVision::processImages(){
    double alpha = 2.2; // Gain (1.0 - 3.0)
    int beta = 1;      // Bias (0 - 100)

    temp1.create( imageCam1.rows, imageCam1.cols, imageCam1.type() );
    temp2.create( imageCam2.rows, imageCam2.cols, imageCam2.type() );

    imageCam1.convertTo(temp1, -1, alpha, beta);
    imageCam2.convertTo(temp2, -1, alpha, beta);

    cv::threshold(temp1, imageCam1, 130.0, 255, cv::THRESH_BINARY);
    cv::threshold(temp2, imageCam2, 130.0, 255, cv::THRESH_BINARY);

    //temp1.copyTo(imageCam1);
    //temp2.copyTo(imageCam2);
}


void StereoVision::set_image1(cv::Mat image){
    imageCam1_original = image;
    imageCam1_original.copyTo(imageCam1);
}

void StereoVision::set_image2(cv::Mat image){
    imageCam2_original = image;
    imageCam2_original.copyTo(imageCam2);
}

cv::Mat StereoVision::get_image1_original(){
    return imageCam1_original;
}

cv::Mat StereoVision::get_image2_original(){
    return imageCam2_original;
}

cv::Mat StereoVision::get_image1_processed(){
    return imageCam1;
}

cv::Mat StereoVision::get_image2_processed(){
    return imageCam2;
}
