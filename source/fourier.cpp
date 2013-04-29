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

std::vector<cv::Mat_<float> > Fourier::FourierDFT(const cv::Mat image_src){
    // 1. Expand input image to optimal size to DFT (Adding zero values to border)
    cv::Mat padded;
    int m = cv::getOptimalDFTSize( image_src.rows );
    int n = cv::getOptimalDFTSize( image_src.cols );    
    cv::copyMakeBorder( // Add zero values to the border
                       image_src,               // Input
                       padded,                  // Output
                       0,                       // Top
                       m - image_src.rows,      // Bottom
                       0,                       // Left
                       n - image_src.cols,      // Right
                       cv::BORDER_CONSTANT,     // Border type
                       cv::Scalar::all(0));     // Value

    // 2. Make place for both the complex and the real values.
    cv::Mat_<float> planes[] = { cv::Mat_<float>(padded), cv::Mat_<float>::zeros(padded.size()) };
    cv::Mat complexI;
    // Add the expanded image and a plane with zeros a complexI
    cv::merge(planes, 2, complexI);

    // 3. Discrete Fourier transform
    cv::dft(complexI, complexI);

    // 4. Separate data
    // planes[0] = Re(DFT(I)
    // planes[1] = Im(DFT(I))
    cv::split(complexI, planes);

    // 5. Crop the spectrum data
    // planes[0] = planes[0] (cv::Rect(0,0, planes[0].cols & -2, planes[0].rows & -2));
    // planes[1] = planes[1] (cv::Rect(0,0, planes[1].cols & -2, planes[1].rows & -2));

    // 6. Returned vector
    std::vector<cv::Mat_<float> > output;
    output.push_back(planes[0]);
    output.push_back(planes[1]);

    return output;
}

std::vector<cv::Mat_<float> > Fourier::FourierDFTInverse(std:: vector<cv::Mat_<float> > input){
    // 1. Expand input image planes to optimal size to DFT (Adding zero values to border)
    cv::Mat paddedR;
    cv::Mat paddedI;
    int m = cv::getOptimalDFTSize(input[0].rows);
    int n = cv::getOptimalDFTSize(input[0].cols);
    cv::copyMakeBorder(input[0], paddedR, 0, m-input[0].rows, 0, n-input[0].cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
    cv::copyMakeBorder(input[1], paddedI, 0, m-input[1].rows, 0, n-input[1].cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

    // 2. Join the Re(DFT(I) and Im(DFT(I)) in complex image
    cv::Mat_<float> planes[] ={ paddedR, paddedI };
    cv::Mat complexI;
    cv::merge(planes, 2, complexI);

    // 3. Discrete Fourier Inverse Transform
    cv:: dft(complexI, complexI, cv::DFT_INVERSE);
    // 4. Separate data
    // planes[0] = Re(DFT(I)
    // planes[1] = Im(DFT(I))
    cv:: split(complexI, planes);

    // 5. Crop the spectrum data
    planes[0] = planes[0] (cv::Rect(0,0, planes[0].cols & -2, planes[0].rows & -2));
    planes[1] = planes[1] (cv::Rect(0,0, planes[1].cols & -2, planes[1].rows & -2));

    // 6. Returned vector
    std::vector<cv::Mat_<float> > output;
    output.push_back(planes[0]);
    output.push_back(planes[1]);
    return output;
}

cv::Mat Fourier::getImageMagnitude(std::vector<cv::Mat_<float> > input){
    // Compute the magnitude
    // sqrt(Re(DFT(I))^2 + Im(DFT(I))^2)
    // planes[0] = Re(DFT(I)
    // planes[1] = Im(DFT(I))
    cv::Mat magI;
    magnitude(input[0], input[1], magI);

    // Switch to logarithmic scale -> log(1 + Magnitude)
    magI += cv::Scalar::all(1);
    cv::log(magI, magI);

    // Normalize image without Rearrange
    cv::Mat magI1;
    cv::normalize(magI, magI1, 0, 1, CV_MINMAX);
    return magI1;
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

void Fourier::FFTShift(std:: vector<cv::Mat_<float> >& input){
    // FFTShit() to each plane of vector
    FFTShift(input[0]);
    FFTShift(input[1]);
}

void Fourier::FFTShift(cv::Mat& image_src){
    // Rearrange the quadrants of Fourier image so that the origin is at the image center
    int cx = image_src.cols/2;
    int cy = image_src.rows/2;

    // Create a ROI per quadrant
    cv::Mat q0(image_src, cv::Rect(0, 0, cx, cy));   // Top-Left
    cv::Mat q1(image_src, cv::Rect(cx, 0, cx, cy));  // Top-Right
    cv::Mat q2(image_src, cv::Rect(0, cy, cx, cy));  // Bottom-Left
    cv::Mat q3(image_src, cv::Rect(cx, cy, cx, cy)); // Bottom-Right

    // swap quadrants (Top-Left with Bottom-Right)
    cv::Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    // swap quadrant (Top-Right with Bottom-Left)
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

void Fourier::LobeFilter(std::vector<cv::Mat_<float> > &input, int mask_type, int mask_size){
    // Get Image Magnitude
    cv::Mat image_copy;
    cv::Mat image = getImageMagnitude(input);
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
    GenerateMask(mask_type, mask_size, mouse.x, mouse.y, mask);
    // Multiply image and filter (Mask)
    cv::multiply(input[0], mask, input[0]);
    cv::multiply(input[1], mask, input[1]);
/*
    cv::imshow("input[0]", input[0]);
    cv::imshow("input[1]", input[1]);
    cv::waitKey();
*/
}

cv::Mat Fourier::UnwrappedPhase(std::vector<cv::Mat_<float> >& input){
    cv::Mat_<float> phase(input[0].rows, input[0].cols);

    for(int i=0; i<phase.rows; i++){
        for(int j=0; j<phase.cols; j++) {
            phase(i,j) = atan2( input[1](i,j), input[0](i,j) );
        }
    }
    return phase;
}

cv::Mat Fourier::PhaseDiference(cv::Mat_<float> phase1, cv::Mat_<float> phase2){
    cv::Mat_<float> diff(phase1.rows, phase1.cols);

    for(int i=0; i<diff.rows; i++){
        for(int j=0; j<diff.cols; j++) {
            diff(i,j) = atan2( sin(phase1(i,j) - phase2(i,j) ),
                               cos(phase1(i,j) - phase2(i,j) ));
        }
    }
    return diff;
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
    mask = cv::Mat::zeros(mask.rows, mask.cols, CV_32F);
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
                    imageROI.at<float>(j,i) = static_cast<float>( gaussian.at<double>(j,i) );
                }
            }
            break;

        }
        default:
            break;
    }
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
