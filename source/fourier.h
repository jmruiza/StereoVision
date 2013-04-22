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
    @param[in] display (true/false) the initial and final images
    @author Juan Manuel Ruiz
    */
    bool FourierDFT(bool display=false);

    /**
    Compute the Convolution Fourier Transform and show it
    @param[in] display (true/false) the initial and final images
    @author Juan Manuel Ruiz
    */
    bool FourierConvolution(bool display=false);

    /**
    Compute the Inverse Fourier Transform and show it
    @param[in] Display (true/false) the initial and final images
    @author Juan Manuel Ruiz
    */
    cv::Mat FourierInverse(cv::Mat image, bool display=false);

    /**
    Rearrange the image
    @param[in] Display (true/false) the initial and final images
    @author Juan Manuel Ruiz
    */
    cv::Mat FFTShift(cv::Mat image, bool display=false);

    /**
    Controller to mouse events
    @param[in] Name of Window
    @author Juan Manuel Ruiz
    */
    void ControlEventMouse( char* nameOfWindow );

    /**
    Draw a square in image
    @param[in] x coordinate
    @param[in] y coordinate
    @param[in] Size of square
    @param[cv::Mat] image
    @author Juan Manuel Ruiz
    */
    void DrawSquare( int x, int y, int size, cv::Mat image);

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
