/************************************************/
/*  Cv_utils.h  								*/
/*												*/
/* Author	: DELTA CARE						*/
/* Date		: 06-Aug-21 2:07:09 AM				*/
/* Version	: V01								*/
/************************************************/

#ifndef CV_UTILS_H
#define CV_UTILS_H

#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/stitching.hpp>
#include <math.h>
#include <vector>
#include<tuple> // for tuple_size and tuple


#define BLOCK 60


using namespace std;
using namespace cv;

class Cv_utils
{
private:
    /* data */
    string results_dir = "assets/results/";
    double median(Mat image);
public:
    Cv_utils();
    ~Cv_utils();


    Mat read_image(string Path, bool gray = false);


    Mat stitch(string path, bool jpg = true);


    void save_image(Mat image, string name);


    void show_image(Mat image, string name = "image", int wait = 0);


    tuple <Mat, int> get_areas(Mat image);


    Mat gray2rgb(Mat image);


    Mat to_gray(Mat image);


    Mat to_binary(Mat image, int threshold = 130);


    Mat to_binary2(Mat image);


    Mat to_binary3(Mat image);


    bool is_gray(Mat image);


    Mat enhance(Mat img, int alpha = 2, int beta = -0.8);


    Mat resize_image(Mat image, int width = 500, int height = 500);


    Mat to_hsv(Mat image);


    tuple<int, int, int> blur_eval(Mat image, bool mask, Mat masked);


    Mat dilation(Mat image, int ksize = 5);


    Mat close(Mat image, int ksize1 = 5, int ksize2 = 5);


    Mat open(Mat image, int ksize1 = 5, int ksize2 = 5);


    Mat erosion(Mat image, int ksize = 5);


    int get_fft(Mat image, bool resize, int width, int height);


    Mat auto_canny(Mat image, float sigma, bool resize, int width, int height, bool mask, Mat masked);


    Mat get_sperm_mask(Mat image, Mat mask);


    tuple<int, Mat, Mat, Mat> cc(Mat image);


    bool found_color(Mat image, Mat mask, int area_threshold = 700, int threshold = 30);


    Mat apply_mask(Mat image, Mat mask);


    bool found_sperm(Mat image);


    Mat anding(Mat image, Mat mask);


    Mat oring(Mat image, Mat mask);


    Mat xoring(Mat image, Mat mask);
};

#endif