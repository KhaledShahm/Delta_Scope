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
    Mat enhance(Mat img, int alpha = 2, int beta = -0.8);
    Mat to_gray(Mat image);
    Mat to_binary(Mat image, int threshold = 130);
    Mat to_binary2(Mat image);
    Mat to_binary3(Mat image);
    bool is_gray(Mat image);
    Mat resize_image(Mat image, int width = 250, int height = 250);
    Mat to_hsv(Mat image);
    tuple <Mat, int> get_areas(Mat image);
    Mat auto_canny(Mat image, float sigma = 0.33, bool resize = false, int resize_thresh = 500, bool mask = false);
    Mat apply_mask(Mat image, Mat mask);
    Mat get_sperm_mask(Mat image);
    tuple<int, Mat, Mat, Mat> cc(Mat image);
    bool found_color(Mat image, int area_threshold = 1000, int threshold = 100);
    int get_fft(Mat image, int width = 500, int height = 500, bool with_blue_mask = false);
    bool found_sperm(Mat image);

};

#endif