#include <stdint.h>
#include <string>
#include <iostream>
#include <termios.h>

#define DATASIZE 100
// #define BBB_I2C_0 "/dev/i2c-0"

using namespace std;

class UART{

private:
    char uartname[20];         // don't use char* uartname specifically for the arm device. memory management issue to avoid buffer overflow.
    unsigned int uartnum;
    char uartdevice[20];    // don't use char* uartdevice specifically for the arm device. memory management issue.
                            // https://stackoverflow.com/questions/7184227/sprintf-segmentation-fault
    speed_t baudRate;
    int file;
    int count;
    unsigned char received[DATASIZE];

public:
    // Variables to hold latest sensor data values

public:
    UART(char* uname, unsigned int unum, speed_t Baud);
    ~UART();
    int open();
    int write(float data);
    int write(double data);
    int write(int data);
    int write(string data);
    int write(char* ptr);
    int dataAvailable();
    string read();
    void close();
    void printVar();
};