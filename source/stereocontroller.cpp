#include "stereocontroller.h"

StereoController::StereoController()
{
    fileName = "conf.xml";
}

bool StereoController::ExistConfigurationFile(){
    if(ConfigurationFile.open(fileName, ConfigurationFile.READ)){
        return true;
    }
    else{
        ConfigurationFile.release();
        return false;
    }
}



void StereoController::demo_cube(){
    Fourier fourier;
    cv::Mat imageLeft, imageF_Left, imageFI_Left, phase_Left;
    cv::Mat imageRight, imageF_Right, imageFI_Right, phase_Right;
    cv::Mat diffPhase;
    std::vector<cv::Mat_<float> > ftL, ftiL;
    std::vector<cv::Mat_<float> > ftR, ftiR;

    // Reading Images
    imageLeft = cv::imread("../../resources/images/face01-Left.jpg", 0);
    imageRight = cv::imread("../../resources/images/face01-Right.jpg", 0);

    // For Left image
    ftL = fourier.FourierDFT(imageLeft);
    fourier.FFTShift(ftL);
    fourier.LobeFilter(ftL, 1, 150);
    fourier.FFTShift(ftL);
    ftiL = fourier.FourierDFTInverse(ftL);

    imageF_Left = fourier.getImageMagnitude(ftL);
    imageFI_Left = fourier.getImageMagnitude(ftiL);
    phase_Left = fourier.UnwrappedPhase(ftiL);

    // For Right image
    ftR = fourier.FourierDFT(imageRight);
    fourier.FFTShift(ftR);
    fourier.LobeFilter(ftR, 1, 150);
    fourier.FFTShift(ftR);
    ftiR = fourier.FourierDFTInverse(ftR);

    imageF_Right = fourier.getImageMagnitude(ftR);
    imageFI_Right = fourier.getImageMagnitude(ftiR);
    phase_Right = fourier.UnwrappedPhase(ftiR);

    diffPhase = fourier.PhaseDiference(phase_Left, phase_Right);
    // Display images
/*
    namedWindow("Left Image");
    namedWindow("Left Image Fourier");
    moveWindow("Left Image", 50, 50);
    moveWindow("Left Image Fourier", 700, 50);

    cv::normalize(imageF_Left, imageF_Left, 0, 1, CV_MINMAX);
    cv::normalize(imageFI_Left, imageFI_Left, 0, 1, CV_MINMAX);
    cv::normalize(phase_Left, phase_Left, 0, 1, CV_MINMAX);

    imshow("Left Image",imageLeft);
    imshow("Left Image Fourier", imageF_Left);
    imshow("Left Image Fourier Inverse", imageFI_Left);
    imshow("Left Unwrapped Phase", phase_Left);
*/
    cv::normalize(diffPhase, diffPhase, 0, 1, CV_MINMAX);
    cv::imshow("Diff Phase", diffPhase);



    cv::waitKey();

}
