#include <iostream>
#include <iomanip>

#include "camera.h"

using namespace std;

int main(){
    int keypress = 0;
    Cam Camera;

    // Change the resolutions
    Camera.Devices[0].resolution_active = Camera.Devices[0].resolutions[8];
    Camera.Devices[1].resolution_active = Camera.Devices[1].resolutions[8];
    Camera.Devices[2].resolution_active = Camera.Devices[2].resolutions[2];

    // Activating all the devices
    // Camera.changeStatus();
    // Activating the devices 0 and 1
    Camera.changeStatus(0);
    Camera.changeStatus(1);

    // Show all the devices information
    Camera.printDevicesInfo();

    int imgCont = 1;
    while ( 1 ) {
        Camera.streamImage();

        for(int i=0; i<Camera.get_devices_number(); i++){
            if( Camera.Devices[i].capturing ){
                // Show images dynamically
                stringstream str;
                str << "Cam: " << i;
                cv::imshow(str.str().c_str(), Camera.Devices[i].image_buffer);
            }
        }

        // Eliminate High bits in keypress with AND operator
        keypress = cvWaitKey(1) & 255;

        // If the press key Esc...
        if( keypress == 27 ){
            break;
        }

        // If the press key Enter...
        if( keypress == 13 ){
            for(int i=0; i<Camera.get_devices_number(); i++){
                if( Camera.Devices[i].capturing ){
                    // Save images dynamically
                    stringstream str;
                    str << "../../resources/images/C" << setw(2) << setfill('0') << i <<
                           "-" << setw(2) << setfill('0') << imgCont << ".jpg";
                    cv::imwrite( str.str().c_str(), Camera.Devices[i].image_buffer);
                }
            }
            imgCont++;
        }
    }

    // std::cout << "Press any key to finish.." << std::endl;
    // cv::waitKey();
    cvDestroyAllWindows();
}

