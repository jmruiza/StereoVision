#include "stereovision.h"
#include "harrisDetector.h"

StereoVision::StereoVision(){
}

void StereoVision::processImages(){
    tempLeft.create( imageLeft.rows, imageLeft.cols, imageLeft.type() );
    tempRight.create( imageRight.rows, imageRight.cols, imageRight.type() );

    // Improve Contrast and Bright
    // improve_ContrastBright();

    // Detect corners
    // detectCorners();

    // Detect Matches
    matchesDetector();
}

void StereoVision::improve_ContrastBright(){
    double alpha = 2.2; // Gain (1.0 - 3.0)
    int beta = 1;      // Bias (0 - 100)
    imageLeft.convertTo(tempLeft, -1, alpha, beta);
    imageRight.convertTo(tempRight, -1, alpha, beta);

    tempLeft.copyTo(imageLeft);
    tempRight.copyTo(imageRight);
}

void StereoVision::detectCorners(){
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
    pts.clear();

    // Compute Harris Values to Right image;
    harris.detect(imageRight);
    // Detect Harris Corners
    harris.getCorners(pts, 0.01);
    // Draw Harris corners
    imageRight_original.copyTo(imageRight);
    harris.drawOnImage(imageRight, pts);
    pts.clear();
}

void StereoVision::matchesDetector(){
    // Detect the keypoints using SURF Detector
    int minHessian = 1000; // 400;

    cv::SurfFeatureDetector detector( minHessian );

    std::vector<cv::KeyPoint> keypointsLeft, keypointsRight;

    detector.detect( imageLeft, keypointsLeft );
    detector.detect( imageRight, keypointsRight );

    // Calculate descriptors (feature vectors)
    cv::SurfDescriptorExtractor extractor;

    cv::Mat descriptorsLeft, descriptorsRight;

    extractor.compute( imageLeft, keypointsLeft, descriptorsLeft );
    extractor.compute( imageRight, keypointsRight, descriptorsRight );

    // Matching descriptor vectors with a brute force matcher
    cv::BruteForceMatcher< cv::L2<float> > matcher;
    std::vector< cv::DMatch > matches;
    matcher.match( descriptorsLeft, descriptorsRight, matches );

    // Draw matches
    drawMatches( imageLeft, keypointsLeft, imageRight, keypointsRight, matches, imageMatches );
}

void StereoVision::generate_image_Fringes(int n){

    // Dimensions and definitions
    int M = 1400; // Colums
    int N = 1150; // Rows
    double w = 3.1416 / 10.0;//4.0;
    double alpha = 3.1416 / 2.0;

    // Generating s(x,y)
    cv::Mat s = cv::Mat::zeros(N, M, CV_64F);
    for(int y=0; y<N; y++)
        for(int x=0; x<M; x++)
            s.at<double>(y,x) = cos( w * x + alpha * n);

    // To display
    // s(x,y)
    normalizateMatrix(s, 0, 255);
    cv::Mat s_d = convertToInterger(s);
    cv::namedWindow("Fringe Pattern"); //, CV_WINDOW_NORMAL);
    cv::imshow("Fringe Pattern", s_d);
}

cv::Mat StereoVision::convertToInterger(const cv::Mat &image){
    cv::Mat im(image.rows, image.cols, CV_8U);
    for(int y=0; y<image.rows; y++){ // Rows y
        for(int x=0; x<image.cols; x++){ // Columns x
            im.at<uchar>(y,x) = image.at<double>(y,x) * 255;
        }
    }
    return im;
}

void StereoVision::normalizateMatrix(cv::Mat &img, int min, int max){
    double min2, max2;
    getImageRangeDouble(img, min2, max2 );

    for(int y=0; y<img.rows; y++){ // Rows y
        for(int x=0; x<img.cols; x++){ // Columns x
            img.at<double>(y,x) = ( img.at<double>(y,x) - min2 ) / (max2-min2);
        }
    }
}

void StereoVision::getImageRangeDouble(const cv::Mat &image, double &min, double &max){
    min = max = image.at<double>(0,0);
    for(int y=0; y<image.rows; y++){ // Rows y
        for(int x=0; x<image.cols; x++){ // Columns j
            if( image.at<double>(y,x) > max )
                max = image.at<double>(y,x);
            if( image.at<double>(y,x) < min )
                min = image.at<double>(y,x);
        }
    }
}

// ================================================================================================

void StereoVision::set_number_of_images(int n){
    number_of_images = n;
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

int StereoVision::get_number_of_images(){
    return number_of_images;
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

cv::Mat StereoVision::get_image_Matches(){
    return imageMatches;
}
