#include "camera.h"

Cam::Cam(){
    getNumberDevices();

    if( get_devices_number() > 0 ){
        setCommonResolutions();
        getDevicesInfo();
    }
}

Cam::~Cam(){
    Devices.erase(Devices.begin(), Devices.end());
    Devices.clear();
}

// Getters and Setters
int Cam::get_devices_number(){
    return n_devices;
}

// Operative Method's

// Stream of images from the devices
void Cam::streamImage(){
    int cont = 0;
    for(int i=0; i<get_devices_number(); i++){
        if( Devices[i].capturing ){
            /*
             * Opens the capture device.
             * If well use "CvCapture* capture = cvCaptureFromCAM( disp );" works fine,
             * it can't handles the camera params.
             * We prefer use:  cvCreateCameraCapture()
             */
            std::cout << "Device ID: " << Devices[i].device_id << std::endl;
           // CvCapture* capture = cvCreateCameraCapture( Devices[i].device_id );
            Devices[i].capture  = cvCreateCameraCapture( Devices[i].device_id );

            // Check Device
            // if ( !capture ) {
            if(!Devices[i].capture){
                /*
                fprintf( stderr, "ERROR: Unknown Capture Device  \n", device );
                getchar();
                */
                return;
            }
            /*
             * Set Resolution, For other camera manipulable parameters check documentation: cvSetCaptureProperty
             */
            // cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH,  Devices[i].resolution_active.x );
            // cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT,  Devices[i].resolution_active.y );
            cvSetCaptureProperty( Devices[i].capture, CV_CAP_PROP_FRAME_WIDTH,  Devices[i].resolution_active.x );
            cvSetCaptureProperty( Devices[i].capture, CV_CAP_PROP_FRAME_HEIGHT,  Devices[i].resolution_active.y );

            // Get frame
            // IplImage* frame = cvQueryFrame( capture );
            IplImage* frame = cvQueryFrame( Devices[i].capture );

            // Check frame
            if ( !frame ) {
                /*
                 * fprintf( stderr, "ERROR: frame is null...\n" );
                 * // Release capture device and memory
                 * cvDestroyAllWindows();
                 * cvReleaseCapture( &capture );
                 * //getchar();
                 */
                return;
            }

            cv::Mat tmp = frame;
            tmp.copyTo( Devices[i].image_buffer );

           // cvReleaseCapture( &capture );
           cvReleaseCapture( &Devices[i].capture );
        }
        else{
            cont++;
        }

        if( cont == get_devices_number() )
            break;
    }
}

// Determine the number of available devices
void Cam::getNumberDevices(){
    CvCapture *cap;
    n_devices = 0;

    while(1){
        cap = cvCreateCameraCapture(n_devices++);
        if (cap == NULL){
            cvReleaseCapture(&cap);
            break;
        }
        cvReleaseCapture(&cap);
    }

    cvReleaseCapture(&cap);
    n_devices--;
}

// Setting Values for common resolutions
void Cam::setCommonResolutions(){
    common_resolutions[0].x = 160;
    common_resolutions[0].y = 120;
    common_resolutions[1].x = 176;
    common_resolutions[1].y = 144;
    common_resolutions[2].x = 320;
    common_resolutions[2].y = 176;
    common_resolutions[3].x = 320;
    common_resolutions[3].y = 240;
    common_resolutions[4].x = 352;
    common_resolutions[4].y = 288;
    common_resolutions[5].x = 432;
    common_resolutions[5].y = 240;
    common_resolutions[6].x = 544;
    common_resolutions[6].y = 288;
    common_resolutions[7].x = 640;
    common_resolutions[7].y = 360;
    common_resolutions[8].x = 640;
    common_resolutions[8].y = 480;
    common_resolutions[9].x = 752;
    common_resolutions[9].y = 416;
    common_resolutions[10].x = 800;
    common_resolutions[10].y = 448;
    common_resolutions[11].x = 800;
    common_resolutions[11].y = 600;
    common_resolutions[12].x = 864;
    common_resolutions[12].y = 480;
    common_resolutions[13].x = 960;
    common_resolutions[13].y = 544;
    common_resolutions[14].x = 960;
    common_resolutions[14].y = 720;
    common_resolutions[15].x = 1024;
    common_resolutions[15].y = 576;
    common_resolutions[16].x = 1184;
    common_resolutions[16].y = 656;
    common_resolutions[17].x = 1280;
    common_resolutions[17].y = 720;
    common_resolutions[18].x = 1280;
    common_resolutions[18].y = 800;
    common_resolutions[19].x = 1280;
    common_resolutions[19].y = 960;
    common_resolutions[20].x = 1280;
    common_resolutions[20].y = 1024;
    common_resolutions[21].x = 1392;
    common_resolutions[21].y = 768;
    common_resolutions[22].x = 1504;
    common_resolutions[22].y = 832;
    common_resolutions[23].x = 1600;
    common_resolutions[23].y = 896;
    common_resolutions[24].x = 1600;
    common_resolutions[24].y = 1200;
    common_resolutions[25].x = 1712;
    common_resolutions[25].y = 960;
    common_resolutions[26].x = 1792;
    common_resolutions[26].y = 1008;
    common_resolutions[27].x = 1920;
    common_resolutions[27].y = 1080;
    common_resolutions[28].x = 2048;
    common_resolutions[28].y = 1536;
    common_resolutions[29].x = 2592;
    common_resolutions[29].y = 1944;
}

// Get the information of devices available
void Cam::getDevicesInfo(){
    Device dev;

    // Get the resolutions for each device
    for(int i=0; i<get_devices_number(); i++){
        dev.device_id = i;
        dev.resolutions_number = 0;
        CvCapture *cap = cvCreateCameraCapture(i);
        for(int j=0; j<N_RESOLUTIONS; j++){
            // Set actual values of resolution
            cvSetCaptureProperty( cap, CV_CAP_PROP_FRAME_WIDTH, common_resolutions[j].x );
            cvSetCaptureProperty( cap, CV_CAP_PROP_FRAME_HEIGHT, common_resolutions[j].y );

            // Compare the actual resolution value with the last accepted value (Width and Height)
            if( common_resolutions[j].x == cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_WIDTH)
                && common_resolutions[j].y == cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_HEIGHT) ){
                dev.resolutions[dev.resolutions_number].x = cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_WIDTH);
                dev.resolutions[dev.resolutions_number].y = cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_HEIGHT);
                dev.resolutions_number++;
            }
            // Initializing other attributes
            dev.capturing = false;
            dev.resolution_active.x = dev.resolutions[0].x;
            dev.resolution_active.y = dev.resolutions[0].y;
            dev.image_buffer = cv::Mat::zeros(dev.resolution_active.y, dev.resolution_active.x, CV_8U);
        }
        cvReleaseCapture(&cap);
        Devices.push_back(dev);
    }
}

// Print the information of devices available
void Cam::printDevicesInfo(){
    // Number of devices availabel
    std::cout << "Devices: " << get_devices_number() << std::endl;

    // Get the information for each device
    for(int i=0; i<get_devices_number(); i++){
        std::cout << "Device: " << i << std::endl;
        std::cout << " - Device ID: " << Devices[i].device_id << std::endl;
        std::cout << " - Device Resolutions: " << Devices[i].resolutions_number << std::endl;

        for(int j=0; j<Devices[i].resolutions_number; j++){
            std::cout << " -- [" << j+1 << "] " <<
                         Devices[i].resolutions[j].y << " x " <<
                         Devices[i].resolutions[j].x << std::endl;
        }

        std::cout << " - Capturing: " << Devices[i].capturing << std::endl;
        std::cout << " - Resolution Active: " << Devices[i].resolution_active.x << " x " <<
                     Devices[i].resolution_active.y << std::endl;
        std::cout << std::endl;
    }
}

