#include <iostream>
#include <iomanip>

#include "camera.h"
#include "stereovision.h"

using namespace std;
using namespace cv;

int main(){
    StereoVision stereo;
    stereo.set_image_Left(imread("C02-01.jpg"));
    stereo.set_image_Right(imread("C01-01.jpg"));

    stereo.processImages();

    // Original Images
    /*
    cvNamedWindow("Left - Original");
    imshow("Left - Original", stereo.get_image_Left_original());
    cvNamedWindow("Right - Original");
    imshow("Right - Original", stereo.get_image_Right_original());

    // Processed Images
    cvNamedWindow("Left - Processed");
    imshow("Left - Processed", stereo.get_image_Left_processed());
    cvNamedWindow("Right - Processed");
    imshow("Right - Processed", stereo.get_image_Right_processed());
    */

    cvNamedWindow("ImageMatches");
    imshow("ImageMatches", stereo.get_image_Matches());

    std::cout << "Press any key to finish.." << std::endl;
    cv::waitKey();
    cvDestroyAllWindows();    
}


