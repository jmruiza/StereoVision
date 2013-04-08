#include <iostream>
#include <iomanip>

#include "camera.h"
#include "stereovision.h"

using namespace std;
using namespace cv;

int main(){
    // StereoVision class instance
    StereoVision Stereo;

    // Generate and show fringes pattern
    Stereo.generate_image_Fringes();

/*
    // Camera class intance
    Cam Camera;

    // Change the resolutions
    Camera.Devices[0].resolution_active = Camera.Devices[0].resolutions[8];
    Camera.Devices[1].resolution_active = Camera.Devices[1].resolutions[8];

    // Activating all the devices
    Camera.changeStatus(0);
    Camera.changeStatus(1);

    int keypress = 0;

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


