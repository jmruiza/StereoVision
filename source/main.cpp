#include <iostream>
#include <iomanip>

#include "camera.h"
#include "stereovision.h"

using namespace std;
using namespace cv;

int main(){
    StereoVision stereo;

    // Original Images
    /*
    cvNamedWindow("Left - Original");
    imshow("Left - Original", stereo.get_image_Left_original());
    cvNamedWindow("Right - Original");
    imshow("Right - Original", stereo.get_image_Right_original());

    // Processed Images
    cvNamedWindow("Left - Processed");
    imshow("Left - Processed", stereo.get_image_Left_processed());
    cvNamedWindow("Right - Processed");
    imshow("Right - Processed", stereo.get_image_Right_processed());
    */

    // Camera class intance
    Cam Camera;

    // Change the resolutions
    Camera.Devices[0].resolution_active = Camera.Devices[1].resolutions[8];
    Camera.Devices[1].resolution_active = Camera.Devices[2].resolutions[8];

    // Activating all the devices
    Camera.changeStatus(0);
    Camera.changeStatus(1);

    int keypress = 0;

    while ( 1 ) {
        Camera.streamImage();

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
    cvDestroyAllWindows();    
}


