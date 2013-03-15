#include "camera.h"

Cam::Cam(){
    getNumberDevices();

    if( get_devices_number() > 0 ){
        setCommonResolutions();
        getDevicesInfo();
        printDevicesInfo();
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
            cvSetCaptureProperty( Devices[i].capture, CV_CAP_PROP_FRAME_WIDTH,  Devices[i].resolution_active.width );
            cvSetCaptureProperty( Devices[i].capture, CV_CAP_PROP_FRAME_HEIGHT,  Devices[i].resolution_active.height );

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
    common_resolutions[0] = cv::Size(160, 120);
    common_resolutions[1] = cv::Size(176, 144);
    common_resolutions[2] = cv::Size(320, 176);
    common_resolutions[3] = cv::Size(320, 240);
    common_resolutions[4] = cv::Size(352, 288);
    common_resolutions[5] = cv::Size(432, 240);
    common_resolutions[6] = cv::Size(544, 288);
    common_resolutions[7] = cv::Size(640, 360);
    common_resolutions[8] = cv::Size(640, 480);
    common_resolutions[9] = cv::Size(752, 416);
    common_resolutions[10] = cv::Size(800, 448);
    common_resolutions[11] = cv::Size(800, 600);
    common_resolutions[12] = cv::Size(864, 480);
    common_resolutions[13] = cv::Size(960, 544);
    common_resolutions[14] = cv::Size(960, 720);
    common_resolutions[15] = cv::Size(1024, 576);
    common_resolutions[16] = cv::Size(1184, 656);
    common_resolutions[17] = cv::Size(1280, 720);
    common_resolutions[18] = cv::Size(1280, 800);
    common_resolutions[19] = cv::Size(1280, 960);
    common_resolutions[20] = cv::Size(1280, 1024);
    common_resolutions[21] = cv::Size(1392, 768);
    common_resolutions[22] = cv::Size(1504, 832);
    common_resolutions[23] = cv::Size(1600, 896);
    common_resolutions[24] = cv::Size(1600, 1200);
    common_resolutions[25] = cv::Size(1712, 960);
    common_resolutions[26] = cv::Size(1792, 1008);
    common_resolutions[27] = cv::Size(1920, 1080);
    common_resolutions[28] = cv::Size(2048, 1536);
    common_resolutions[29] = cv::Size(2592, 1944);
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
            cvSetCaptureProperty( cap, CV_CAP_PROP_FRAME_WIDTH, common_resolutions[j].width );
            cvSetCaptureProperty( cap, CV_CAP_PROP_FRAME_HEIGHT, common_resolutions[j].height );

            // Compare the actual resolution value with the last accepted value (Width and Height)
            if( common_resolutions[j].width == cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_WIDTH)
                && common_resolutions[j].height == cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_HEIGHT) ){
                dev.resolutions[dev.resolutions_number].width = cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_WIDTH);
                dev.resolutions[dev.resolutions_number].height = cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_HEIGHT);
                dev.resolutions_number++;
            }
            // Initializing other attributes
            dev.capturing = false;
            dev.resolution_active = dev.resolutions[0];
            dev.image_buffer = cv::Mat::zeros( dev.resolution_active, CV_8U );
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
                         Devices[i].resolutions[j].width << " x " <<
                         Devices[i].resolutions[j].height << std::endl;
        }

        std::cout << " - Capturing: " << Devices[i].capturing << std::endl;
        std::cout << " - Resolution Active: " << Devices[i].resolution_active.width << " x " <<
                     Devices[i].resolution_active.height << std::endl;
        std::cout << std::endl;
    }
}

