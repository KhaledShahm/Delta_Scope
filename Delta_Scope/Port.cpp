/************************************************/
/*  Port.cpp									*/
/*												*/
/* Author	: DELTA CARE						*/
/* Date		: 06-Aug-21 2:07:09 AM				*/
/* Version	: V01								*/
/************************************************/


#include "Port.h"


Port::Port()
{
}


Port::~Port()
{

}


bool Port::Open(const unsigned long ulPortNo, const unsigned long ulBaundrate)
{
	TCHAR PortName[10] = { '\\','\\','.','\\','C','O','M',0,0,0 };//"\\\\.\\COM";
	TCHAR chrTemple[5] = { 0 };
	HANDLE		 hComDev[TOTAL_PORT_NUM] = { NULL };
	DCB dcb;

	if (ulPortNo >= TOTAL_PORT_NUM)
	{
		return false;
	}

	_itot(ulPortNo + START_PORT_NUM, chrTemple, 10);
	_tcscat(PortName, chrTemple);


	hComDev[ulPortNo] = CreateFile(PortName, // Serial port name 
		GENERIC_READ | GENERIC_WRITE, // Support reading and writing
		0, // Exclusive mode, serial port does not support sharing
		NULL, // Safety attribute pointer, the default value is NULL
		OPEN_EXISTING, // Open an existing serial port file
		FILE_FLAG_OVERLAPPED, // 0: synchronous mode, FILE_FLAG_OVERLAPPED: asynchronous mode
		NULL); // Used to copy the file handle, the default value is NULL, this parameter must be set to NULL for the serial port
	if (hComDev[ulPortNo] == (HANDLE)-1)
	{
		return false;
	}


	// Configure the buffer size
	if (!SetupComm(hComDev[ulPortNo], iBufferSize, iBufferSize))
	{
		return false;
	}

	// Configuration parameters
	memset(&dcb, 0, sizeof(dcb));

	if (!GetCommState(hComDev[ulPortNo], &dcb))
		return false;

	dcb.BaudRate = ulBaundrate;
	dcb.fParity = NOPARITY;
	dcb.ByteSize = 8;
	dcb.StopBits = ONESTOPBIT;

	if (!SetCommState(hComDev[ulPortNo], &dcb))
	{
		return false;
	}

	// Timeout processing, unit: milliseconds
	// Total timeout = time factor number of characters read or written + time constant
	COMMTIMEOUTS TimeOuts;
	TimeOuts.ReadIntervalTimeout = 10;		//Read interval timeout
	TimeOuts.ReadTotalTimeoutMultiplier = 1;	// Read time coefficient
	TimeOuts.ReadTotalTimeoutConstant = 1;	// Read time constant
	TimeOuts.WriteTotalTimeoutMultiplier = 50;	 // write time coefficient
	TimeOuts.WriteTotalTimeoutConstant = 20;	 // Write time constant
	SetCommTimeouts(hComDev[ulPortNo], &TimeOuts);

	PurgeComm(hComDev[ulPortNo], PURGE_TXCLEAR | PURGE_RXCLEAR); // Clear the serial port buffer

	memcpy(pHandle, &hComDev[ulPortNo], sizeof(hComDev[ulPortNo])); // Save the handle

	return true;
}


void Port::Close()
{
	HANDLE hComDev = *(HANDLE*)pHandle;
	CloseHandle(hComDev);
}


int Port::Send(const void* buf, int len)
{
	HANDLE hCom = *(HANDLE*)pHandle;

	DWORD dwBytesWrite = len; // Number of data bytes successfully written
	DWORD dwErrorFlags; // error flags
	COMSTAT comStat; // Communication status
	OVERLAPPED m_osWrite; // Asynchronous input and output structure

	// Create an event handler for OVERLAPPED, it will not be really used, but the system requires it
	memset(&m_osWrite, 0, sizeof(m_osWrite));
	m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, L"WriteEvent");

	ClearCommError(hCom, &dwErrorFlags, &comStat); // Clear communication errors and get the current status of the device
	BOOL bWriteStat = WriteFile(hCom, // Serial port handle
		buf, //The first address of the data
		dwBytesWrite, //The number of data bytes to be sent
		&dwBytesWrite, // DWORD*, used to receive and return the number of data bytes successfully sent
		&m_osWrite); // NULL means synchronous sending, OVERLAPPED* means asynchronous sending
	if (!bWriteStat)
	{
		if (GetLastError() == ERROR_IO_PENDING) // If the serial port is writing
		{
			WaitForSingleObject(m_osWrite.hEvent, 1000); // Wait for writing event for 1 second
		}
		else
		{
			ClearCommError(hCom, &dwErrorFlags, &comStat); // Clear communication errors
			CloseHandle(m_osWrite.hEvent); // Close and release hEvent memory
			return 0;
		}
	}
	return dwBytesWrite;
}


int Port::Receive(void* buf, int maxlen)
{
	HANDLE hCom = *(HANDLE*)pHandle;

	// Asynchronous
	DWORD wCount = maxlen; // Number of data bytes successfully read
	DWORD dwErrorFlags; // error flags
	COMSTAT comStat; // Communication status
	OVERLAPPED m_osRead; // Asynchronous input and output structure

	// Create an event handler for OVERLAPPED, it will not be really used, but the system requires it
	memset(&m_osRead, 0, sizeof(m_osRead));
	m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, L"ReadEvent");

	ClearCommError(hCom, &dwErrorFlags, &comStat); // Clear communication errors and get the current status of the device
	if (!comStat.cbInQue) return  0; // If the number of bytes in the input buffer is 0, return false

	BOOL bReadStat = ReadFile(hCom, // Serial port handle
		buf, //The first address of the data
		wCount, // Maximum number of bytes of data to be read
		&wCount, // DWORD*, used to receive and return the number of data bytes successfully read
		&m_osRead); // NULL means synchronous sending, OVERLAPPED* means asynchronous sending
	if (!bReadStat)
	{
		if (GetLastError() == ERROR_IO_PENDING) // If the serial port is being read
		{
			// The last parameter of the GetOverlappedResult function is set to TRUE
			//The function will wait until the read operation is completed or it returns due to an error
			GetOverlappedResult(hCom, &m_osRead, &wCount, TRUE);
		}
		else
		{
			ClearCommError(hCom, &dwErrorFlags, &comStat); // Clear communication errors
			CloseHandle(m_osRead.hEvent); // Close and release the memory of hEvent
			return 0;
		}
	}
	return wCount;
}
