#include "pdi.h"

/* Constructor */
PDI::PDI(){
}

PDI::~PDI(){
}

void PDI::mosaicEcualization(cv::Mat &img, int pixels, bool interpolate){
    cv::Mat temp;
    imageRGB(img);
    // Slices (pixels * pixels)
    int hor = int(img.cols/pixels);
    int ver = int(img.rows/pixels);
    int rHor = int(img.cols%pixels);
    int rVer = int(img.rows%pixels);
    int x, y;

    for(int i=0; i<ver; i++){ // Moving in rows
        y = i*pixels;
        for(int j=0; j<hor; j++){
            x = j*pixels;
            temp = cv::Mat(img, cvRect(x, y, pixels, pixels));
            // Ecualizing subimage
            ecualizeImage(temp);
            if(rHor>0 && j==hor-1){
                temp = cv::Mat(img, cvRect(x+pixels, y, rHor, pixels));
                // Ecualizing subimage
                ecualizeImage(temp);
            }
        }
        if(rVer>0 && i==ver-1){
            for(int j=0; j<hor; j++){
                x = j*pixels;
                temp = cv::Mat(img, cvRect(x, y+pixels, pixels, rVer));
                // Ecualizing subimage
                ecualizeImage(temp);
                if(rHor>0 && j==hor-1){
                    temp = cv::Mat(img, cvRect(x+pixels, y+pixels, rHor, rVer));
                    // Ecualizing subimage
                    ecualizeImage(temp);
                }
            }
        }
    }

    if(interpolate){
        if(img.cols < 2*pixels || img.rows < 2*pixels ){
            std::cout << "ERROR: It's not possible to equalize, try again changing the dimensions of the sub-images." << std::endl;
            std::cout << "Image Dimensions: " << img.rows << ", " << img.cols << std::endl;
            std::cout << "Sub-image Dimensions: " << pixels << ", " << pixels << std::endl;
        }
        else{
            // Get the image range (to re-convert)
            int min, max;
            getImageRange(img, min, max);

            int subimg = 0;
            int f00, f01, f10, f11;

            cv::Mat im = convertToDouble(img);


            // img: imagen de entrada (en cuadritos)
            // pixels: dimensiones del cuadrito
            // hor: num de cuadros horizontales
            // ver: num de cuadros verticales
            // rHor: pixeles horizontales sobrantes
            // rVer: pixeles verticales sobrantes
            // y: fila inicial para la sub imagen
            // x: columna inicial para la sub imagen

            std::cout << "Image dims: " << img.rows << " x " << img.cols << std::endl;

            for(int v=0; v<ver; v++){ // Moving in rows (Vertical)
                y = v*pixels + pixels/2;
                for(int h=0; h<hor; h++){ // Moving in colums (Horizontal)
                    x = h*pixels + pixels/2;
                    std::cout << "(" << v << ", " << h << "): " << y << ", " << x << std::endl;
                    if( y+pixels < img.rows && x+pixels < img.cols ){
                        temp = cv::Mat(img, cvRect(x, y, pixels, pixels));
                        for(int i=0; i<temp.rows; i++){
                            for(int j=0; j<temp.cols; j++){
                                int s = j + 1;
                                int t = i + 1;
                                f00 = vHistograms[subimg].HistA[temp.at<uchar>(i, j)];
                                f01 = vHistograms[subimg+1].HistA[temp.at<uchar>(i, j)];
                                f10 = vHistograms[subimg+ver].HistA[temp.at<uchar>(i, j)];
                                f11 = vHistograms[subimg+ver+1].HistA[temp.at<uchar>(i, j)];
                                temp.at<uchar>(i,j) = (1-s) * (1-t) * f00 + s * (1-t) * f10
                                        + (1-s) * t+1 * f01 + s * t * f11;
                            }
                        }
                    }
                    subimg++;
                }
            }
/*




            for(int v=0; v<ver; v++){ // Moving in rows
                y = v*pixels;
                for(int h=0; h<hor; h++){
                    x = h*pixels;
                    // Ecualizing subimage
                    std::cout << subimg << std::endl;
                    Hists = vHistograms[subimg];
                    for(int s=0; s<pixels; s++){
                        for(int t=0; t<pixels; t++){
                            int ss = s + v * pixels;
                            int tt = t + h * pixels;
                            f00 = vHistograms[subimg].HistA[img.at<uchar>(s, t)];
                            f01 = vHistograms[subimg+1].HistA[img.at<uchar>(s, t)];
                            f10 = vHistograms[subimg+ver].HistA[img.at<uchar>(s, t)];
                            f11 = vHistograms[subimg+ver+1].HistA[img.at<uchar>(s, t)];                            // std::cout << "s: " << s << " ss: " << ss << " t: " << t << " tt: " << tt << " => ";
                            // std::cout << "f00(" << 0+v*pixels << ", " << 0+h*pixels << ") - "
                            //          << "f10(" << 0+v*pixels << ", " << pixels-1+h*pixels << ") - "
                            //          << "f01(" << pixels-1+v*pixels << ", " << 0+h*pixels << ") - "
                            //          << "f11(" << pixels-1+v*pixels << ", " << pixels-1+h*pixels << ")" << std::endl;

                            im.at<double>(ss,tt) = (1-s) * (1-t) * f00 + s * (1-t) * f10
                                                 + (1-s) * t+1 * f01 + s * t * f11;
                        }
                    }
                    subimg++;

                    if(rHor>0 && h==hor-1){
                        std::cout << subimg << std::endl;
                        Hists = vHistograms[subimg];
                        // Ecualizing subimage
                        subimg++;


                    }
                }

                if(rVer>0 && v==ver-1){
                    for(int h=0; h<hor; h++){
                        x = h*pixels;
                        std::cout << subimg << std::endl;
                        Hists = vHistograms[subimg];
                        // Ecualizing subimage
                        subimg++;

                        if(rHor>0 && h==hor-1){
                            std::cout << subimg << std::endl;
                            Hists = vHistograms[subimg];
                            // Ecualizing subimage
                            subimg++;

                        }

                    }
                }

            }
*/
            // Image double converted into integer
            // normalizateMatrix(im, min, max);
            // cv::Mat im2 = convertToInterger(im);
            // im2.copyTo(img);
        }
    }
}

bool PDI::imageRGB(const cv::Mat &img){
   // std::cout << "Channels: " << img.channels() << " -> ";
    if( img.channels() == 1 ){
        isRGB = false;
        // std::cout << "Isn't' RGB!!" << std::endl;
        return false;
    }

    for(int i=0; i<img.rows; i++){ // Rows i
        for(int j=0; j<img.cols; j ++){ // Columns j
            if( img.at<cv::Vec3b>(0,0)[0] != img.at<cv::Vec3b>(0,0)[1] ||
                img.at<cv::Vec3b>(0,0)[0] != img.at<cv::Vec3b>(0,0)[2] ||
                img.at<cv::Vec3b>(0,0)[1] != img.at<cv::Vec3b>(0,0)[2] ){
                isRGB = true;
                // std::cout << "Is RGB!!" << std::endl;
                return true;
            }
            else{
                isRGB = false;
            }
        }
    }
    // std::cout << "Isn't' RGB!!" << std::endl;
    return false;
}

void PDI::ecualizeImage(cv::Mat &img){
    // Copy the image to temp image
    cv::Mat out = cv::Mat(img);

    // Get histograms (Normal and accumulated) to image
    histograms Hists;
    initHistograms(Hists.Hist, Hists.HistA);
    getHistograms(img, Hists.Hist, Hists.HistA);
    normalizateArray(Hists.Hist);
    normalizateArray(Hists.HistA);
    vHistograms.push_back(Hists);

    for(int i=0; i<img.rows; i++){ // Rows i
        for(int j=0; j<img.cols; j ++){ // Columns j
            out.at<uchar>(i,j) = Hists.HistA[int(img.at<uchar>(i,j))];
        }
    }

    img = out.clone();
}

void PDI::initHistograms(int *hist, int *histA, int dims){
    for(int i=0; i<dims; i++){
        hist[i] = 0;
        histA[i] = 0;
    }
}

void PDI::printArray(int *hist, int dims){
    for(int i=0; i<dims; i++){
        std::cout << "H[" << i << "]: " << hist[i] << std::endl;
    }
}

void PDI::printArray(double *hist, int dims){
    for(int i=0; i<dims; i++){
        std::cout << "H[" << i << "]: " << hist[i] << std::endl;
    }
}

void PDI::getHistograms(const cv::Mat &img, int *hist, int *histA, int dims){
    // Get Histogram
    for(int i=0; i<img.rows; i++) // Rows i
        for(int j=0; j<img.cols; j ++) // Columns j
            hist[img.at<uchar>(i,j)]++;

    // Get Histogram accumulated
    histA[0] = hist[0];
    for(int i=1; i<dims; i++){
        histA[i] = histA[i-1] + hist[i];
    }
}

void PDI::normalizateArray(int *hist, int dims){
    int max, min;
    max = getMaxValueArray(hist);
    min = getMinValueArray(hist);

    for(int i=0; i<dims; i++){
        hist[i] = dims*(hist[i]-min)/max;
    }
}

void PDI::showHistogram(std::string str, int *hist, int dims){
    // Create a white image with the dimensions of levels in the histogram
    cv::Mat histogram = cv::Mat(dims, dims, CV_8U, cv::Scalar(255));

    // Draw a vertical line of each value in the histagram
    for(int h=0; h<dims; h++){
        cv::line(histogram, cv::Point(h,dims), cv::Point(h,dims-hist[h]), cv::Scalar::all(0));
    }

    cv::namedWindow(str, CV_WINDOW_NORMAL);
    cv::imshow(str, histogram);
}

int PDI::getMaxValueArray(int *hist, int dims){
    int max = hist[0];
    for(int i=1; i<dims; i++){
        if(hist[i] > max)
            max = hist[i];
    }
    return max;
}

int PDI::getMinValueArray(int *hist, int dims){
    int min = hist[0];
    for(int i=1; i<dims; i++){
        if(hist[i] < min)
            min = hist[i];
    }
    return min;
}

// Convolution ==============================================================================

void PDI::gaussianConvolution(int sigma, cv::Mat &image, bool separated_kernel){
    // Get the image range (to re-convert)
    int min, max;
    getImageRange(image, min, max);
    // Create a copy of the values (as double)
    cv::Mat g = convertToDouble(image);
    // Image double of zeros
    cv::Mat f = cv::Mat::zeros(g.rows, g.cols, CV_64F);

    if(separated_kernel){ // Kernel separated routine
        // Generating Kernel
        double h0[6*sigma];
        double h1[6*sigma];
        gaussianArrays(sigma, h0, h1);

        f = convolutionSeparatedKernel(g, h0, 6*sigma, h1, 6*sigma);
    }

    else{ // Unique Kernel routine
        // Generating Kernel
        cv::Mat h = gaussianImage(sigma);
        cv::namedWindow("Kernel",CV_WINDOW_NORMAL);
        cv::imshow("Kernel", convertToInterger(h));

        f = convolution(g, h);
    }

    // Image double converted into integer
    normalizateMatrix(f, min, max);
    cv::Mat img = convertToInterger(f);

    cv::namedWindow("Initial Image");
    cv::imshow("Initial Image", image);
    cv::namedWindow("Final Image");
    cv::imshow("Final Image", img);
}

cv::Mat PDI::convolution(const cv::Mat &g, const cv::Mat &h){
    int ox, oy;
    ox = oy = h.rows / 2; // 6 * sigma / 2
    cv::Mat f = cv::Mat::zeros(g.rows, g.cols, CV_64F);

    for(int y=0; y<f.rows; y++) // Rows y
        for(int x=0; x<f.cols; x++) // Columns x
            for(int n=-oy; n<=oy; n++) // Rows n
                for(int m=-ox; m<=ox; m++) // Columns m
                    if( ( y-n>=0 && x-m>=0 ) && ( y-n<f.rows && x-m<f.cols ) )
                            f.at<double>(y, x) += g.at<double>(y-n,x-m) * h.at<double>(oy-n, ox-m);
    return f;
}

void PDI::testConvolution(){
    cv::Mat g = cv::Mat::ones(10, 10, CV_64F);
    cv::Mat h = cv::Mat::ones(3, 3, CV_64F);

    // cv::Mat f = convolution(g, h);
    cv::Mat f = convolution(g, h);

    for(int y=0; y<f.rows; y++){
        for(int x=0; x<f.cols; x++){
            std::cout << f.at<double>(y,x) << " ";
        }
        std::cout << std::endl;
    }

    normalizateMatrix(f, 0, 255);
    cv::Mat f_d = convertToInterger(f);
    cv::namedWindow("f(x,y)"); //, CV_WINDOW_NORMAL);
    cv::imshow("f(x,y)", f_d);
}

cv::Mat PDI::convolutionSeparatedKernel(const cv::Mat &g, const double *h0, int dim1, const double *h1, int dim2){
    int ox, oy;
    ox = dim1 / 2; // 6 * sigma / 2
    oy = dim2 / 2; // 6 * sigma / 2

    cv::Mat f = cv::Mat::zeros(g.rows, g.cols, CV_64F);
    cv::Mat f1 = cv::Mat::zeros(g.rows, g.cols, CV_64F);

    // Iterate columns
            for(int y=0; y<f.rows; y++)
                for(int x=0; x<f.cols; x++)
                    for(int m=-ox; m<ox; m++)
                        if( !(x-m < 0 || x-m > f.cols) )
                            f1.at<double>(y,x) += g.at<double>(y,x-m) * h0[ox-m];

    // Iterate rows
            for(int y=0; y<f.rows; y++)
                for(int x=0; x<f.cols; x++)
                    for(int m=-oy; m<oy; m++)
                        if( y+m>=0 && y+m<f.rows )
                            f.at<double>(y,x) += f1.at<double>(y+m,x) * h0[oy-m];
    return f;
}

cv::Mat PDI::gaussianImage(int sigma){
    cv::Mat image( 6 * sigma, 6 * sigma, CV_64F);
    double range = 6.0 * sigma / 2.0;
    //std::cout << -rango << " -> " << rango << std::endl;
    float x = -range;
    for(int i=0; i<image.rows; i++){ // Rows i
        float  y = -range;
        for(int j=0; j<image.cols; j++){ // Columns j
            image.at<double>(i,j) = exp( -(x * x + y * y) / (2.0 * sigma * sigma) );
            y++;
        }
        x++;
    }
    return image;
}

void PDI::gaussianArrays(int sigma, double *h0, double *h1 ){
    // Inizializate Arrays
    for(int i=0; i<6*sigma; i++)
        h0[i] = h1[i] = 0.0;

    double range = 6.0 * sigma / 2.0;
    int x = -range;

    for(int i=0; i<6*sigma; i++){
        h0[i] = exp( -(x*x) / (2.0 * sigma * sigma) );
        h1[i] = exp( -(x*x) / (2.0 * sigma * sigma) );
        // std::cout << " x = " << x << " -> h0 = " << h0[i] << " h1 = " << h1[i] << std::endl;
        x++;
    }
}

void PDI::getImageRange(const cv::Mat &image, int &min, int &max){
    min = max = image.at<uchar>(0,0);
    for(int y=0; y<image.rows; y++){ // Rows y
        for(int x=0; x<image.cols; x++){ // Columns j
            if( image.at<uchar>(y,x) > max )
                max = image.at<uchar>(y,x);
            if( image.at<uchar>(y,x) < min )
                min = image.at<uchar>(y,x);
        }
    }
}

void PDI::getImageRangeDouble(const cv::Mat &image, double &min, double &max){
    min = max = image.at<double>(0,0);
    for(int y=0; y<image.rows; y++){ // Rows y
        for(int x=0; x<image.cols; x++){ // Columns j
            if( image.at<double>(y,x) > max )
                max = image.at<double>(y,x);
            if( image.at<double>(y,x) < min )
                min = image.at<double>(y,x);
        }
    }
}

cv::Mat PDI::convertToDouble(const cv::Mat &image){
    cv::Mat im(image.rows, image.cols, CV_64F);
    for(int y=0; y<image.rows; y++){ // Rows y
        for(int x=0; x<image.cols; x++){ // Columns j
            im.at<double>(y,x) = image.at<uchar>(y,x) / 255.0;
        }
    }
    return im;
}

cv::Mat PDI::convertToInterger(const cv::Mat &image){
    cv::Mat im(image.rows, image.cols, CV_8U);
    for(int y=0; y<image.rows; y++){ // Rows y
        for(int x=0; x<image.cols; x++){ // Columns x
            im.at<uchar>(y,x) = image.at<double>(y,x) * 255;
        }        
    }
    return im;
}

void PDI::normalizateMatrix(cv::Mat &img, int min, int max){
    double min2, max2;
    getImageRangeDouble(img, min2, max2 );

    for(int y=0; y<img.rows; y++){ // Rows y
        for(int x=0; x<img.cols; x++){ // Columns x
            img.at<double>(y,x) = ( img.at<double>(y,x) - min2 ) / (max2-min2);
        }
    }
}

// Convolution ==============================================================================
void PDI::testPhase(){
    // Dimensions and definitions
    int M = 300; // Colums
    int N = 300; // Rows
    double fact = 0.0005;
    double w = 3.1416 / 3.0;
    int sigma = 5;

    // Generating phi(x,y)
    cv::Mat phi = cv::Mat::zeros(N, M, CV_64F);
    for(int y=0; y<N; y++)
        for(int x=0; x<M; x++)
            phi.at<double>(y,x) = ((x-M/2.0) * (x-M/2.0) + (y-N/2.0) * (y-N/2.0)) * fact;

    // Generating g(x,y)
    cv::Mat g = cv::Mat::zeros(N, M, CV_64F);
    for(int y=0; y<N; y++)
        for(int x=0; x<M; x++)
            g.at<double>(y,x) = cos( phi.at<double>(y,x) + w * x );

    // Generating g1(x,y) and g2(x,y)
    cv::Mat g1 = cv::Mat::zeros(N, M, CV_64F);
    cv::Mat g2 = cv::Mat::zeros(N, M, CV_64F);

    for(int y=0; y<N; y++)
        for(int x=0; x<M; x++){
            g1.at<double>(y,x) = g.at<double>(y,x) * cos( w * x );
            g2.at<double>(y,x) = g.at<double>(y,x) * sin( w * x );
        }

    // Generating Kernel
    double h0[6*sigma];
    double h1[6*sigma];
    gaussianArrays(sigma, h0, h1);

    // Normal Convolution
    cv::Mat f1 = convolutionSeparatedKernel(g1, h0, 6*sigma, h1, 6*sigma);
    cv::Mat f2 = convolutionSeparatedKernel(g2, h0, 6*sigma, h1, 6*sigma);

    // Phase
    cv::Mat phi_1 = cv::Mat::zeros(N, M, CV_64F);
    for(int y=0; y<N; y++)
        for(int x=0; x<M; x++)
            phi_1.at<double>(y,x) = atan2( f2.at<double>(y,x), f1.at<double>(y,x));


    // Normalizate convolution
    cv::Mat i = cv::Mat::ones(N, M, CV_64F);
    cv::Mat eta = convolutionSeparatedKernel(i, h0, 6*sigma, h1, 6*sigma);
    cv::Mat f1n = cv::Mat::zeros(N, M, CV_64F);
    cv::Mat f2n = cv::Mat::zeros(N, M, CV_64F);

    for(int y=0; y<N; y++)
        for(int x=0; x<M; x++){
            f1n.at<double>(y,x) = f1.at<double>(y,x) / eta.at<double>(y,x);
            f2n.at<double>(y,x) = f2.at<double>(y,x) / eta.at<double>(y,x);
        }

    // Phase
    cv::Mat phi_2 = cv::Mat::zeros(N, M, CV_64F);
    for(int y=0; y<N; y++)
        for(int x=0; x<M; x++)
            phi_2.at<double>(y,x) = atan2( f2n.at<double>(y,x), f1n.at<double>(y,x));



    // To display
//    // phi(x,y)
//    normalizateMatrix(phi, 0, 255);
//    cv::Mat phi_d = convertToInterger(phi);
//    cv::namedWindow("Phi"); //, CV_WINDOW_NORMAL);
//    cv::imshow("Phi", phi_d);
//    // g(x,y)
//    normalizateMatrix(g, 0, 255);
//    cv::Mat g_d = convertToInterger(g);
//    cv::namedWindow("g(x,y)"); //, CV_WINDOW_NORMAL);
//    cv::imshow("g(x,y)", g_d);
//    // g1(x,y)
//    normalizateMatrix(g1, 0, 255);
//    cv::Mat g1_d = convertToInterger(g1);
//    cv::namedWindow("g1(x,y)"); //, CV_WINDOW_NORMAL);
//    cv::imshow("g1(x,y)", g1_d);
//    // g2(x,y)
//    normalizateMatrix(g2, 0, 255);
//    cv::Mat g2_d = convertToInterger(g2);
//    cv::namedWindow("g2(x,y)"); //, CV_WINDOW_NORMAL);
//    cv::imshow("g2(x,y)", g2_d);
//    // f1(x,y)
//    normalizateMatrix(f1, 0, 255);
//    cv::Mat f1_d = convertToInterger(f1);
//    cv::namedWindow("f1(x,y)"); //, CV_WINDOW_NORMAL);
//    cv::imshow("f1(x,y)", f1_d);
//    // f2(x,y)
//    normalizateMatrix(f2, 0, 255);
//    cv::Mat f2_d = convertToInterger(f2);
//    cv::namedWindow("f2(x,y)"); //, CV_WINDOW_NORMAL);
//    cv::imshow("f2(x,y)", f2_d);
//    // f1(x,y)
//    normalizateMatrix(f1n, 0, 255);
//    cv::Mat f1n_d = convertToInterger(f1n);
//    cv::namedWindow("f1(x,y) Normalizated"); //, CV_WINDOW_NORMAL);
//    cv::imshow("f1(x,y) Normalizated", f1n_d);
//    // f2(x,y)
//    normalizateMatrix(f2n, 0, 255);
//    cv::Mat f2n_d = convertToInterger(f2n);
//    cv::namedWindow("f2(x,y) Normalizated"); //, CV_WINDOW_NORMAL);
//    cv::imshow("f2(x,y) Normalizated", f2n_d);

    // phi_1(x,y)
    normalizateMatrix(phi_1, 0, 255);
    cv::Mat phi_1d = convertToInterger(phi_1);
    cv::namedWindow("phi_1(x,y)"); //, CV_WINDOW_NORMAL);
    cv::imshow("phi_1(x,y)", phi_1d);

    // phi_2(x,y)
    normalizateMatrix(phi_2, 0, 255);
    cv::Mat phi_2d = convertToInterger(phi_2);
    cv::namedWindow("phi_2(x,y) Normalizated"); //, CV_WINDOW_NORMAL);
    cv::imshow("phi_2(x,y) Normalizated", phi_2d);
}
