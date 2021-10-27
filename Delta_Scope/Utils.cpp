/************************************************/
/*  Utils.cpp	              					*/
/*												*/
/* Author	: DELTA CARE						*/
/* Date		: 06-Aug-21 2:07:09 AM				*/
/* Version	: V01								*/
/************************************************/


#include <chrono>
#include "Utils.h"


int Utils::getNumFrames()
{
    return this->numFrames;
}


void Utils::start_time()
{
    this->timeStart = clock();
}


void Utils::stop_time()
{
    this->timeStop = clock();
}


void Utils::update_frame()
{
    this->numFrames++;
}


double Utils::elapsed_time()
{
    double elapsed_secs = double(this->timeStop - this->timeStart) / CLOCKS_PER_SEC;
    return elapsed_secs;
}


double Utils::fps()
{
    double fps = this->numFrames / this->elapsed_time();
    return fps;
}

