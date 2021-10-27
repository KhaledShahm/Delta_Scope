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
	Cam_Stream(int device, int api);
	void start();
	void update();
	Mat* read();
	void stop();

private:
	int apiID = CAP_ANY;	// 0 = autodetect default API
	int device_id = 0;		// 0 = Open default camera
	bool stopped = false;

	VideoCapture	  stream;	// Intialize VideoCapture
	mutex			  m;

	Cam_Stream& operator=(const Cam_Stream& o);	// protect assignment
	Cam_Stream(const Cam_Stream&) {};           // protect copy constructor
	void init();								// base constructor 
};

#endif  //CAM_STREAM_H