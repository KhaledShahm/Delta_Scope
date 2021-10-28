/************************************************/
/*  Cam_Stream.cpp								*/
/*												*/
/* Author	: DELTA CARE						*/
/* Date		: 06-Aug-21 2:07:09 AM				*/
/* Version	: V01								*/
/************************************************/


#include "Cam_Stream.h"


Cam_Stream::Cam_Stream()
{
}




void Cam_Stream::init(int device_id, int apiID)
{
    // Open selected camera using selected API
    this->stream.open(device_id, apiID);
    if (!this->stream.isOpened()) {
        std::cerr << "(!)ERROR: Unable to open camera\n";
        exit(EXIT_FAILURE);
    }

    this->stream.read(this->frame);
    if (this->frame.empty())
    {
        std::cerr << "(!)Error1: Blank frame grabbed\n";
        return;
    }
}




void Cam_Stream::start()
{
    this->stopped = false;
    std::thread t1(&Cam_Stream::update, this);
    t1.detach();
}


void Cam_Stream::update()
{
    //--- GRAB AND WRITE LOOP


    while (true)
    {
        if (this->stopped)  return;


        this->m.lock();

        // wait for a new frame from camera and store it into 'frame'
        this->stream.read(this->frame);



        if (this->frame.empty())
        { //check if succeeded
            std::cerr << "(!)Error: Blank frame grabbed\n";
            return;
        }


        this->m.unlock();

    }
}


// return the frame most recently read
cv::Mat* Cam_Stream::read()
{
    return &this->frame;
}


// make thread stop
void Cam_Stream::stop()
{
    this->stopped = true;
}