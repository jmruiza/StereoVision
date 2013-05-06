#include "stereocalibrator.h"

StereoCalibrator::StereoCalibrator()
{
    char nameFile[] = "conf.xml";
    cv::FileStorage file;
    if(file.open(nameFile, file.READ)){
        std::cout << "Read data from: \"" << nameFile << "\"" << std::endl;
        getDataFromFile(file);
    }
    else{
        file.release();
        std::cout << "Calibrar: " << std::endl;
        //CaptureFormCameras();
        CalibrateCameras(nameFile);
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
}

void StereoCalibrator::CaptureFormCameras(){
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

void StereoCalibrator::CalibrateCameras(char nameFile[]){
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

        // Se agrega el nombre al vector
        filelistL.push_back(strLeft.str());
        filelistR.push_back(strRight.str());

        // Se muestra cada imagen
        imageL = cv::imread(strLeft.str(), 0);
        imageR = cv::imread(strRight.str(), 0);

        cv::imshow("Left Camera", imageL);
        cv::imshow("Right Camera", imageR);

        // Retardo para desplegar las imagenes
        cv::waitKey(100);
    }
    cv::destroyAllWindows();

    // Se crea un objeto de la clase CameraCalibrator
    CameraCalibrator camCalibratorL;
    CameraCalibrator camCalibratorR;

    // Se especifica el numero de esquinas verticales y horizontales internas
    cv::Size boardSize(8,6);

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

    cv::FileStorage file(nameFile, cv::FileStorage::WRITE);
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

void StereoCalibrator::getDataFromFile(cv::FileStorage &file){
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

