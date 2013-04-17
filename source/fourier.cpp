#include "fourier.h"

Fourier::Fourier()
{
}


bool Fourier::FourierDFT(int mode, bool show){
    if(image_in.empty()){
        return false;
    }

    // 1. Expand input image to optimal size
    cv::Mat padded;

    // Get the optimal size to DFT
    int m = cv::getOptimalDFTSize( image_in.rows );
    int n = cv::getOptimalDFTSize( image_in.cols );

    // Add zero values to the border
    cv::copyMakeBorder(image_in,                // Input
                       padded,                  // Output
                       0,                       // Top
                       m - image_in.rows,       // Bottom
                       0,                       // Left
                       n - image_in.cols,       // Right
                       cv::BORDER_CONSTANT,     // Border type
                       cv::Scalar::all(0));     // Value

    // 2. Make place for both the complex and the real values.
    cv::Mat planes[] = { cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F) };
    cv::Mat complexI;

    // Add the expanded image and a plane with zeros a complexI
    cv::merge(planes, 2, complexI);

    // 3. Discrete Fourier transform
    cv::dft(complexI, complexI);

    // 4. Compute the magnitude -> sqrt(Re(DFT(I))^2 + Im(DFT(I))^2)
    cv::split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
    cv::Mat magI = planes[0];

    // 5. Switch to logarithmic scale -> log(1 + Magnitude)
    magI += cv::Scalar::all(1);
    cv::log(magI, magI);


    if(mode == 0){
        // Normalize image without Rearrange
        cv::normalize(magI, image_out, 0, 1, CV_MINMAX);
    }
    else{
        // 6. Crop the spectrum, if it has an odd number of rows or columns
        magI = magI(cv::Rect(0, 0, magI.cols & -2, magI.rows & -2));

        // Rearrange the quadrants of Fourier image  so that the origin is at the image center
        int cx = magI.cols/2;
        int cy = magI.rows/2;

        cv::Mat q0(magI, cv::Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
        cv::Mat q1(magI, cv::Rect(cx, 0, cx, cy));  // Top-Right
        cv::Mat q2(magI, cv::Rect(0, cy, cx, cy));  // Bottom-Left
        cv::Mat q3(magI, cv::Rect(cx, cy, cx, cy)); // Bottom-Right

        cv::Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
        q0.copyTo(tmp);
        q3.copyTo(q0);
        tmp.copyTo(q3);

        q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
        q2.copyTo(q1);
        tmp.copyTo(q2);

        // 7. Normalize: Transform the matrix with float values into a viewable image form (float between values 0 and 1).
        cv::normalize(magI, image_out, 0, 1, CV_MINMAX);
    }

    // 8. Show result
    if(show){
        imshow("Input Image", image_in);
        imshow("spectrum magnitude", image_out);
        cv::waitKey();
    }

    return true;
}


// ================================================================================================

void Fourier::set_image_in(cv::Mat image){
    image.copyTo(image_in);
}

void Fourier::set_image_out(cv::Mat image){
    image.copyTo(image_out);
}

cv::Mat Fourier::get_image_in(){
    return image_in;
}

cv::Mat Fourier::get_image_out(){
    return image_out;
}
