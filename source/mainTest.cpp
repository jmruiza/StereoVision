#include <iostream>
#include <string>

#include "camera.h"

using namespace std;

int main(){
    int keypress = 0;
    Cam Camera;

    // Activating the devices 0 and 1
    Camera.Devices[0].capturing = true;
    Camera.Devices[1].capturing = true;

    // Change the resolutions
    Camera.Devices[0].resolution_active = Camera.Devices[0].resolutions[0];
    Camera.Devices[1].resolution_active = Camera.Devices[1].resolutions[0];


    // Camera.printDevicesInfo();

    for(int i=0; i<Camera.get_devices_number(); i++){
        if( Camera.Devices[i].capturing ){
            // Create namedWindows dynamically
            stringstream str;
            str << i;
            cvNamedWindow(str.str().c_str());
            cv::imshow(str.str().c_str(), Camera.Devices[i].image_buffer);
        }
    }

    while ( 1 ) {
        Camera.streamImage();

        for(int i=0; i<Camera.get_devices_number(); i++){
            if( Camera.Devices[i].capturing ){
                // Create namedWindows dynamically
                stringstream str;
                str << i;
                cv::imshow(str.str().c_str(), Camera.Devices[i].image_buffer);
            }
        }

        // Eliminate High bits in keypress with AND operator
        keypress = cvWaitKey(1) & 255;

        // Until press Enter key...
        if( keypress == 13 || keypress == 27 ){
            break;
        }
    }

    // std::cout << "Press any key to finish.." << std::endl;
    // cv::waitKey();
    cvDestroyAllWindows();
}

