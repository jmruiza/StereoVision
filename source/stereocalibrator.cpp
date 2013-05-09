#include "stereocalibrator.h"

StereoCalibrator::StereoCalibrator(){

}

void StereoCalibrator::demo(){
    setFileName("conf.xml");

    cv::FileStorage file;
    if(file.open(getFileName(), file.READ)){
        std::cout << "Read data from: \"" << getFileName() << "\"" << std::endl;
        getDataFromFile(file);
    }
    else{
        file.release();
        std::cout << "Calibrar: " << std::endl;
        //CaptureFormCameras();
        CalibrateCameras(getFileName());
    }

    cv::namedWindow("From Camera");
    cv::namedWindow("Rectify");
    cv::moveWindow("From Camera", 0, 0);
    cv::moveWindow("Rectify", 700, 0);

    CamCalib.setCameraMatrix(LeftCameraMatrix);
    CamCalib.setDistCoeffs(LeftDistCoeff);
    cv::Mat image = cv::imread("../../resources/images/IM01L.jpg");
    cv::Mat image1 = CamCalib.remap(image);

    cv::imshow("From Camera", image);
    cv::imshow("Rectify", image1);

    cv::waitKey();
    cv::destroyAllWindows();
}

void StereoCalibrator::CaptureFormCameras(){
    int devLeft = getLeftDevice();
    int devRight = getRightDevice();
    int n_images = getN_Images();

    // Camera class intance
    Cam Camera;

    // Change the resolutions and activate devices
    Camera.Devices[devLeft].resolution_active = Camera.Devices[devLeft].resolutions[8];
    Camera.Devices[devRight].resolution_active = Camera.Devices[devRight].resolutions[8];

    // Activate the devices
    Camera.changeStatus(devLeft);
    Camera.changeStatus(devRight);
    int imgCont = 1;

    cv::namedWindow("Left Camera");
    cv::namedWindow("Right Camera");
    cv::moveWindow("Left Camera", 0, 0);
    cv::moveWindow("Right Camera", 700, 0);

    while ( imgCont<= n_images ) {
        Camera.streamImage();

        cv::Mat black_screen = cv::Mat::zeros(Camera.Devices[devLeft].image_buffer.rows,
                                              Camera.Devices[devLeft].image_buffer.cols,
                                              Camera.Devices[devLeft].image_buffer.type());

        // Show images dynamically
        cv::imshow("Left Camera", Camera.Devices[devLeft].image_buffer);
        cv::imshow("Right Camera", Camera.Devices[devRight].image_buffer);
        cv::waitKey(1000);

        // Show images dynamically
        cv::imshow("Left Camera", black_screen);
        cv::imshow("Right Camera", black_screen);
        cv::waitKey(100);

        // Save images dynamically
        std::stringstream strLeft;
        strLeft << "../../resources/images/IM" << std::setw(2) << std::setfill('0') << imgCont << "L.jpg";
        cv::imwrite( strLeft.str().c_str(), Camera.Devices[devLeft].image_buffer);
        std::stringstream strRight;
        strRight << "../../resources/images/IM" << std::setw(2) << std::setfill('0') << imgCont << "R.jpg";
        cv::imwrite( strRight.str().c_str(), Camera.Devices[devRight].image_buffer);

        std::cout << "Saved: " << strLeft.str().c_str() << std::endl;
        std::cout << "Saved: " << strRight.str().c_str() << std::endl;

        // Increment image counter
        imgCont++;
    }

    std::cout << "Press any key to finish.." << std::endl;
    cv::waitKey();
    cv::destroyAllWindows();
 }

void StereoCalibrator::CalibrateCameras(const char nameFile[]){
    cv::Mat imageL, imageR;

    // Create vector to save all images name
    std::vector<std::string> filelistL;
    std::vector<std::string> filelistR;

    cv::namedWindow("Left Camera");
    cv::namedWindow("Right Camera");
    cv::moveWindow("Left Camera", 0, 0);
    cv::moveWindow("Right Camera", 700, 0);

    // Generate the name of files dynamically
    for (int i=1; i<=20; i++) {
        // Generate the name with path of files dynamically.
        std::stringstream strLeft;
        strLeft << "../../resources/images/IM" << std::setw(2) << std::setfill('0') << i << "L.jpg";
        std::stringstream strRight;
        strRight << "../../resources/images/IM" << std::setw(2) << std::setfill('0') << i << "R.jpg";

        // Add name to vector
        filelistL.push_back(strLeft.str());
        filelistR.push_back(strRight.str());

        // Show image
        imageL = cv::imread(strLeft.str(), 0);
        imageR = cv::imread(strRight.str(), 0);

        cv::imshow("Left Camera", imageL);
        cv::imshow("Right Camera", imageR);
        cv::waitKey(100);
    }
    cv::destroyAllWindows();

    // Se especifica el numero de esquinas verticales y horizontales internas
    cv::Size boardSize(8,6);
    cv::Size size;

    // Calibrate Left device
    CamCalib.addChessboardPoints(filelistL, boardSize);
    CamCalib.setCalibrationFlag(true, true);
    size = imageL.size();
    CamCalib.calibrate(size);
    LeftCameraMatrix = CamCalib.getCameraMatrix();
    LeftDistCoeff = CamCalib.getDistCoeffs();

    // Calibrate Right device
    CamCalib.addChessboardPoints(filelistR, boardSize);
    CamCalib.setCalibrationFlag(true, true);
    size = imageR.size();
    CamCalib.calibrate(size);
    RightCameraMatrix = CamCalib.getCameraMatrix();
    RightDistCoeff = CamCalib.getDistCoeffs();

    // Save in file and show
    cv::FileStorage file(nameFile, cv::FileStorage::WRITE);
    file << "LeftCameraMatrix" << LeftCameraMatrix;
    file << "LeftDistCoeffs" << LeftDistCoeff;
    file << "RightCameraMatrix" << RightCameraMatrix;
    file << "RightDistCoeffs" << RightDistCoeff;
    file.release();
    std::cout << "Saved in: \"" << nameFile << "\"" << std::endl;
}

void StereoCalibrator::getDataFromFile(cv::FileStorage &file){
    file["LeftCameraMatrix"] >> LeftCameraMatrix;
    file["LeftDistCoeffs"] >> LeftDistCoeff;
    file["RightCameraMatrix"] >> RightCameraMatrix;
    file["RightDistCoeffs"] >> RightDistCoeff;
}

// Getter and Setters
void StereoCalibrator::setN_Images(int n_image){
    N_Images = n_image;
}

void StereoCalibrator::setFileName(const char name[]){
    fileName = name;
}

void StereoCalibrator::setLeftDevice(int device){
    LeftDevice = device;
}

void StereoCalibrator::setRightDevice(int device){
    RightDevice = device;
}

void StereoCalibrator::setLeftCameraMatrix(cv::Mat matrix){
    LeftCameraMatrix = matrix;
}

void StereoCalibrator::setRightCameraMatrix(cv::Mat matrix){
    RightCameraMatrix = matrix;
}

int StereoCalibrator::getN_Images(){
    return N_Images;
}

const char* StereoCalibrator::getFileName(){
    return fileName;
}

int StereoCalibrator::getLeftDevice(){
    return LeftDevice;
}

int StereoCalibrator::getRightDevice(){
    return RightDevice;
}

cv::Mat StereoCalibrator::getLeftCameraMatrix(){
    return LeftCameraMatrix;
}

cv::Mat StereoCalibrator::setRightCameraMatrix(){
    return RightCameraMatrix;
}
