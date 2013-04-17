/*
    Creator: Juan Manuel Ruiz
    Date: January 15th 2013

    This class applies the PDI class examples
    Centro de Investigaciones en Optica A.C. CIO
*/

#ifndef PDI_H
#define PDI_H

#include <math.h>
#include <iostream>
#include <string>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

class PDI{

public:
    /* Constructor */
    PDI();
    ~PDI();
    bool isRGB;

    struct histograms{
        int Hist[256];
        int HistA[256];
    };

    std::vector <histograms> vHistograms;

    void mosaicEcualization(cv::Mat &img, int pixels, bool interpolate = false);

    /**
    Determines whether an image is RGB
    @param[in] image
    @param[out] true if an RGB image, false otherwise
    @author Juan Manuel Ruiz
    */
    bool imageRGB(const cv::Mat &img);

    /** Ecualize an Image
    @param[in] image
    @param[out] Ecualized Image
    @author Juan Manuel Ruiz
    */
    void ecualizeImage(cv::Mat &img);
    void initHistograms(int *hist, int *histA, int dims=256);
    void getHistograms(const cv::Mat &img, int *hist, int *histA, int dims=256);
    void normalizateArray(int *hist, int dims=256);
    void showHistogram(std::string str, int *hist, int dims=256);
    int getMaxValueArray(int *hist, int dims=256);
    int getMinValueArray(int *hist, int dims=256);
    void printArray(int *hist, int dims=256);
    void printArray(double *hist, int dims=256);

    /** Convolution */
    void gaussianConvolution(int sigma, cv::Mat &image, bool separated_kernel=false);
    void gaussianArrays(int sigma, double *h0, double *h1 );
    cv::Mat gaussianImage(int sigma);
    void getImageRange(const cv::Mat &image, int &min, int &max );
    void getImageRangeDouble(const cv::Mat &image, double &min, double &max);
    cv::Mat convertToDouble(const cv::Mat &image);
    cv::Mat convertToInterger(const cv::Mat &image);
    void normalizateMatrix(cv::Mat &img, int min, int max);

    void testConvolution();
    cv::Mat convolution(const cv::Mat &g, const cv::Mat &h);
    cv::Mat convolutionSeparatedKernel(const cv::Mat &g, const double *h0, int dim1, const double *h1, int dim2);

    /** Phase wrapping */
    void testPhase();

private:

};

#endif // PDI_H
