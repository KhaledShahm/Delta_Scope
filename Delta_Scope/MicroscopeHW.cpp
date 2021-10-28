/************************************************/
/*  MicroscopeHW.cpp							*/
/*												*/
/* Author	: DELTA CARE						*/
/* Date		: 06-Aug-21 2:07:09 AM				*/
/* Version	: V01								*/
/************************************************/

#include "MicroscopeHW.h"



using namespace std;


MicroscopeHW::MicroscopeHW()
{
	
}


MicroscopeHW::~MicroscopeHW()
{

}

void MicroscopeHW::init(const unsigned long ulPortNo, const unsigned long ulBaundrate)
{
	// initialize Serial Port
	if (!(port.Open(ulPortNo, ulBaundrate)))
	{
		std::cout << "Fail to connect" << std::endl;
		exit(EXIT_FAILURE);
	}

}

int MicroscopeHW::get_loc(char axis)
{
	char buff[50] = { 0 };
	string command;
	string segment;

	command = "g";
	command += axes[axis];
	int len_data = 0;
	int i = 0;

	// send the command with until the mcu send back "OK" to verify that it receive the command right
	while (segment != "OK")
	{
		i++;
		memset(buff, 0, 50);
		int send_count = port.Send(command.c_str(), command.size() + 1);
		port.Receive(buff, 50);
		segment = buff[0];
		segment += buff[1];
		if (i == 5)
		{
			cout << "Not receive OK" << endl;
			return int(NULL);
		}
		Sleep(1);
	}

	segment.clear();

	switch ((buff[3]))
	{
	case 'a':
		return (int(buff[6]) - 48);
	case 'b':
		segment = buff[6];
		segment += buff[7];
		return (stoi(segment));
	case 'c':
		segment = buff[6];
		segment += buff[7];
		segment += buff[8];
		return (stoi(segment));
	case 'd':
		segment = buff[6];
		segment += buff[7];
		segment += buff[8];
		segment += buff[9];
		return (stoi(segment));
	case 'f':
		segment = buff[6];
		segment += buff[7];
		segment += buff[8];
		segment += buff[9];
		segment += buff[10];
		return (stoi(segment));
	default:
		cout << " Error Checksum Receive" << endl;
		return int(NULL);
	}
}


int MicroscopeHW::get_speed(char axis)
{
	char buff[50] = { 0 };
	string command;
	string segment;

	command = "g";
	command += axes[axis];
	command += ".cs";
	int len_data = 0;
	int i = 0;

	// send the command with until the mcu send back "OK" to verify that it receive the command right
	while (segment != "OK")
	{
		i++;
		memset(buff, 0, 50);
		int send_count = port.Send(command.c_str(), command.size() + 1);
		port.Receive(buff, 50);
		segment = buff[0];
		segment += buff[1];
		if (i == 5)
		{
			cout << "Not receive OK" << endl;
			return int(NULL);
		}
		Sleep(1);
	}

	segment.clear();

	switch ((buff[3]))
	{
	case 'a':
		return (int(buff[6]) - 48);
	case 'b':
		segment = buff[6];
		segment += buff[7];
		return (stoi(segment));
	case 'c':
		segment = buff[6];
		segment += buff[7];
		segment += buff[8];
		return (stoi(segment));
	case 'd':
		segment = buff[6];
		segment += buff[7];
		segment += buff[8];
		segment += buff[9];
		return (stoi(segment));
	case 'f':
		segment = buff[6];
		segment += buff[7];
		segment += buff[8];
		segment += buff[9];
		segment += buff[10];
		return (stoi(segment));
	default:
		cout << " Error Checksum Receive" << endl;
		return int(NULL);
	}
}


bool MicroscopeHW::move(char axis, int location)
{
	char buff[50] = { 0 };
	string command;
	int length = get_int_len(location);
	string segment;

	switch (length)
	{
	case 1:
		command = "as";
		command += axes[axis];
		command += ":";
		command += to_string(location);
		break;
	case 2:
		command = "bs";
		command += axes[axis];
		command += ":";
		command += to_string(location);
		break;
	case 3:
		command = "cs";
		command += axes[axis];
		command += ":";
		command += to_string(location);
		break;
	case 4:
		command = "ds";
		command += axes[axis];
		command += ":";
		command += to_string(location);
		break;
	case 5:
		command = "fs";
		command += axes[axis];
		command += ":";
		command += to_string(location);
		break;
	default:
		cout << " Error Checksum Send" << endl;
		return false;
		break;
	}

	// send the command with until the mcu send back "OK" to verify that it receive the command right
	while (segment != "OK")
	{
		int send_count = port.Send(command.c_str(), command.size() + 1);
		memset(buff, 0, 50);
		port.Receive(buff, 50);
		segment = buff[0];
		segment += buff[1];
		Sleep(1);
	}
	return true;
}


void MicroscopeHW::stop(char axis)
{
	this->move(axes[axis], this->get_loc(axes[axis]));
}


bool MicroscopeHW::set_speed(char axis, int speed)
{
	char buff[50] = { 0 };
	string command;
	int length = get_int_len(speed);
	string segment;
	int i = 0;


	switch (length)
	{
	case 1:
		command = "as";
		command += axes[axis];
		command += ".cs:";
		command += to_string(speed);
		break;
	case 2:
		command = "bs";
		command += axes[axis];
		command += ".cs:";
		command += to_string(speed);
		break;
	case 3:
		command = "cs";
		command += axes[axis];
		command += ".cs:";
		command += to_string(speed);
		break;
	case 4:
		command = "ds";
		command += axes[axis];
		command += ".cs:";
		command += to_string(speed);
		break;
	case 5:
		command = "fs";
		command += axes[axis];
		command += ".cs:";
		command += to_string(speed);
		break;
	default:
		cout << " Error Checksum Send" << endl;
		return false;
		break;
	}


	// send the command with until the mcu send back "OK" to verify that it receive the command right
	while (segment != "OK")
	{
		i++;
		memset(buff, 0, 50);
		int send_count = port.Send(command.c_str(), command.size() + 1);
		port.Receive(buff, 50);
		segment = buff[0];
		segment += buff[1];
		if (i == 5)
		{
			return false;
		}
		Sleep(1);
	}


	return true;
}


void MicroscopeHW::step(char axis, int dir, int step_size)
{
	int new_loc = this->get_loc(axis) + (dir * step_size);
	this->move(axis, new_loc);
}


bool MicroscopeHW::go_home(char Test)
{
	if (this->counter-- <= 0) return false;

	int directions[2] = { -1, 1 };
	int x_loc = 0;
	int y_loc = 0;
	int a_loc = 0;
	int z_loc = 0;

	if (Test == test[Motility])
	{
		std::cout << "Motility" << std::endl;
		x_loc = this->X_center_Motility + (directions[rand() % 2] * (rand() % 500));
		y_loc = this->Y_center_Motility + (directions[rand() % 2] * (rand() % 500));
		a_loc = this->A_Motility;
		z_loc = this->Z_Motility;
	}

	else if (Test == test[Morpholgy])
	{
		std::cout << "Morphology" << std::endl;
		x_loc = this->X_center_Morpholgy + (directions[rand() % 2] * (rand() % 500));
		y_loc = this->Y_center_Morpholgy + (directions[rand() % 2] * (rand() % 500));
		a_loc = this->A_Morpholgy;
		z_loc = this->Z_Morpholgy;
	}
	else return false;

	move(X, x_loc);
	move(Y, y_loc);
	move(Z, z_loc);
	move(A, a_loc);

	while (this->get_loc(X) != x_loc);
	while (this->get_loc(Y) != y_loc);
	while (this->get_loc(Z) != z_loc);
	while (this->get_loc(A) != a_loc);

	return true;
}


bool MicroscopeHW::go_Start()
{
	set_speed(X, Speed[X]);
	set_speed(Y, Speed[Y]);
	set_speed(Z, Speed[Z]);


	move(X, X_start);
	move(Y, Y_start);
	move(Z, Z_start);

	while (this->get_loc(X) != X_start);
	while (this->get_loc(Y) != Y_start);
	while (this->get_loc(Z) != Z_start);

	return true;
}


int MicroscopeHW::get_int_len(int value)
{
	int l = 1;
	while (value > 9) { l++; value /= 10; }
	return l;
}