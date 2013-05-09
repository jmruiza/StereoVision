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

    CameraCalibrator CamCalib;

    void demo();

    void CaptureFormCameras();
    void CalibrateCameras(const char nameFile[]);
    void getDataFromFile(cv::FileStorage &file);

    // Getters & Setters
    void setN_Images(int n_image);
    void setFileName(const char name[]);
    void setLeftDevice(int device);
    void setRightDevice(int device);
    void setLeftCameraMatrix(cv::Mat matrix);
    void setRightCameraMatrix(cv::Mat matrix);

    int getN_Images();
    const char* getFileName();
    int getLeftDevice();
    int getRightDevice();
    cv::Mat getLeftCameraMatrix();
    cv::Mat setRightCameraMatrix();

private:
    const char* fileName;
    int N_Images;
    int LeftDevice,RightDevice;
    cv::Mat LeftCameraMatrix, RightCameraMatrix;
    cv::Mat LeftDistCoeff, RightDistCoeff;
};

#endif // STEREOCALIBRATOR_H
