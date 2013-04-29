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
    @param[out] Vector of images with Fourier Transform
    @author Juan Manuel Ruiz
   */
    std::vector<cv::Mat_<float> > FourierDFT(const cv::Mat image_src);

    /**
    Compute the Inverse Fourier Transform
    @param[in] Vector of images with Fourier Transform
    @param[out] Vector of images with Inverse Fourier Transform
    @author Juan Manuel Ruiz
    */
    std::vector<cv::Mat_<float> > FourierDFTInverse(std::vector<cv::Mat_<float> > input);

    /**
    Get the magnitude image
    @param[in] Vector of images with Fourier Transform
    @param[out] magnitude image of spectrum
    @author Juan Manuel Ruiz
    */
    cv::Mat getImageMagnitude(std::vector<cv::Mat_<float> > input);

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
    @param[in] Vector of images with Fourier Transform
    @param[out] Vector of images with Rearrange images Fourier's Transform
    @author Juan Manuel Ruiz
    */
    void FFTShift(std::vector<cv::Mat_<float> > &input);

    /**
    Rearrange the quadrants of Fourier image  so that the origin is at the image center
    @param[in] image source
    @param[out] Rearrange image source
    @author Juan Manuel Ruiz
    */
    void FFTShift(cv::Mat& image_dst);

    /**
    LobeFilter
    @param[in] Vector of images with Fourier Transform
    @param[in] Mask type:
               0 - Rectangular
               1 - Circular
               2 - Gaussian
    @param[in] Size of sides(Rectangle) or radius(circle/Gaussian)
    @param[out] Vector of images with lobe filtered
    @author Juan Manuel Ruiz
     */
    void LobeFilter(std::vector<cv::Mat_<float> > &input, int mask_type, int mask_size);

    cv::Mat UnwrappedPhase(std::vector<cv::Mat_<float> >& input);

    cv::Mat PhaseDiference(cv::Mat_<float> phase1, cv::Mat_<float> phase2);

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
