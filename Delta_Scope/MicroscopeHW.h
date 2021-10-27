/************************************************/
/*  MicroscopeHW.h								*/
/*												*/
/* Author	: DELTA CARE						*/
/* Date		: 06-Aug-21 2:07:09 AM				*/
/* Version	: V01								*/
/************************************************/

#ifndef __MICROSCOPEHW_H
#define __MICROSCOPEHW_H

#include<Windows.h>
#include"Port.h"
#include <iostream>
#include <string>

#define	Number_axis		4

#define		X			0
#define		Y			1
#define		Z			2
#define		A			3


#define Number_test		2

#define Motility		0
#define Morpholgy		1


class MicroscopeHW
{
public:
	/*
	*  Constractor Function:  MicroScope
	*  --------------------
	*	initialize serial port
	*
	*	Cam_Id: Id camera number in char like "1"
	*	ulPortNo: com port number like 2
	*	baud: baud rate of the serial port like 9600, 57600, ..
	*
	*  returns: void
	*/
	MicroscopeHW(const unsigned long ulPortNo, const unsigned long ulBaundrate);

	/*
	*  Destructor Function: MicroScope
	*  --------------------
	*  Return to homming
	*
	*/
	~MicroscopeHW();


	/*
	* Function: get_loc
	* --------------------
	*
	* receive from the MCU/serial the location for specific axis
	*
	* axis:	X, Y, Z, A
	*
	*  returns: location in miccrometer (um) for x,y, 0 if failed
	*/
	int get_loc(char axis);


	/*
	* Function: get_speed
	* --------------------
	*
	* receive from the MCU/serial the speed for specific axis
	*
	* axis:	X, Y, Z, A
	*
	returns: location in miccrometer (um) for x,y, 0 if failed
	*/
	int get_speed(char axis);


	/*
	* Function: move
	* --------------------
	*
	* move to specific location for specific axis
	*
	* axis: X, Y, Z, A
	* location: [X: 0 to 10000], [Y: 0 to 10000],[Z: 0 to 10000],[A: 0 to 10000]
	*
	*  returns: returns: true if successful, false if failed
	*/
	bool move(char axis, int location);


	/*
	* Function:  stop
	* --------------------
	*
	* stop specific motor
	*
	* axis: X, Y, Z, A
	*
	*  returns: void
	*/
	void stop(char axis);


	/*
	* Function:  set_speed
	* --------------------
	*
	*	set Speed to the MCU/serial the speed for specific axis
	*
	*	axis:	X, Y, Z, A
	*	speed:	in range 1 to 120 (1 maximum speed)
	*
	*	returns: False or True for success operation
	*/
	bool set_speed(char axis, int speed);


	/*
	* Function:  step
	* --------------------
	*
	* move by step size
	*
	*  returns: void
	*/
	void step(char axis, int dir = 1, int step_size = 0);


	/*
	* Function:  go_home
	* --------------------
	*
	* go at specifi location to start auto_focus
	*
	*  returns: True or False for faild auto_focus u must change your slice
	*/
	bool go_home(char Test);


	/*
	* Function:  go_Start
	* --------------------
	*
	*  go at specifi location to put your slice
	*
	*  returns: True or False for faild go_Start to check your conection
	*/
	bool go_Start();


	char axes[Number_axis] = { 'X', 'Y', 'Z', 'A' };
	char test[Number_test] = { Motility , Morpholgy };

private:

	char Speed[Number_axis] = { 1 , 1 , 1 , 1 };

	int get_int_len(int value);

	// Positions of Starting
	const int X_start = 20000;
	const int Y_start = 50000;
	const int Z_start = 1000;

	// Position of Starting Slice Motility
	const int X_center_Motility = 14000;
	const int Y_center_Motility = 20000;
	const int A_Motility = 8000;
	const int Z_Motility = 20000;


	// Position of Starting Slice Morpholgy
	const int X_center_Morpholgy = 34500;
	const int Y_center_Morpholgy = 20000;
	const int A_Morpholgy = 19000;
	const int Z_Morpholgy = 20000;


	//port
	Port port;

	int  counter = 5;

};

#endif  /* __MICROSCOPEHW_H */