#include <iostream>
#include <iomanip>
#include <fstream>
#include <time.h>

#include "cameracalibrator.h"
#include "stereocontroller.h"



using namespace std;
using namespace cv;

void CaptureFormCamera();
void CalibrateCamera(char nameFile[]);
void getDataOfFile(FileStorage &file);

int main(){
    /*
    StereoController stereoCont;
    stereoCont.demo_cube();
    */

    //ifstream file("conf.dat");
    //if(file){

    char nameFile[] = "conf.xml";
    FileStorage file;
    if(file.open(nameFile, file.READ)){
        cout << "Read data from: \"" << nameFile << "\"" << endl;
        getDataOfFile(file);
    }
    else{
        file.release();
        cout << "Calibrar: " << endl;
        //CaptureFormCamera();
        CalibrateCamera(nameFile);
    }

    CameraCalibrator CamCal;
    cv::namedWindow("From Camera");
    cv::namedWindow("Rectify");
    cv::moveWindow("From Camera", 0, 0);
    cv::moveWindow("Rectify", 700, 0);

    cv::Mat image = cv::imread("../../resources/images/IM01L.jpg");


    cv::imshow("From Camera", image);
    cv::imshow("Rectify", image);

    cv::waitKey();
    cv::destroyAllWindows();

/*
    // Capture
    int keypress = 0;

    int devLeft = 2;
    int devRight = 1;

    // StereoVision class instance
    StereoVision Stereo;

    // Camera class intance
    Cam Camera;

    // Change the resolutions and activate devices 
    Camera.Devices[devLeft].resolution_active = Camera.Devices[devLeft].resolutions[8];
    Camera.Devices[devRight].resolution_active = Camera.Devices[devRight].resolutions[8];

    // Activate the devices
    Camera.changeStatus(devLeft);
    Camera.changeStatus(devRight);

    // Set the number of images to process
    Stereo.set_number_of_images(7);

    int imgCont = 1;
    while ( 1 ) {
        // Generate and show fringes pattern
        Stereo.generate_image_Fringes(imgCont);

        Camera.streamImage();

        // Show images dynamically
        cv::imshow("Left Camera", Camera.Devices[devLeft].image_buffer);
        cv::imshow("Right Camera", Camera.Devices[devRight].image_buffer);

        // Eliminate High bits in keypress with AND operator
        keypress = cvWaitKey(1) & 255;

        // If the press key Esc: End of capture
        if( keypress == 27 || imgCont > Stereo.get_number_of_images() ){
            break;
        }

        // If the press key Enter: Save the image
        if( keypress == 13 ){
            // Save images dynamically
            stringstream strLeft;
            strLeft << "../../resources/images/face" << setw(2) << setfill('0') << imgCont << "-Left.jpg";
            cv::imwrite( strLeft.str().c_str(), Camera.Devices[devLeft].image_buffer);
            stringstream strRight;
            strRight << "../../resources/images/face" << setw(2) << setfill('0') << imgCont << "-Right.jpg";
            cv::imwrite( strRight.str().c_str(), Camera.Devices[devRight].image_buffer);

            // Increment image counter
            imgCont++;
        }
    }
*/
    // std::cout << "Press any key to finish.." << std::endl;
    // cv::waitKey();
    //cvDestroyAllWindows();
}

void CaptureFormCamera(){
    int devLeft = 0;
    int devRight = 1;
    int n_images = 20;

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
        stringstream strLeft;
        strLeft << "../../resources/images/IM" << setw(2) << setfill('0') << imgCont << "L.jpg";
        cv::imwrite( strLeft.str().c_str(), Camera.Devices[devLeft].image_buffer);
        stringstream strRight;
        strRight << "../../resources/images/IM" << setw(2) << setfill('0') << imgCont << "R.jpg";
        cv::imwrite( strRight.str().c_str(), Camera.Devices[devRight].image_buffer);

        cout << "Saved: " << strLeft.str().c_str() << endl;
        cout << "Saved: " << strRight.str().c_str() << endl;

        // Increment image counter
        imgCont++;
    }

    std::cout << "Press any key to finish.." << std::endl;
    cv::waitKey();
    cv::destroyAllWindows();
 }

void CalibrateCamera( char nameFile[] ){
    cv::Mat imageL, imageR;

    // Create vector to save all images name
    vector<string> filelistL;
    vector<string> filelistR;

    cv::namedWindow("Left Camera");
    cv::namedWindow("Right Camera");
    cv::moveWindow("Left Camera", 0, 0);
    cv::moveWindow("Right Camera", 700, 0);

    // Generate the name of files dynamically
    for (int i=1; i<=20; i++) {
        // Generate the name with path of files dynamically.
        stringstream strLeft;
        strLeft << "../../resources/images/IM" << setw(2) << setfill('0') << i << "L.jpg";
        stringstream strRight;
        strRight << "../../resources/images/IM" << setw(2) << setfill('0') << i << "R.jpg";

        // Se agrega el nombre al vector
        filelistL.push_back(strLeft.str());
        filelistR.push_back(strRight.str());

        // Se muestra cada imagen
        imageL = imread(strLeft.str(), 0);
        imageR = imread(strRight.str(), 0);

        imshow("Left Camera", imageL);
        imshow("Right Camera", imageR);

        // Retardo para desplegar las imagenes
        cv::waitKey(100);
    }
    cv::destroyAllWindows();

    // Se crea un objeto de la clase CameraCalibrator
    CameraCalibrator camCalibratorL;
    CameraCalibrator camCalibratorR;

    // Se especifica el numero de esquinas verticales y horizontales internas
    Size boardSize(8,6);

    // Se abren todas las imagenes y se extraen sus puntos de las esquinas
    camCalibratorL.addChessboardPoints(filelistL, boardSize);
    camCalibratorR.addChessboardPoints(filelistR, boardSize);

    // Calibrando la camara
    camCalibratorL.setCalibrationFlag(true, true);
    camCalibratorR.setCalibrationFlag(true, true);
    cv::Size sizeL = imageL.size();
    cv::Size sizeR = imageR.size();
    camCalibratorL.calibrate(sizeL);
    camCalibratorR.calibrate(sizeR);

    //Imagen sin distorcion
    //image = imread("PRUEBA01.png");
    //imshow("Imagen Original", image);
    //Mat uImage= cameraCalibrator.remap(image);

    // Mostrar la Matriz de la camara
    cv::Mat LeftcameraMatrix = camCalibratorL.getCameraMatrix();
    cv::Mat RightcameraMatrix = camCalibratorL.getCameraMatrix();

    FileStorage file(nameFile, FileStorage::WRITE);
    file << "LeftCameraMatrix" << LeftcameraMatrix;
    file << "RightCameraMatrix" << RightcameraMatrix;
    file.release();

    std::cout << "Left Camera Matrix: " << std::endl;
    std::cout << LeftcameraMatrix.at<double>(0,0) << "\t\t" << LeftcameraMatrix.at<double>(0,1) << "\t\t" << LeftcameraMatrix.at<double>(0,2) << std::endl;
    std::cout << LeftcameraMatrix.at<double>(1,0) << "\t\t" << LeftcameraMatrix.at<double>(1,1) << "\t\t" << LeftcameraMatrix.at<double>(1,2) << std::endl;
    std::cout << LeftcameraMatrix.at<double>(2,0) << "\t\t" << LeftcameraMatrix.at<double>(2,1) << "\t\t" << LeftcameraMatrix.at<double>(2,2) << std::endl << std::endl;
    std::cout << "Right Camera Matrix: " << std::endl;
    std::cout << RightcameraMatrix.at<double>(0,0) << "\t\t" << RightcameraMatrix.at<double>(0,1) << "\t\t" << RightcameraMatrix.at<double>(0,2) << std::endl;
    std::cout << RightcameraMatrix.at<double>(1,0) << "\t\t" << RightcameraMatrix.at<double>(1,1) << "\t\t" << RightcameraMatrix.at<double>(1,2) << std::endl;
    std::cout << RightcameraMatrix.at<double>(2,0) << "\t\t" << RightcameraMatrix.at<double>(2,1) << "\t\t" << RightcameraMatrix.at<double>(2,2) << std::endl << std::endl;
    std::cout << "Saved in: \"" << nameFile << "\"" << std::endl;
}

void getDataOfFile(FileStorage &file){
    cv::Mat LeftcameraMatrix, RightcameraMatrix;
    file["LeftCameraMatrix"] >> LeftcameraMatrix;
    file["RightCameraMatrix"] >> RightcameraMatrix;

    std::cout << "Left Camera Matrix: " << std::endl;
    std::cout << LeftcameraMatrix.at<double>(0,0) << "\t\t" << LeftcameraMatrix.at<double>(0,1) << "\t\t" << LeftcameraMatrix.at<double>(0,2) << std::endl;
    std::cout << LeftcameraMatrix.at<double>(1,0) << "\t\t" << LeftcameraMatrix.at<double>(1,1) << "\t\t" << LeftcameraMatrix.at<double>(1,2) << std::endl;
    std::cout << LeftcameraMatrix.at<double>(2,0) << "\t\t" << LeftcameraMatrix.at<double>(2,1) << "\t\t" << LeftcameraMatrix.at<double>(2,2) << std::endl;
    std::cout << "Right Camera Matrix: " << std::endl;
    std::cout << RightcameraMatrix.at<double>(0,0) << "\t\t" << RightcameraMatrix.at<double>(0,1) << "\t\t" << RightcameraMatrix.at<double>(0,2) << std::endl;
    std::cout << RightcameraMatrix.at<double>(1,0) << "\t\t" << RightcameraMatrix.at<double>(1,1) << "\t\t" << RightcameraMatrix.at<double>(1,2) << std::endl;
    std::cout << RightcameraMatrix.at<double>(2,0) << "\t\t" << RightcameraMatrix.at<double>(2,1) << "\t\t" << RightcameraMatrix.at<double>(2,2) << std::endl;
}

