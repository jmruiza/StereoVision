#include <iostream>
#include <iomanip>
#include <fstream>

#include "cameracalibrator.h"
#include "stereocontroller.h"



using namespace std;
using namespace cv;

void CaptureFormCamera();
void CalibrateCamera();
void getDataOfFile(ifstream *file);

int main(){
    /*
    StereoController stereoCont;
    stereoCont.demo_cube();
    */

    // Parametros iniciales
    // int accion;

    ifstream file("conf.dat");
    if(file){
        cout << "Recuperar datos del archivo..." << endl;
        getDataOfFile(&file);
    }
    else{
        cout << "Calibrar: " << endl;
        //CaptureFormCamera();
        CalibrateCamera();
    }
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

void CalibrateCamera(){
    // Se crea una imagen vacia
    Mat imageL, imageR;

    // Se crea un vector que almacenara todos los nombres de las imagenes
    vector<string> filelistL;
    vector<string> filelistR;

    // ciclo que genera la lista de los nombres de las imagenes
    for (int i=1; i<=20; i++) {
        // Se genera el nombre para cada imagen incluyendo su ruta
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

        imshow("Image L", imageL);
        imshow("Image R", imageR);

        // Retardo para desplegar las imagenes
        waitKey(100);
    }
    destroyAllWindows();

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
    Size sizeL = imageL.size();
    Size sizeR = imageR.size();
    camCalibratorL.calibrate(sizeL);
    camCalibratorR.calibrate(sizeR);

    //Imagen sin distorcion
    //image = imread("PRUEBA01.png");
    //imshow("Imagen Original", image);
    //Mat uImage= cameraCalibrator.remap(image);

    // Mostrar la Matriz de la camara
    Mat cameraMatrixL = camCalibratorL.getCameraMatrix();
    Mat cameraMatrixR = camCalibratorL.getCameraMatrix();

    ofstream archivo("conf.dat");
    archivo << "Left Camera intrinsic: " << cameraMatrixL.rows << "x" << cameraMatrixL.cols << endl;
    archivo << cameraMatrixL.at<double>(0,0) << "\t\t" << cameraMatrixL.at<double>(0,1) << "\t\t" << cameraMatrixL.at<double>(0,2) << endl;
    archivo << cameraMatrixL.at<double>(1,0) << "\t\t" << cameraMatrixL.at<double>(1,1) << "\t\t" << cameraMatrixL.at<double>(1,2) << endl;
    archivo << cameraMatrixL.at<double>(2,0) << "\t\t" << cameraMatrixL.at<double>(2,1) << "\t\t" << cameraMatrixL.at<double>(2,2) << endl;
    archivo << "Right Camera intrinsic: " << cameraMatrixL.rows << "x" << cameraMatrixL.cols << endl;
    archivo << cameraMatrixR.at<double>(0,0) << "\t\t" << cameraMatrixR.at<double>(0,1) << "\t\t" << cameraMatrixR.at<double>(0,2) << endl;
    archivo << cameraMatrixR.at<double>(1,0) << "\t\t" << cameraMatrixR.at<double>(1,1) << "\t\t" << cameraMatrixR.at<double>(1,2) << endl;
    archivo << cameraMatrixR.at<double>(2,0) << "\t\t" << cameraMatrixR.at<double>(2,1) << "\t\t" << cameraMatrixR.at<double>(2,2) << endl;
    archivo.close();
/*
     cout << " Camera intrinsic: " << cameraMatrix.rows << "x" << cameraMatrix.cols << endl;
     cout << cameraMatrix.at<double>(0,0) << "\t\t" << cameraMatrix.at<double>(0,1) << "\t\t" << cameraMatrix.at<double>(0,2) << endl;
     cout << cameraMatrix.at<double>(1,0) << "\t\t" << cameraMatrix.at<double>(1,1) << "\t\t" << cameraMatrix.at<double>(1,2) << endl;
     cout << cameraMatrix.at<double>(2,0) << "\t\t" << cameraMatrix.at<double>(2,1) << "\t\t" << cameraMatrix.at<double>(2,2) << endl;

     imshow("Imagen ajustada, sin distorcion", uImage);
     cvWaitKey();
*/
}

void getDataOfFile(ifstream *file){
    char line;
    while(file->good()){
        line = file->get();
        cout << line;
    }
}

