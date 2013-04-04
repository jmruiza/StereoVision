#include "stereovision.h"

StereoVision::StereoVision()
{

}

void StereoVision::processImages(){
    double alpha = 2.2; // Gain (1.0 - 3.0)
    int beta = 1;      // Bias (0 - 100)

    tempIzq.create( imageIzq.rows, imageIzq.cols, imageIzq.type() );
    tempDer.create( imageDer.rows, imageDer.cols, imageDer.type() );

    imageIzq.convertTo(tempIzq, -1, alpha, beta);
    imageDer.convertTo(tempDer, -1, alpha, beta);

    cv::threshold(tempIzq, imageIzq, 130.0, 255, cv::THRESH_BINARY);
    cv::threshold(tempDer, imageDer, 130.0, 255, cv::THRESH_BINARY);

    //tempIzq.copyTo(imageIzq);
    //tempDer.copyTo(imageDer);
}


void StereoVision::set_image_izq(cv::Mat image){
    imageIzq_original = image;
    imageIzq_original.copyTo(imageIzq);
}

void StereoVision::set_image_der(cv::Mat image){
    imageDer_original = image;
    imageDer_original.copyTo(imageDer);
}

cv::Mat StereoVision::get_image_izq_original(){
    return imageIzq_original;
}

cv::Mat StereoVision::get_image_der_original(){
    return imageDer_original;
}

cv::Mat StereoVision::get_image_izq_processed(){
    return imageIzq;
}

cv::Mat StereoVision::get_image_der_processed(){
    return imageDer;
}
