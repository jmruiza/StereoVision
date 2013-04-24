#include "fourier.h"
#include "pdi.h"

//void mouseEvent(int evt, int x, int y, int flags, void* param);

struct Mouse{
    int event;
    int x;
    int y;
};

// Control mouse events
void mouseEvent(int evt, int x, int y, int flags, void* param){
//    cv::Point* sel_point = (cv::Point*) param;

    Mouse* mouse = (Mouse*) param;
    mouse->event = evt;
    mouse->x = x;
    mouse->y = y;
/*
    // Push Down Left Button
    if(evt==CV_EVENT_LBUTTONDOWN){
        sel_point->x = x;
        sel_point->y = y;
    }

    // CV_EVENT_MOUSEMOVE
    if(evt==CV_EVENT_MOUSEMOVE){
        sel_point->x = x;
        sel_point->y = y;
    }
*/

}

Fourier::Fourier()
{
}

bool Fourier::FourierDFT(bool display){
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

    // Crop the spectrum, if it has an odd number of rows or columns
    magI = magI(cv::Rect(0, 0, magI.cols & -2, magI.rows & -2));

    // Rearrage the spectrum to center the origin
    magI = FFTShift(magI);

    // Normalize the spectrum
    cv::normalize(magI, image_out, 0, 1, CV_MINMAX);

    // Show result
    if(display){
        imshow("Input Image", image_in);
        imshow("spectrum magnitude", image_out);
        cv::waitKey();
    }


    return true;
}

bool Fourier::FourierConvolution(bool display){
    if(image_in.empty()){
        return false;
    }

    PDI pdi;
    int sigma = 5;

    // Generating Kernel
    double h0[6*sigma];
    double h1[6*sigma];
    pdi.gaussianArrays(sigma, h0, h1);

    // Convert image_in to double
    cv::Mat g = pdi.convertToDouble(image_in);

    // Normalizate convolution
    cv::Mat i = cv::Mat::ones(image_in.rows, image_in.cols, CV_64F);
    cv::Mat eta = pdi.convolutionSeparatedKernel(i, h0, 6*sigma, h1, 6*sigma);
    cv::Mat f = cv::Mat::zeros(image_in.rows, image_in.cols, CV_64F);

    for(int y=0; y<image_in.rows; y++){
        for(int x=0; x<image_in.cols; x++){
            f.at<double>(y,x) = g.at<double>(y,x) / eta.at<double>(y,x);
        }
    }

    // Normalize image
    cv::normalize(f, image_out, 0, 1, CV_MINMAX);

    // Show result
    if(display){
        imshow("Input Image", image_in);
        imshow("Output Image", image_out);
        cv::waitKey();
    }

    return true;
}

cv::Mat Fourier::FFTShift(cv::Mat image, bool display){
    cv::Mat out;
    image.copyTo(out);

    // Rearrange the quadrants of Fourier image  so that the origin is at the image center
    int cx = out.cols/2;
    int cy = out.rows/2;

    // Create a ROI per quadrant
    cv::Mat q0(out, cv::Rect(0, 0, cx, cy));   // Top-Left
    cv::Mat q1(out, cv::Rect(cx, 0, cx, cy));  // Top-Right
    cv::Mat q2(out, cv::Rect(0, cy, cx, cy));  // Bottom-Left
    cv::Mat q3(out, cv::Rect(cx, cy, cx, cy)); // Bottom-Right

    // swap quadrants (Top-Left with Bottom-Right)
    cv::Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    // swap quadrant (Top-Right with Bottom-Left)
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);

    if( display ){
        cv::Mat im_in;
        cv::Mat im_out;
        //  Normalize to display
        cv::normalize(image, im_in, 0, 1, CV_MINMAX);
        cv::normalize(out, im_out, 0, 1, CV_MINMAX);
        // Display images
        cv::imshow("Image In", image);
        cv::imshow("Image Out", out);
    }

    return out;
}

cv::Mat Fourier::LobeFilter(cv::Mat image, int mask_type){
    cv::Mat image_copy;
    image.copyTo(image_copy);

    // Mouse control Struct
    Mouse mouse;
    //cv::Point ret_point;

    // Window to selection
    cv::namedWindow("Select a Lobe");

    // Event Mouse controller
    //cvSetMouseCallback("Select a Lobe", mouseEvent, (void*)(&ret_point));
    cvSetMouseCallback("Select a Lobe", mouseEvent, (void*)(&mouse));

    // Until dont press key Enter
    //while(cv::waitKey(100) != 13){
    while(mouse.event != CV_EVENT_LBUTTONDOWN){
        // Show selected area
        //image_copy = DrawSquare(ret_point, 200, image);
        image_copy = DrawSquare(mouse.x, mouse.y, 200, image);
        cv::imshow("Select a Lobe", image_copy);
    }
    // Destroy the window "Select a Lobe"
    cv::destroyWindow("Select a Lobe");

    // Generate a Mask
    cv::Mat mask(image.size(), image.type());
    //GenerateMask(mask_type, 200, ret_point, mask);
    GenerateMask(mask_type, 200, mouse.x, mouse.y, mask);



    cv::imshow("Mask", mask);
    cv::waitKey();
    return image;
}

cv::Mat Fourier::DrawSquare(cv::Point sel_point, int size, cv::Mat image){
    cv::Mat image_copy;
    image.copyTo(image_copy);

    // Validate the dimensions
    int x, y;
    x = sel_point.x-size/2;
    y = sel_point.y-size/2;

    if( x<0 )
        x = 0;
    if( y<0 )
        y = 0;
    if( x+size > image.cols )
        x = image.cols - size;
    if( y+size > image.rows )
        y = image.rows - size;

    cv::Rect rec( x, y, size, size);
    cv::rectangle(image_copy, rec, cv::Scalar(0));
    return image_copy;
}

cv::Mat Fourier::DrawSquare( int xp, int yp, int size, cv::Mat image){
    cv::Mat image_copy;
    image.copyTo(image_copy);

    // Validate the dimensions
    int x, y;
    x = xp-size/2;
    y = yp-size/2;

    if( x<0 )
        x = 0;
    if( y<0 )
        y = 0;
    if( x+size > image.cols )
        x = image.cols - size;
    if( y+size > image.rows )
        y = image.rows - size;

    cv::Rect rec( x, y, size, size);
    cv::rectangle(image_copy, rec, cv::Scalar(0));
    return image_copy;
}

void Fourier::GenerateMask(int type, int size, cv::Point sel_point, cv::Mat& mask){
    mask = cv::Mat::zeros(mask.rows, mask.cols,mask.type());

    // Validate the dimensions
    int x, y;
    x = sel_point.x-size/2;
    y = sel_point.y-size/2;

    if( x<0 )
        x = 0;
    if( y<0 )
        y = 0;
    if( x+size > mask.cols )
        x = mask.cols - size;
    if( y+size > mask.rows )
        y = mask.rows - size;

    // Define image ROI
    cv::Mat imageROI;
    imageROI = mask(cv::Rect(x, y, size, size));
    //cv::Mat image = cv::Mat::ones(imageROI.rows, mask.cols, imageROI.type());
    imageROI += 1;
}

void Fourier::GenerateMask(int type, int size, int xp, int yp, cv::Mat& mask){
    mask = cv::Mat::zeros(mask.rows, mask.cols,mask.type());

    // Validate the dimensions
    int x, y;
    x = xp-size/2;
    y = yp-size/2;

    if( x<0 )
        x = 0;
    if( y<0 )
        y = 0;
    if( x+size > mask.cols )
        x = mask.cols - size;
    if( y+size > mask.rows )
        y = mask.rows - size;

    // Define image ROI
    cv::Mat imageROI;
    imageROI = mask(cv::Rect(x, y, size, size));
    //cv::Mat image = cv::Mat::ones(imageROI.rows, mask.cols, imageROI.type());
    imageROI += 1;
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
