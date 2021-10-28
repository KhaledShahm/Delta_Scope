#include <opencv2/opencv.hpp>
//#include "MicroscopeHW.h"
#include "Cam_Stream.h"
#include "Utils.h"
//#include "CV_Utils.h"
using namespace cv;

int main()
{
    //Cv_utils cv_utils;
	Cam_Stream CS;
	Utils utils;
	int num_frames = 60;
    cv::Mat* frame;
    Mat test_image;
    // start streaming video
    CS.start();
    utils.start_time();
   
    while (utils.getNumFrames() < 60)
    {
        frame = CS.read();
        //tie(test_image, f) = cv_utils.get_areas(*frame);
        //

        //// show live and wait for a key with timeout long enough to show images
        cv::imshow("Live", *frame);
        if (cv::waitKey(5) >= 0) {
            break;
        }
        /*cvtColor(*frame, gray, COLOR_BGR2GRAY);
        Canny(gray, edge, lowThreshold, lowThreshold * ratio, kernel_size);*/
        utils.update_frame();
    }
	utils.stop_time();
	CS.stop();
	std::cout <<"Elapsed_Time: " << utils.elapsed_time() << std::endl;
    std::cout <<"FPS: " << utils.fps() << std::endl;
	exit(0);
	return 0;
}