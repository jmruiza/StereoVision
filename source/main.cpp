#include <iostream>
#include <iomanip>

#include "camera.h"
#include "stereovision.h"

using namespace std;
using namespace cv;

int main(){
    // StereoVision class instance
    StereoVision Stereo;

    // Camera class intance
    Cam Camera;

    // Change the resolutions
    Camera.Devices[0].resolution_active = Camera.Devices[0].resolutions[8];
    Camera.Devices[1].resolution_active = Camera.Devices[1].resolutions[8];

    // Activate the devices
    Camera.changeStatus(0);
    Camera.changeStatus(1);

    // Set the number of images to process
    Stereo.set_number_of_images(7);

    // Generate and show fringes pattern
    Stereo.generate_image_Fringes();

    // Initizalizate the image stream
    Camera.streamImage();

    // Asign the values
    Stereo.set_image_Left(Camera.Devices[0].image_buffer);
    Stereo.set_image_Right(Camera.Devices[1].image_buffer);

    // Show the images
    cvNamedWindow("Left Image");
    imshow("Left Image", Stereo.get_image_Left_processed());

    cvNamedWindow("Right Image");
    imshow("Right Image", Stereo.get_image_Right_processed());

    cv::waitKey();

    for(int i=1; i<=Stereo.get_number_of_images(); i++){
        // Initizalizate the image stream
        Camera.streamImage();

        // Generate and show fringes pattern
        Stereo.generate_image_Fringes(i);

        // Asign the values
        Stereo.set_image_Left(Camera.Devices[0].image_buffer);
        Stereo.set_image_Right(Camera.Devices[1].image_buffer);

        stringstream str;
        str << "../../resources/images/CamLeft" << setw(2) << setfill('0') << i << ".jpg";
        cv::imwrite( str.str().c_str(), Stereo.get_image_Left_original());

        str << "../../resources/images/CamRightt" << setw(2) << setfill('0') << i << ".jpg";
        cv::imwrite( str.str().c_str(), Stereo.get_image_Right_original());

        // Save images
        for(int i=0; i<Camera.get_devices_number(); i++){
            if( Camera.Devices[i].capturing ){
                // Save images dynamically
                stringstream str;
                str << "../../resources/images/Cam" << setw(2) << setfill('0') << i <<
                       "-" << setw(2) << setfill('0') << i << ".jpg";
                cv::imwrite( str.str().c_str(), Camera.Devices[i].image_buffer);
            }
        }

        // Show the images
        imshow("Left Image", Stereo.get_image_Left_processed());
        imshow("Right Image", Stereo.get_image_Right_processed());

        cv::waitKey(1000);
    }

    // Desactivate the devices
    Camera.changeStatus(0);
    Camera.changeStatus(1);
    Camera.streamImage();

/*
    // Infinite Loop to image capture
    // Escape key: Stop Loop
    // Enter key: Save image


    while ( 1 ) {
        //Camera.streamImage();
        stereo.set_image_Left(Camera.Devices[0].image_buffer);
        stereo.set_image_Right(Camera.Devices[1].image_buffer);

        stereo.processImages();

        cvNamedWindow("ImageMatches");
        imshow("ImageMatches", stereo.get_image_Matches());

        // Eliminate High bits in keypress with AND operator
        keypress = cvWaitKey(1) & 255;

        // If the press key Esc...
        if( keypress == 27 ){
            break;
        }
    }
*/
    cv::waitKey();
    cvDestroyAllWindows();
}


