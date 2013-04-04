#include <iostream>
#include <iomanip>

#include "camera.h"
#include "stereovision.h"

using namespace std;
using namespace cv;

int main(){
    StereoVision stereo;
    stereo.set_image_izq(imread("C02-01.jpg", 0));
    stereo.set_image_der(imread("C01-01.jpg", 0));
    stereo.processImages();

    // Original Images
    cvNamedWindow("1 - Original");
    imshow("1 - Original", stereo.get_image_izq_original());
    cvNamedWindow("2 - Original");
    imshow("2 - Original", stereo.get_image_der_original());

    // Processed Images
    cvNamedWindow("1 - Processed");
    imshow("1 - Processed", stereo.get_image_izq_processed());
    cvNamedWindow("2 - Processed");
    imshow("2 - Processed", stereo.get_image_der_processed());

    std::cout << "Press any key to finish.." << std::endl;
    cv::waitKey();
    cvDestroyAllWindows();    
}


