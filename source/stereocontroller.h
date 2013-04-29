#ifndef STEREOCONTROLLER_H
#define STEREOCONTROLLER_H

#include "fourier.h"

class StereoController : public StereoController
{
public:
    StereoController();

private:
    Fourier fourier;

    struct image_pair{
        cv::Mat Left;
        cv::Mat LeftFourier;
        cv::Mat Right;
        cv::Mat RightFourier;
    };
};

#endif // STEREOCONTROLLER_H
