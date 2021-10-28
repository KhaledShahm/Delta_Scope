/************************************************/
/*  Cam_Stream.h								*/
/*												*/
/* Author	: DELTA CARE						*/
/* Date		: 06-Aug-21 2:07:09 AM				*/
/* Version	: V01								*/
/************************************************/


#ifndef 	CAM_STREAM_H
#define		CAM_STREAM_H

#include <thread>
#include <mutex>
#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;


class Cam_Stream
{
public:
	Mat				 frame;
	Cam_Stream();
	void start();
	void update();
	Mat* read();
	void stop();
	void init(int device_id, int apiID);								// base constructor 

private:
	bool stopped = false;

	VideoCapture	  stream;	// Intialize VideoCapture
	
	mutex			  m;
	
};

#endif  //CAM_STREAM_H