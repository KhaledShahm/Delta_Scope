/************************************************/
/*  Utils.h	                					*/
/*												*/
/* Author	: DELTA CARE						*/
/* Date		: 06-Aug-21 2:07:09 AM				*/
/* Version	: V01								*/
/************************************************/

#ifndef UTILS_H
#define UTILS_H

#include <ctime>

class Utils
{
public:
    int getNumFrames();
    void start_time();
    void stop_time();
    void update_frame();
    double elapsed_time();
    double fps();

private:
    clock_t timeStart, timeStop;
    int numFrames = 0;

};

#endif // UTILS_H