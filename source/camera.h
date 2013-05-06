/*
    Creator: Juan Manuel Ruiz
    Date: October 6th 2012

    This class handles the attributes of the camera and capture method that returns an image.
*/

#ifndef CAM_H
#define CAM_H
#define N_RESOLUTIONS 30

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <iostream>


class Cam{

public:

    // Constructors and Destructor

    /**
    Constructor (single)
    @author Juan Manuel Ruiz
    */
    Cam();

    /**
    Destructor
    @author Juan Manuel Ruiz
    */
    ~Cam();

    // Attributes of devices (All detected devices)

    /** Struct of devices information */
    struct Device{
        int device_id;                          // ID (number) of device
        int resolutions_number;                 // Number of resolutions available for the device
        cv::Size resolutions[N_RESOLUTIONS];    // Resolutions available for the device
        bool capturing;                         // Flag to indicate if the device it is used in capture mode
        cv::Size resolution_active;             // Resolution used
        CvCapture* capture;                     // Capture
        cv::Mat image_buffer;                   // Image buffer captured
    };

     /** Vector of devices information */
    std::vector <Device> Devices;

    // Attributes for a specific device


    // Operative Method's    
    /** Print the information of devices available
    @author Juan Manuel Ruiz
    */
    void printDevicesInfo();

    /** Change the status flag capturing of a device
    @param[in] device to change status ( Wihtout param change all devices)
    @author Juan Manuel Ruiz
    */
    void changeStatus( int device=-1 );

    /** Get stream of images from the devices
    @author Juan Manuel Ruiz
    */
    void streamImage();

    // Getters and Setters
    /** Get the number of devices available
    @author Juan Manuel Ruiz
    */
    int get_devices_number();

private:

    // Attributes of devices (All detected devices)
    /** Number of devices available */
    int n_devices;

    /** Resolutions available */
    cv::Size common_resolutions[N_RESOLUTIONS];

    // Attributes for a specific device


    // Operative Method's
    /** Get the number of devices available
    @author Juan Manuel Ruiz
    */
    void getNumberDevices();

    /** Set the resolutions array with common values
    @author Juan Manuel Ruiz
    */
    void setCommonResolutions();

    /** Get the information of devices available
    @author Juan Manuel Ruiz
    */
    void getDevicesInfo();
};

#endif // CAM_H
