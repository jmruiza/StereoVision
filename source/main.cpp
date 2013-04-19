#include <iostream>
#include <iomanip>

#include "camera.h"
#include "stereovision.h"
#include "fourier.h"

using namespace std;
using namespace cv;

int main(){
    Fourier fourier;

    Mat imageLeft = imread("../../resources/images/face01-Left.jpg", 0);
    Mat imageRight = imread("../../resources/images/face01-Right.jpg", 0);

    // Processing Left Image
    fourier.set_image_in(imageLeft);
    // Fourier DFT
    fourier.FourierDFT(true);
    Mat imageLeft_Fourier = fourier.get_image_out();
    // Fourier Convolution
    //fourier.FourierConvolution(false);
    //Mat imageLeft_FourierConv = fourier.get_image_out();


    // Processing Right Image (Fourier DFT)
    fourier.set_image_in(imageRight);
    // Fourier DFT
    fourier.FourierDFT(false);
    Mat imageRight_Fourier = fourier.get_image_out();
    // Fourier Convolution
    //fourier.FourierConvolution(false);
    //Mat imageRight_FourierConv = fourier.get_image_out();



    // Display images
    //imshow("Image Left Fourier DFT", imageLeft_Fourier);
    //imshow("Image Right Fourier DFT", imageRight_Fourier);

    //imshow("Image Left Fourier Convolution", imageLeft_FourierConv);
    //imshow("Image Right Fourier Convolution", imageRight_FourierConv);
    waitKey();

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
    cvDestroyAllWindows();
}


