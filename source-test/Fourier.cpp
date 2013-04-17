#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace  cv;

int main(int argc, char ** argv)
{


    Mat I = imread("../../resources/images/cube01-Left.jpg", CV_LOAD_IMAGE_GRAYSCALE);

    if( I.empty())
        return -1;

    // 1. Expand input image to optimal size
    Mat padded;

    // Get the optimal size to DFT
    int m = getOptimalDFTSize( I.rows );
    int n = getOptimalDFTSize( I.cols );

    // Add zero values to the border
    copyMakeBorder(I,               // Input
                   padded,          // Output
                   0,               // Top
                   m - I.rows,      // Bottom
                   0,               // Left
                   n - I.cols,      // Right
                   BORDER_CONSTANT, // Border type
                   Scalar::all(0)); // Value

    // 2. Make place for both the complex and the real values.
    Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
    Mat complexI;

    // Add the expanded image and a plane with zeros a complexI
    merge(planes, 2, complexI);

    // 3. Discrete Fourier transform
    dft(complexI, complexI);

    // 4. Compute the magnitude -> sqrt(Re(DFT(I))^2 + Im(DFT(I))^2)
    split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
    Mat magI = planes[0];

    // 5. Switch to logarithmic scale -> log(1 + Magnitude)
    magI += Scalar::all(1);
    log(magI, magI);

    // Normalize image without Rearrange
    Mat magI1;
    normalize(magI, magI1, 0, 1, CV_MINMAX);

    // 6. Crop the spectrum, if it has an odd number of rows or columns
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    // Rearrange the quadrants of Fourier image  so that the origin is at the image center
    int cx = magI.cols/2;
    int cy = magI.rows/2;

    Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

    Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);

    // 7. Normalize: Transform the matrix with float values into a viewable image form (float between values 0 and 1).
    normalize(magI, magI, 0, 1, CV_MINMAX);

    // 8. Show result
    imshow("Input Image", I);
    imshow("spectrum magnitude", magI);
    imshow("spectrum magnitude 1", magI1);
    waitKey();

    return 0;
}
