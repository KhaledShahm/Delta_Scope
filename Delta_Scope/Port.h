/************************************************/
/*  Port.h										*/
/*												*/
/* Author	: DELTA CARE						*/
/* Date		: 06-Aug-21 2:07:09 AM				*/
/* Version	: V01								*/
/************************************************/

#ifndef 	PORT_H
#define		PORT_H

#define		TOTAL_PORT_NUM		65
#define		START_PORT_NUM		0

#define		iBufferSize 250
#define     UARTBufferLength 2000

#include <windows.h>
#include <tchar.h>
class Port
{
public:
	Port();
	~Port();


	/*
	* Function:  Open
	* --------------------
	* Open Serial Port
	*
	* Paramters:
	* PortNo(serial port name): "1" "2" etc. under Windows, "/dev/ttyS1" etc. under Linux
	* Baundrate (baud rate): 9600, 19200, 38400, 43000, 56000, 57600, 115200
	*
	*  returns: true if successful, false if failed
	*/
	bool Open(const unsigned long ulPortNo, const unsigned long ulBaundrate);


	/*
	* Function:  Close
	* --------------------
	*
	* Close the serial port, parameters to be determined
	*
	*  returns: true if successful, false if failed
	*/
	void Close();


	/*
	* Function: Send
	* --------------------
	*
	*  Send data or write data
	*
	* Paramters:
	*	const void* buf: Data that u need to write it
	*	len : length of data
	*
	*  returns: the length of the sent data successfully, OR 0 if it fails
	*/
	int Send(const void* buf, int len);


	/*
	* Function: Receive
	* --------------------
	*
	*  Accept data or Read data
	*
	* Paramters:
	*	void* buf: Buffer for data that u need to receive it
	*	maxlen : Maximum length of data
	*
	*  returns: the length of the actual data read successfully, OR 0 if it fails
	*/
	int Receive(void* buf, int maxlen);


private:
	int pHandle[16];
};

#endif // PORT_H