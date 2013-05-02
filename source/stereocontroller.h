#ifndef STEREOCONTROLLER_H
#define STEREOCONTROLLER_H

#include "fourier.h"
#include "camera.h"
#include "stereovision.h"
#include "fourier.h"


class StereoController
{
public:
    StereoController();

    void demo_figure();
    void demo_cube();
    void demo_prism();

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
