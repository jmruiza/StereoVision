/*
    Creator: Juan Manuel Ruiz
    Date: May 6th 2013

    This class controls the calibration for a pair of cameras (Stereo)
*/

#ifndef STEREOCALIBRATOR_H
#define STEREOCALIBRATOR_H

#include <vector>
#include <iomanip>
#include <iostream>


#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "camera.h"
#include "cameracalibrator.h"

class StereoCalibrator
{

public:
    StereoCalibrator();

    void CaptureFormCameras();
    void CalibrateCameras(char nameFile[]);
    void getDataFromFile(cv::FileStorage &file);

    // Getters & Setters
    void setLeftCameraMatrix(cv::Mat matrix);
    void setRightCameraMatrix(cv::Mat matrix);
    cv::Mat getLeftCameraMatrix();
    cv::Mat setRightCameraMatrix();

private:
    char fileName[];
    cv::Mat LeftCameraMatrix;
    cv::Mat RightCameraMatrix;
};

#endif // STEREOCALIBRATOR_H
