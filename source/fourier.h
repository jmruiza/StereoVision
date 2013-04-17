#ifndef FOURIER_H
#define FOURIER_H

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

class Fourier
{
public:
    Fourier();

    bool FourierDFT(int mode=0, bool show=true);
    void set_image_in(cv::Mat image);
    void set_image_out(cv::Mat image);
    cv::Mat get_image_in();
    cv::Mat get_image_out();

private:
    cv::Mat image_in;
    cv::Mat image_out;
};

#endif // FOURIER_H
