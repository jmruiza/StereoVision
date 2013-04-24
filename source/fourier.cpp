#include "fourier.h"
#include "pdi.h"

// ==============================================================================================
// The struct "Mouse" and function "mouseEvent" must be externs to the class
// ==============================================================================================

// Mouse controller struct
struct Mouse{
    int event;
    int x;
    int y;
};

// Control mouse events
void mouseEvent(int evt, int x, int y, int flags, void* param){
    Mouse* mouse = (Mouse*) param;
    mouse->event = evt;
    mouse->x = x;
    mouse->y = y;
}
// ==============================================================================================

Fourier::Fourier()
{
}

void Fourier::FourierDFT(const cv::Mat image_src, cv::Mat &image_dst){
    if(image_src.empty()){
        image_src.copyTo(image_dst);
        return;
    }

    // 1. Expand input image to optimal size
    cv::Mat padded;
    // Get the optimal size to DFT
    int m = cv::getOptimalDFTSize( image_src.rows );
    int n = cv::getOptimalDFTSize( image_src.cols );
    // Add zero values to the border
    cv::copyMakeBorder(image_src,               // Input
                       padded,                  // Output
                       0,                       // Top
                       m - image_src.rows,      // Bottom
                       0,                       // Left
                       n - image_src.cols,      // Right
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

    magI.copyTo(image_dst);

    // Normalize the spectrum
    //cv::normalize(magI, image_dst, 0, 1, CV_MINMAX);
}

void Fourier::FourierDFTInverse(const cv::Mat image_src, cv::Mat& image_dst){
    if(image_src.empty()){
        image_src.copyTo(image_dst);
        return;
    }

    // 1. Expand input image to optimal size
    cv::Mat padded;
    // Get the optimal size to DFT
    int m = cv::getOptimalDFTSize( image_src.rows );
    int n = cv::getOptimalDFTSize( image_src.cols );
    // Add zero values to the border
    cv::copyMakeBorder(image_src,           // Input
                       padded,              // Output
                       0,                   // Top
                       m - image_src.rows,  // Bottom
                       0,                   // Left
                       n - image_src.cols,  // Right
                       cv::BORDER_CONSTANT, // Border type
                       cv::Scalar::all(0)); // Value

    // 2. Make place for both the complex and the real values.
    cv::Mat planes[] = { cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F) };
    cv::Mat complexI;

    // Add the expanded image and a plane with zeros a complexI
    cv::merge(planes, 2, complexI);

    // 3. Discrete Fourier transform
    cv::dft(complexI, complexI, cv::DFT_INVERSE);

    // 4. Compute the magnitude -> sqrt(Re(DFT(I))^2 + Im(DFT(I))^2)
    cv::split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
    cv::Mat magI = planes[0];

    // 5. Switch to logarithmic scale -> log(1 + Magnitude)
    magI += cv::Scalar::all(1);
    cv::log(magI, magI);
    // Crop the spectrum, if it has an odd number of rows or columns
    magI = magI(cv::Rect(0, 0, magI.cols & -2, magI.rows & -2));

    magI.copyTo(image_dst);
    // Normalize the spectrum
    // cv::normalize(magI, image_dst, 0, 1, CV_MINMAX);
}

void Fourier::FourierConvolution(const cv::Mat image_src, cv::Mat& image_dst){
    if(image_src.empty()){
        image_src.copyTo(image_dst);
        return;
    }

    PDI pdi;
    int sigma = 5;

    // Generating Kernel
    double h0[6*sigma];
    double h1[6*sigma];
    pdi.gaussianArrays(sigma, h0, h1);

    // Convert image_in to double
    cv::Mat g = pdi.convertToDouble(image_src);

    // Normalizate convolution
    cv::Mat i = cv::Mat::ones(image_src.rows, image_src.cols, CV_64F);
    cv::Mat eta = pdi.convolutionSeparatedKernel(i, h0, 6*sigma, h1, 6*sigma);
    cv::Mat f = cv::Mat::zeros(image_src.rows, image_src.cols, CV_64F);

    for(int y=0; y<image_src.rows; y++){
        for(int x=0; x<image_src.cols; x++){
            f.at<double>(y,x) = g.at<double>(y,x) / eta.at<double>(y,x);
        }
    }

    f.copyTo(image_dst);
    // Normalize image
    //cv::normalize(f, image_dst, 0, 1, CV_MINMAX);
}

void Fourier::FFTShift(const cv::Mat image_src, cv::Mat& image_dst){
    image_src.copyTo(image_dst);

    // Rearrange the quadrants of Fourier image so that the origin is at the image center
    int cx = image_dst.cols/2;
    int cy = image_dst.rows/2;

    // Create a ROI per quadrant
    cv::Mat q0(image_dst, cv::Rect(0, 0, cx, cy));   // Top-Left
    cv::Mat q1(image_dst, cv::Rect(cx, 0, cx, cy));  // Top-Right
    cv::Mat q2(image_dst, cv::Rect(0, cy, cx, cy));  // Bottom-Left
    cv::Mat q3(image_dst, cv::Rect(cx, cy, cx, cy)); // Bottom-Right

    // swap quadrants (Top-Left with Bottom-Right)
    cv::Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    // swap quadrant (Top-Right with Bottom-Left)
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);

    //cv::normalize(image_dst, image_dst, 0, 1, CV_MINMAX);
}

cv::Mat Fourier::LobeFilter(cv::Mat image, int mask_type, int mask_size){
    // Make a copy of the image to work
    cv::Mat image_copy;
    image.copyTo(image_copy);

    // Mouse control object
    Mouse mouse;

    // Window to selection
    cv::namedWindow("Select a Lobe");
    cv::moveWindow("Select a Lobe", 50, 50);

    // Event Mouse controller
    cvSetMouseCallback("Select a Lobe", mouseEvent, (void*)(&mouse));

    while(cv::waitKey(1)){
        // Show selected area
        switch(mask_type){
            case 0:
                image_copy = DrawSquare(mouse.x, mouse.y, mask_size, image);
                break;
            case 1:
                image_copy = DrawCircle(mouse.x, mouse.y, mask_size, image);
                break;
            case 2:
                image_copy = DrawCircle(mouse.x, mouse.y, mask_size, image);
                break;
            default:
                break;
        }
        cv::normalize(image_copy, image_copy, 0, 1, CV_MINMAX);
        cv::imshow("Select a Lobe", image_copy);

        // Until dont press mouse left key
        if(mouse.event == 1)
            break;
    }

    // Destroy the window "Select a Lobe"
    cv::destroyWindow("Select a Lobe");

    // Generate a Mask
    cv::Mat mask(image.size(), image.type());
    //GenerateMask(mask_type, 200, ret_point, mask);
    GenerateMask(mask_type, 200, mouse.x, mouse.y, mask);
    // Multiply image and filter (Mask)
    cv::multiply(image,mask,image);
    return image;
}

cv::Mat Fourier::DrawSquare( int xc, int yc, int size, cv::Mat image){
    cv::Mat image_copy;
    image.copyTo(image_copy);

    // Validate the dimensions
    int x, y;
    validateDimensions(xc, yc, x, y, size, image);

    cv::Rect rec( x, y, size, size);
    cv::rectangle(image_copy, rec, cv::Scalar(0));
    return image_copy;
}

cv::Mat Fourier::DrawCircle( int xc, int yc, int size, cv::Mat image){
    cv::Mat image_copy;
    image.copyTo(image_copy);

    // Validate the dimensions
    int x, y;
    validateDimensions(xc, yc, x, y, size, image);
    //cv::Point

    cv::circle(image_copy, cv::Point(x+size/2,y+size/2), size/2, cv::Scalar(0));
    return image_copy;
}

void Fourier::GenerateMask(int mask_type, int size, int xc, int yc, cv::Mat& mask){
    mask = cv::Mat::zeros(mask.rows, mask.cols,mask.type());
    // Validate the dimensions
    int x, y;
    validateDimensions(xc, yc, x, y, size, mask);
    // Define image ROI
    cv::Mat imageROI;
    imageROI = mask(cv::Rect(x, y, size, size));

    // Show selected area
    switch(mask_type){
        case 0:
            imageROI = 1;
            break;

        case 1:
            cv::circle(imageROI, cv::Point(size/2, size/2), size/2, cv::Scalar(1),-1);
            break;

        case 2:{
            PDI pdi;
            int sigma = size/6;
            cv::Mat gaussian = pdi.gaussianImage(sigma);

            cv::normalize(gaussian, gaussian, 0, 1, CV_MINMAX);

            for(int j=0; j<imageROI.rows; j++){
                for(int i=0; i<imageROI.cols; i++){
                    imageROI.at<double>(j,i) = 1; //gaussian.at<double>(j,i);
                }
            }


            double gmin, gmax, imin, imax;
            pdi.getImageRangeDouble(gaussian, gmin, gmax);
            pdi.getImageRangeDouble(imageROI, imin, imax);
            std::cout << gaussian.type() << " = " << imageROI.type() << std::endl;
            std::cout << gaussian.rows << " x " << gaussian.cols << " = "
                      << imageROI.rows << " x " << imageROI.cols << std::endl;
            std::cout << gmin << " - " << gmax << " = "
                      << imin << " - " << imax << std::endl;
            imshow("gaussian",gaussian);
            break;
        }
        default:
            break;
    }

    cv::normalize(mask, mask, 0, 1, CV_MINMAX);
    imshow("Mask",mask);
    cv::waitKey();
}

void Fourier::validateDimensions(int xc, int yc, int& x, int& y, int size, const cv::Mat image){
    // Validate the dimensions
    x = xc-size/2;
    y = yc-size/2;

    if( x<0 )
        x = 0;
    if( y<0 )
        y = 0;
    if( x+size > image.cols )
        x = image.cols - size;
    if( y+size > image.rows )
        y = image.rows - size;
}
