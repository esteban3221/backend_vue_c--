#define BSD_COMP
#include <stdio.h>   
#include <string.h>  
#include <unistd.h>  
#include <fcntl.h>   
#include <errno.h>   
#include <termios.h> 
#include <sys/ioctl.h>
#include "../inc/itl_types.h"
#include "serialfunc.h"
#define FIONREAD 0x541B
SSP_PORT OpenSSPPort(const char * port)
{
	int port_handle;
	port_handle = open(port,O_RDWR| O_NOCTTY | O_NDELAY);
	if (port_handle == -1)
	{
		perror("Unable to open port");
	}
	else
	{
		fcntl(port_handle,F_SETFL,1);
		SetupSSPPort(port_handle);
	}
	return port_handle;
}

void CloseSSPPort(const SSP_PORT port)
{
	if (port >= 0)
	{
		close(port);
	}
}

int WriteData(const unsigned char * data, unsigned long length, const SSP_PORT port)
{
	long n;
	long offset;
	long bytes_left = length;
	offset = 0;
	while (bytes_left > 0)
	{
		while (TransmitComplete(port) == 0)
			usleep(100);
		n = write(port,&data[offset],bytes_left);
		usleep(500);
		if (n < 0)
		{
			perror("Write Port Failed");
			return 0;
		}
		offset += n;
		bytes_left -= n;
	}
	return 1;
}


void SetupSSPPort(const SSP_PORT port)
{
	struct termios options;
	tcgetattr(port,&options);
	
	cfsetispeed(&options,B9600);
	cfsetospeed(&options,B9600);
	
	options.c_cflag &= ~PARENB;
	options.c_cflag |= CSTOPB;

	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;

	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
        options.c_oflag &= ~OPOST;
	options.c_iflag = 0;
	options.c_iflag &= ~(IXON | IXOFF | IXANY);
	options.c_cflag &= ~CRTSCTS;


	options.c_cflag |= (CLOCAL | CREAD);
   	fcntl(port, F_SETFL, FNDELAY);
	tcsetattr(port,TCSANOW,&options);
}

int BytesInBuffer(SSP_PORT port)
{
	int bytes;
	ioctl(port,FIONREAD,&bytes);
	return bytes;
}

int TransmitComplete(SSP_PORT port)
{
    int bytes;
    ioctl(port,TIOCOUTQ,&bytes);
    return (bytes == 0);
}

int ReadData(const SSP_PORT port, unsigned char * buffer, unsigned long bytes_to_read)
{
	return read(port,buffer,bytes_to_read);
}

void SetBaud(const SSP_PORT port, const unsigned long baud)
{
	struct termios options;
	tcgetattr(port,&options);
	//9600 baud
	switch(baud)
	{
    case 9600:
        cfsetispeed(&options,B9600);
        break;
    case 19200:
        cfsetispeed(&options,B19200);
        break;
    case 38400:
        cfsetispeed(&options,B38400);
        break;
    case 57600:
        cfsetispeed(&options,B57600);
        break;
    case 115200:
        cfsetispeed(&options,B115200);
        break;
	}
	tcsetattr( port,TCSANOW,&options);
	tcgetattr(port,&options);
}