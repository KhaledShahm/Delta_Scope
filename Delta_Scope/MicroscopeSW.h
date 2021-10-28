/************************************************/
/*  MicroscopeHW.h								*/
/*												*/
/* Author	: DELTA CARE						*/
/* Date		: 06-Aug-21 2:07:09 AM				*/
/* Version	: V01								*/
/************************************************/

#ifndef __MICROSCOPESW_H
#define __MICROSCOPESW_H

#include "Cam_Stream.h"
#include "MicroscopeHW.h"


class MicroscopeSW
{
public:
	Mat*			frame;

	MicroscopeSW(const unsigned long CAM_ID,const unsigned long ulPortNo, const unsigned long ulBaundrate);
	~MicroscopeSW();
	
	void move_up();
	void move_down();
	void move_right();
	void move_left();

	void auto_focus_morpholgy();

	void auto_focus_motility();

private:
	
	void scan_slice_motility();
	
	void scan_slice_morpholgy();
	
	void get_image();
	
	void save_video();
	
	void init(const unsigned long CAM_ID, const unsigned long ulPortNo, const unsigned long ulBaundrate);

	MicroscopeHW	HW_Scope;
	Cam_Stream		CS;
};

#endif