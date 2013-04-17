#ifndef FOURIER_H
#define FOURIER_H

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

class Fourier
{
public:

    /**
    Constructor (single)
    @author Juan Manuel Ruiz
    */
    Fourier();

    /**
    Compute the Discrete Fourier Transform and show it
    @param[in] Mode (0-1) crop and rearrange the image
    @param[in] Show (true/false) the initial and final images
    @author Juan Manuel Ruiz
    */
    bool FourierDFT(int mode=0, bool show=true);

    /**
    Compute the Convolution Fourier Transform and show it
    @param[in] Mode (0-1) crop and rearrange the image
    @param[in] Show (true/false) the initial and final images
    @author Juan Manuel Ruiz
    */
    bool FourierConvolution(int mode=0, bool show=true);

    // Getters and Setters
    void set_image_in(cv::Mat image);
    void set_image_out(cv::Mat image);
    cv::Mat get_image_in();
    cv::Mat get_image_out();

private:
    cv::Mat image_in;
    cv::Mat image_out;
};

#endif // FOURIER_H
