#ifndef FOURIER_H
#define FOURIER_H

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

class Fourier{

public:     
    /**
    Constructor (single)
    @author Juan Manuel Ruiz
    */
    Fourier();

    /**
    Compute the Discrete Fourier Transform
    @param[in] image source
    @param[in] image destination
    @param[out] Fourier Transform of image source
    @author Juan Manuel Ruiz
   */
    void FourierDFT(const cv::Mat image_src, cv::Mat& image_dst);

    /**
    Compute the Inverse Fourier Transform
    @param[in] image source
    @param[in] image destination
    @param[out] Inverse Fourier Transform of image source
    @author Juan Manuel Ruiz
    */
    void FourierDFTInverse(const cv::Mat image_src, cv::Mat& image_dst);

    /**
    Compute the Convolution Fourier Transform
    @param[in] image source
    @param[in] image destination
    @param[out] Fourier Transform of image source
    @author Juan Manuel Ruiz
    */
    void FourierConvolution(const cv::Mat image_src, cv::Mat& image_dst);

    /**
    Rearrange the quadrants of Fourier image  so that the origin is at the image center
    @param[in] image source
    @param[in] image destination
    @param[out] Rearrange image source
    @author Juan Manuel Ruiz
    */
    void FFTShift(const cv::Mat image_src, cv::Mat& image_dst);

    /**
    LobeFilter
    @param[in] image source
    @param[in] Mask type:
               0 - Rectangular
               1 - Circular
               2 - Gaussian
    @param[in] Size of sides(Rectangle) or radius(circle/Gaussian)
    @param[out] Image with lobe filtered
    @author Juan Manuel Ruiz
     */
    cv::Mat LobeFilter( cv::Mat image, int mask_type, int mask_size);

    /**
    Draw a square in image
    @param[in] x coordinate of center of square
    @param[in] y coordinate of center of square
    @param[in] Size of square
    @param[out] image
    @author Juan Manuel Ruiz
    */
    cv::Mat DrawSquare( int x, int y, int size, cv::Mat image);

    cv::Mat DrawCircle( int xc, int yc, int size, cv::Mat image);

    /**
    Generate a mask
    @param[in] Mask type:
               0 - Rectangular
               1 - Circular
               2 - Gaussian
    @param[in] Size of sides(Rectangle) or radius(circle/Gaussian)
    @param[in] x coordinate of center point
    @param[in] y coordinate of center point
    @param[out] Mask image
    @author Juan Manuel Ruiz
    */
    void GenerateMask(int mask_type, int size, int xc, int yc, cv::Mat& mask);

    /**
    Validate the data to create the mask and show the lobe selector
    @param[in] x coordinate of center point
    @param[in] y coordinate of center point
    @param[in] x coordinate of superior corner
    @param[in] y coordinate of superior corner
    @param[in] Size of sides(Rectangle) or radius(circle/Gaussian)
    @param[in] Mask image
    @author Juan Manuel Ruiz
    */
    void validateDimensions(int xc, int yc, int& x, int& y, int size, const cv::Mat image);
private:

};

#endif // FOURIER_H
