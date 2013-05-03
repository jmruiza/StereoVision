#include "cameracalibrator.h"

// Constructor
CameraCalibrator::StereoCalibration(){
    flag = 0;
    mustInitUndistort = true;
}

// Open the chessboard images and extract corner points
int CameraCalibrator::addChessboardPoints( const std::vector<std::string>& filelist, cv::Size & boardSize) {

    // Corner points
    std::vector<cv::Point2f> imageCorners;
    std::vector<cv::Point3f> objectCorners;

    // Init 3D Points of corners (X,Y,Z)= (i,j,0)
    for (int i=0; i<boardSize.height; i++) {
        for (int j=0; j<boardSize.width; j++) {
            objectCorners.push_back(cv::Point3f(i, j, 0.0f));
        }
    }

    cv::Mat image;      // To contain the pattern image
    int successes = 0;

    // For all images
    for (int i=0; i<filelist.size(); i++) {
        // Load each image in temporal image
        image = cv::imread(filelist[i],0);
        // Get corners of each image
        bool found = cv::findChessboardCorners(image, boardSize, imageCorners);

        // Refines the corner locations
        cv::cornerSubPix( image, imageCorners,
                        cv::Size(5,5),
                        cv::Size(-1,-1),
                        cv::TermCriteria(
                              cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS,
                              30,   // The maximum number of iterations or elements to compute
                              0.1)  // The desired accuracy or change in parameters at which the iterative algorithm stops
                          );

        // Compare the number of found corners with the board area
        if(imageCorners.size() == boardSize.area()) {
            // Add found object points and corresponding image points
            addPoints(imageCorners, objectCorners);
            successes++;
        }

        // Draw corner in temporal image
        cv::drawChessboardCorners(image, boardSize, imageCorners, found);

        // Show image with found corners
        cv::imshow("Found corners", image);
        cv::waitKey(100);
    }
    cvDestroyWindow( "Found corners" );
    return successes;
}

// Add found image points (2D) and corresponding object points (3D)
void CameraCalibrator::addPoints(const std::vector<cv::Point2f>& imageCorners, const std::vector<cv::Point3f>& objectCorners) {
    // Image points 2D view
    imagePoints.push_back(imageCorners);
    // Object point 3D scene
    objectPoints.push_back(objectCorners);
}

// Calibrate the camera returns re-proyection error
double CameraCalibrator::calibrate(cv::Size &imageSize)
{
    // Undistort flag
    mustInitUndistort= true;

    // Rotation and translation Matrix (Output)
    std::vector<cv::Mat> rvecs, tvecs;

    // Calibrate
    return calibrateCamera(objectPoints,    // 3D points
                    imagePoints,            // Image points
                    imageSize,              // Image size
                    cameraMatrix,           // Camera Matrix
                    distCoeffs,             // Camera distortion coefficients
                    rvecs, tvecs,           // Rs, Ts
                    flag);                  // Configuration Option
                    // ,CV_CALIB_USE_INTRINSIC_GUESS);

}

// remove distortion in an image (after calibration)
cv::Mat CameraCalibrator::remap(const cv::Mat &image) {
    // Undistorted Image (return)
    cv::Mat undistorted;

    // Called once per calibration
    if (mustInitUndistort) {
        // Rectify image
        cv::initUndistortRectifyMap(
                cameraMatrix,       // Computed camera matrix
                distCoeffs,         // Computed distortion matrix
                cv::Mat(),          // Optional rectification (none)
                cv::Mat(),          // Camera matrix to generate undistorted
                cv::Size(640,480),  //
                // image.size(),    // size of undistorted
                CV_32FC1,           // type of output map
                map1, map2);        // the x and y mapping functions

        mustInitUndistort= false;
    }

    // Apply mapping functions
    cv::remap(image, undistorted, map1, map2, cv::INTER_LINEAR);
    return undistorted;
}


// Set the calibration options
// radial8CoeffEnabled - should be true if 8 radial coefficients are required (5 is default)
// tangentialParamEnabled - should be true if tangeantial distortion is present
void CameraCalibrator::setCalibrationFlag(bool radial8CoeffEnabled, bool tangentialParamEnabled) {
    // Set the flag used in cv::calibrateCamera()
    flag = 0;
    if (!tangentialParamEnabled)
        flag += CV_CALIB_ZERO_TANGENT_DIST;
    if (radial8CoeffEnabled)
        flag += CV_CALIB_RATIONAL_MODEL;
}

// Getters
cv::Mat CameraCalibrator::getCameraMatrix(){
    return cameraMatrix;
}

cv::Mat CameraCalibrator::getDistCoeffs(){
    return distCoeffs;
}
