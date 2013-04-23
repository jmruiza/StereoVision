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
     LobeFilter
     @param image
     @param mask type
     */
    cv::Mat LobeFilter( cv::Mat image, int mask_type);

    /**
    Draw a square in image
    @param[in] x coordinate of center of square
    @param[in] y coordinate of center of square
    @param[in] Size of square
    @param[cv::Mat] image
    @author Juan Manuel Ruiz
    */
    cv::Mat DrawSquare( int x, int y, int size, cv::Mat image);

    /**
    Draw a square in image
    @param[cv::Point] coordinate of center of square
    @param[in] Size of square
    @param[cv::Mat] image
    @author Juan Manuel Ruiz
    */
    cv::Mat DrawSquare( cv::Point sel_point, int size, cv::Mat image);

    void GenerateMask(int type, int size, cv::Point sel_point, cv::Mat& mask);
    void GenerateMask(int type, int size, int xp, int yp, cv::Mat& mask);


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
