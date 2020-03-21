#include "uart.h"
#include <stdio.h>
#include <cstdlib>
#include <cstring>  // for strcpy
#include <fcntl.h>
#include <unistd.h>
#include <fstream>
#include <ctype.h>

char *float2str(float num) {
    static char retnum[21];       // Enough for 20 digits plus NUL from a 64-bit uint.
    sprintf(retnum, "%.4f\n", num);
    return retnum;
}

char *double2str(double num) {
    static char retnum[21];       // Enough for 20 digits plus NUL from a 64-bit uint.
    sprintf(retnum, "%.4f\n", num);
    return retnum;
}

char *int2str(int num) {
    static char retnum[21];       // Enough for 20 digits plus NUL from a 64-bit uint.
    sprintf(retnum, "%d\n", num);
    return retnum;
}

char *string2str(string data) {
    static char retnum[21];       // Enough for 20 digits plus NUL from a 64-bit uint.
    sprintf(retnum, "%s\n", data.c_str());
    return retnum;
}

int numchar(char* ptr){
    for(int i=0;i<DATASIZE;i++)
    {
        if(*(ptr+i)=='\n')
        return i+1;
    }
    return DATASIZE;
}

UART::UART(char* uname, unsigned int unum, speed_t Baud) {
    printf("UART constructred!\n");
    strcpy(uartname, uname);    // char* cannot be simply assigned to char[]. Should use strcpy or memcpy
    uartnum=unum;
    baudRate=Baud;
    sprintf(uartdevice,"%s%d",uname,unum);
    // "/dev/ttyO1"
    this->open();
}

void UART::printVar(){
    printf("device name: %s\n",uartdevice);
}

// The leading :: just means that "open" here refers to a non-member function in the global namespace, instead of referring to UART::open in the current namespace.

// Baud
// https://docs.rs/termios/0.2.2/termios/os/linux/index.html#structs
// https://docs.rs/termios/0.2.2/termios/os/linux/type.speed_t.html
// Const   val(speed_t)
// B0          0           B57600      4097
// B50         1           B115200     4098
// B75         2           B230400     4099
// B110        3           B460800     4100
// B134        4           B500000     4101
// B150        5           B576000     4102
// B200        6           B921600     4103
// B300        7           B1000000    4104
// B600        8           B1152000    4105
// B1200       9           B1500000    4106
// B1800       10          B2000000    4107
// B2400       11          B2500000    4108
// B4800       12          B3000000    4109
// B9600       13          B3500000    4110
// B19200      14          B4000000    4111
// B38400      15
int UART::open(){
    if((file=::open(uartdevice,O_RDWR))<0)
    {
        perror("UART: Failed to open the file.\n");
        return 1;
    }
    struct termios options;
    tcgetattr(this->file,&options);
    options.c_cflag=baudRate|CS8|CREAD|CLOCAL;   // B1000000, B115200, B230400, B460800, B500000, B1000000
    options.c_iflag=IGNPAR|ICRNL;
    tcflush(this->file,TCIFLUSH);
    tcsetattr(this->file,TCSANOW,&options);
//    cout << cfgetispeed(&options) << endl;
    return 0;
}

int UART::write(float data){
    char* ptr=float2str(data);
    int numlength=numchar(ptr);
    // printf("%s",ptr);
    if(::write(this->file, ptr, numlength)!=numlength){
        perror("UART: Failed to write to the device\n");
        return 1;
    }
   return 0;
}

int UART::write(double data){
    char* ptr=double2str(data);
    int numlength=numchar(ptr);
    // printf("%s",ptr);
    if(::write(this->file, ptr, numlength)!=numlength){
        perror("UART: Failed to write to the device\n");
        return 1;
    }
   return 0;
}

int UART::write(int data){
    char* ptr=int2str(data);
    int numlength=numchar(ptr);
    // printf("%s",ptr);
    if(::write(this->file, ptr, numlength)!=numlength){
        perror("UART: Failed to write to the device\n");
        return 1;
    }
   return 0;
}

int UART::write(string data){
    char* ptr=string2str(data);
    int numlength=numchar(ptr);
    // printf("%s",ptr);
    if(::write(this->file, ptr, numlength)!=numlength){
        perror("UART: Failed to write to the device\n");
        return 1;
    }
   return 0;
}

int UART::write(char* ptr){
    int numlength=numchar(ptr);
    // printf("%s",ptr);
    if(::write(this->file, ptr, numlength)!=numlength){
        perror("UART: Failed to write to the device\n");
        return 1;
    }
   return 0;
}

int UART::dataAvailable(){
    if((this->count=::read(this->file,(void*)(this->received),DATASIZE))<0)
    {
        perror("Failed to read from the input\n");
    }
    return this->count;
}

string UART::read(){
    int num=numchar((char*)this->received);
    string str((char *)received);
    // cout << num << ", " << this->count << ", " << str.substr(0, num-1) << endl;
    // print("%s",this->received);
    return str.substr(0, num-1);
}
    
void UART::close(){
  ::close(this->file);
  this->file = -1;
}

UART::~UART() {
  if(file!=-1) this->close();
}
