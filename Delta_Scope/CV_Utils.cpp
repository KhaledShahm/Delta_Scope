/************************************************/
/*  Cv_utils.h  								*/
/*												*/
/* Author	: DELTA CARE						*/
/* Date		: 06-Aug-21 2:07:09 AM				*/
/* Version	: V01								*/
/************************************************/


#include "CV_Utils.h"




Cv_utils::Cv_utils()
{

}


Cv_utils::~Cv_utils()
{
}


double Cv_utils::median(Mat image)
{
    double m = (image.rows * image.cols) / 2;
    int bin = 0;
    double med = -1.0;

    int histSize = 256;
    float range[] = { 0, 256 };
    const float* histRange = { range };
    bool uniform = true;
    bool accumulate = false;
    Mat hist;
    calcHist(&image, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);

    for (int i = 0; i < histSize && med < 0.0; ++i)
    {
        bin += cvRound(hist.at< float >(i));
        if (bin > m && med < 0.0)
            med = i;
    }

    return med;
}


Mat Cv_utils::read_image(string Path, bool gray)
{
    if (gray) return imread(Path, IMREAD_GRAYSCALE);
    else return imread(Path);
}


Mat Cv_utils::stitch(string path, bool jpg)
{
    vector<Mat> imgs;
    vector<String> fn;
    if (jpg) path += "/*.jpg";
    else path += "/*.png";
    glob(path, fn, false);

    vector<Mat> images;
    size_t count = fn.size(); //number of jpg files in images folder


    // Get all the images that need to be 
    // stitched as arguments from path
    for (size_t i = 0; i < count; i++)
        imgs.push_back(imread(fn[i]));

    // Define mode for stitching as panoroma 
    // (One out of many functions of Stitcher)
    Stitcher::Mode mode = Stitcher::PANORAMA;

    // Define object to store the stitched image
    Mat pano;

    // Create a Stitcher class object with mode panoroma
    Ptr<Stitcher> stitcher = Stitcher::create(mode);

    // Command to stitch all the images present in the image array
    Stitcher::Status status = stitcher->stitch(imgs, pano);

    //if (status != Stitcher::OK) return;

    return pano;
}


void Cv_utils::save_image(Mat image, string name)
{
    string path = results_dir;
    path += name;
    path += ".jpg";
    imwrite(path, image);
}


void Cv_utils::show_image(Mat image, string name, int wait)
{
    imshow(name, image);
    waitKey(wait);
    destroyWindow(name);
}


tuple <Mat, int>  Cv_utils::get_areas(Mat image)
{
    Mat enhanced = this->enhance(image, 1000, -960);
    enhanced = this->to_binary(enhanced);
    enhanced = ~enhanced;
    int a = sum(enhanced)[0];
    return make_tuple(enhanced, a);
}


Mat Cv_utils::gray2rgb(Mat image)
{
    Mat RGB;
    cvtColor(image, RGB, COLOR_GRAY2RGB);
    return RGB;
}


Mat Cv_utils::to_gray(Mat image)
{
    if (this->is_gray(image)) return image;

    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);
    return gray;
}


Mat Cv_utils::to_binary(Mat image, int threshold)
{
    Mat gray = image, image_copy;
    if (!(this->is_gray(image)))  gray = this->to_gray(image);

    // Make a copy of the image
    gray.copyTo(image_copy);

    for (int x = 0; x < image_copy.rows; x++)
    {
        for (int y = 0; y < image_copy.cols; y++)
        {
            // Accesssing values of each pixels
            if (image_copy.at<uchar>(x, y) > threshold)
            {
                image_copy.at<uchar>(x, y) = 255;
            }
            else
            {
                image_copy.at<uchar>(x, y) = 0;
            }
        }
    }

    return image_copy;
}


Mat Cv_utils::to_binary2(Mat image)
{
    Mat gray = image, thresh;
    if (!(this->is_gray(image)))  gray = this->to_gray(image);

    adaptiveThreshold(gray, thresh, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, 5);
    return thresh;
}


Mat Cv_utils::to_binary3(Mat image)
{
    Mat gray = image, thresh;
    if (!(this->is_gray(image)))  gray = this->to_gray(image);


    threshold(gray, thresh, 230, 255, THRESH_BINARY + THRESH_OTSU);
    return thresh;
}


bool Cv_utils::is_gray(Mat image)
{
    return (image.channels() == 1);
}


Mat Cv_utils::enhance(Mat img, int alpha, int beta)
{
    Mat gauss_mask, image_sharp;

    GaussianBlur(img, gauss_mask, Size(23, 23), 10);
    addWeighted(img, alpha, gauss_mask, beta, 0.0, image_sharp);
    return image_sharp;
}


Mat Cv_utils::resize_image(Mat image, int width, int height)
{
    // let's scale the image using new  width and height
    Mat resized;

    //resize
    resize(image, resized, Size(width, height), INTER_LINEAR);
    return resized;

}


Mat Cv_utils::to_hsv(Mat image)
{
    Mat HSV_image;
    cvtColor(image, HSV_image, COLOR_BGR2HSV);
    return HSV_image;
}


tuple<int, int, int> Cv_utils::blur_eval(Mat image, bool mask, Mat masked)
{
    Mat edge;
    int fft_result = this->get_fft(image, true, 500, 500);
    if (mask) edge = this->auto_canny(image, 0.33, false, 500, 500, true, masked);
    edge = this->auto_canny(image, 0.33, false, 500, 500, true, masked);
    int canny_result = sum(edge)[0];
    int total_res = ((int(fft_result)) * 1000) + canny_result;
    return make_tuple(fft_result, canny_result, total_res);
}


Mat Cv_utils::dilation(Mat image, int ksize)
{
    Mat gray = image;
    if (!(this->is_gray(image)))  gray = this->to_gray(image);


    // Create a structuring element (SE)

    Mat element = getStructuringElement(
        MORPH_RECT, Size(2 * ksize + 1,
            2 * ksize + 1),
        Point(ksize, ksize));


    // For Dilation
    Mat dill;
    dilate(image, dill, element,
        Point(-1, -1), 1);

    return dill;
}


Mat Cv_utils::close(Mat image, int ksize1, int ksize2)
{
    Mat gray = image;
    if (!(this->is_gray(image)))  gray = this->to_gray(image);
    Mat dilated = this->dilation(image, ksize1);
    Mat closed = this->erosion(dilated, ksize2);

    return closed;
}


Mat Cv_utils::open(Mat image, int ksize1, int ksize2)
{
    Mat gray = image;
    if (!(this->is_gray(image)))  gray = this->to_gray(image);

    Mat dilated = this->erosion(image, ksize1);
    Mat open = this->dilation(dilated, ksize2);
    return open;
}


Mat Cv_utils::erosion(Mat image, int ksize)
{
    Mat gray = image;
    if (!(this->is_gray(image)))  gray = this->to_gray(image);


    // Create a structuring element (SE)

    Mat element = getStructuringElement(
        MORPH_RECT, Size(2 * ksize + 1,
            2 * ksize + 1),
        Point(ksize, ksize));


    // For Dilation
    Mat erod;
    erode(gray, erod, element,
        Point(-1, -1), 1);

    return erod;

}


int Cv_utils::get_fft(Mat image, bool resize, int width, int height)
{
    Mat gray, resized_image;

    if (!(is_gray(image))) gray = to_gray(image);

    if (resize)resized_image = resize_image(gray, width, height);


    int cx = image.cols / 2;
    int cy = image.rows / 2;

    // Go float
    Mat fImage;
    resized_image.convertTo(fImage, CV_32F);


    // FFT
    Mat fourierTransform;
    dft(fImage, fourierTransform, DFT_SCALE | DFT_COMPLEX_OUTPUT);


    //center low frequencies in the middle
    //by shuffling the quadrants.
    Mat q0(fourierTransform, Rect(0, 0, cx, cy));       // Top-Left - Create a ROI per quadrant
    Mat q1(fourierTransform, Rect(cx, 0, cx, cy));      // Top-Right
    Mat q2(fourierTransform, Rect(0, cy, cx, cy));      // Bottom-Left
    Mat q3(fourierTransform, Rect(cx, cy, cx, cy));     // Bottom-Right


    Mat tmp;                                            // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);


    q1.copyTo(tmp);                                     // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);


    // Block the low frequencies
    // #define BLOCK could also be a argument on the command line of course
    fourierTransform(Rect(cx - BLOCK, cy - BLOCK, 2 * BLOCK, 2 * BLOCK)).setTo(0);

    //shuffle the quadrants to their original position
    Mat orgFFT;
    fourierTransform.copyTo(orgFFT);
    Mat p0(orgFFT, Rect(0, 0, cx, cy));       // Top-Left - Create a ROI per quadrant
    Mat p1(orgFFT, Rect(cx, 0, cx, cy));      // Top-Right
    Mat p2(orgFFT, Rect(0, cy, cx, cy));      // Bottom-Left
    Mat p3(orgFFT, Rect(cx, cy, cx, cy));     // Bottom-Right

    p0.copyTo(tmp);
    p3.copyTo(p0);
    tmp.copyTo(p3);

    p1.copyTo(tmp);                                     // swap quadrant (Top-Right with Bottom-Left)
    p2.copyTo(p1);
    tmp.copyTo(p2);

    // IFFT
    Mat invFFT;
    Mat logFFT;
    double minVal, maxVal;

    dft(orgFFT, invFFT, DFT_INVERSE | DFT_REAL_OUTPUT);


    invFFT = abs(invFFT);
    minMaxLoc(invFFT, &minVal, &maxVal, NULL, NULL);

    if (maxVal <= 0.0)
    {
        return 1;
    }

    log(invFFT, logFFT);
    logFFT *= 20;

    //result = numpy.mean(img_fft)
    Scalar result = mean(logFFT);

    return int(result.val[0]);
}


Mat Cv_utils::auto_canny(Mat image, float sigma, bool resize, int width, int height, bool mask, Mat masked)
{
    Mat gray, blurred, edged;
    image.copyTo(gray);

    if (!(this->is_gray(image))) gray = this->to_gray(image);

    if (resize) gray = this->resize_image(gray, width, height);

    GaussianBlur(gray, blurred, Size(5, 5), 0);
    double v = median(blurred);

    //apply automatic Canny edge detection using the computed median
    int	lower = max(0, int((1.0 - sigma) * v));
    int	upper = min(255, int((1.0 + sigma) * v));

    Canny(blurred, edged, lower, upper);

    if (mask) edged = this->apply_mask(edged, masked);

    return edged;
}


Mat Cv_utils::get_sperm_mask(Mat image, Mat mask)
{
    Mat gray, mask, result;
    if (!is_gray(image))  gray = to_gray(image);


    mask = to_binary(image, 120);
    result = apply_mask(image, mask);
    return result;
}


tuple<int, Mat, Mat, Mat> Cv_utils::cc(Mat image)
{
    Mat labels;
    Mat centroids;
    Mat stats;
    int nlabels;

    //Calcul des différentes composantes connexes de l'image
    nlabels = connectedComponentsWithStats(image, labels, stats, centroids, 4, CV_32S);

    return make_tuple(nlabels, labels, stats, centroids);
}


bool Cv_utils::found_color(Mat image, Mat mask, int area_threshold, int threshold)
{
    Mat blured_image, blue_mask, hsv, resized_image;
    int are_blue_mask = 0;
    resized_image = this->resize_image(image);
    hsv = to_hsv(resized_image);

    GaussianBlur(hsv, blured_image, Size(9, 9), 10);

    inRange(blured_image, Scalar(110, threshold, threshold), Scalar(130, 255, 255), blue_mask);

    for (int x = 0; x < blue_mask.rows; x++)
    {
        for (int y = 0; y < blue_mask.cols; y++)
        {
            // Accesssing values of each pixels
            if (blue_mask.at<uchar>(x, y) == 255)
            {
                are_blue_mask++;
            }
        }
    }

    return are_blue_mask > area_threshold;
}


Mat Cv_utils::apply_mask(Mat image, Mat mask)
{
    Mat copy_image;
    image.copyTo(copy_image);

    for (int x = 0; x < mask.rows; x++)
    {
        for (int y = 0; y < mask.cols; y++)
        {
            // Accesssing values of each pixels
            if (mask.at<uchar>(x, y) == 0)
            {
                copy_image.at<uchar>(x, y) = 0;
            }

        }
    }
    copy_image.convertTo(copy_image, CV_8U);
    return copy_image;
}


bool Cv_utils::found_sperm(Mat image)
{
    return true;
}


Mat Cv_utils::anding(Mat image, Mat mask)
{
    Mat output;
    bitwise_and(image, mask, output);
    output.convertTo(output, CV_8U);
    return output;
}


Mat Cv_utils::oring(Mat image, Mat mask)
{
    Mat output;
    bitwise_or(image, mask, output);
    output.convertTo(output, CV_8U);
    return output;
}


Mat  Cv_utils::xoring(Mat image, Mat mask)
{
    Mat output;
    bitwise_xor(image, mask, output);
    output.convertTo(output, CV_8U);
    return output;
}