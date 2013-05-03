#ifndef CAMERACALIBRATOR_H
#define CAMERACALIBRATOR_H

#include <vector>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

class CameraCalibrator {

private:
    // points to find
    std::vector< std::vector<cv::Point3f> > objectPoints;
    std::vector< std::vector<cv::Point2f> > imagePoints;

    // output Matrix
    cv::Mat cameraMatrix;
    cv::Mat distCoeffs;

    // flag to specify how calibration is done
    int flag;

    // used in image undistortion
    cv::Mat map1, map2;
    bool mustInitUndistort;

public:
    /**
     * @brief   CameraCalibrator
     * @def     Constructor (single)
     */
    CameraCalibrator();

    /**
     * @brief   addChessboardPoints
     * @def     Open the chessboard images and extract corner points
     * @param   filelist - List of the images to find the corner points
     * @param   boardSize - size of pattern (number of vertical and horizontal squares)
     * @return  number of successful processed images
     */
    int addChessboardPoints(const std::vector<std::string>& filelist, cv::Size & boardSize);

    /**
     * @brief   addPoints
     * @def     Add found object points and corresponding image points
     * @param   imageCorners - 2D points of corners
     * @param   objectCorners - 3D points of corners
     */
    void addPoints(const std::vector<cv::Point2f>& imageCorners, const std::vector<cv::Point3f>& objectCorners);

    /**
     * @brief   calibrate
     * @def     Calibrate the camera
     * @param   imageSize
     * @return  Re-Proyection error
     */
    double calibrate(cv::Size &imageSize);

    /**
     * @brief   setCalibrationFlag
     * @def     Set the calibration options
     * @param   radial8CoeffEnabled - should be true if 8 radial coefficients are required (5 is default)
     * @param   tangentialParamEnabled - should be true if tangeantial distortion is present
     */
    void setCalibrationFlag(bool radial8CoeffEnabled=false, bool tangentialParamEnabled=false);

    /**
     * @brief   remap
     * @def     Remove distortion to image
     * @param   distorted image
     * @return  undistorted image
     */
    cv::Mat remap(const cv::Mat &image);

    // Getters
    /**
     * @brief   getCameraMatrix
     * @return  cameraMatrix
     */
    cv::Mat getCameraMatrix();

    /**
     * @brief   getDistCoeffs
     * @return  distCoeffs
     */
    cv::Mat getDistCoeffs();
};

#endif // CAMERACALIBRATOR_H
