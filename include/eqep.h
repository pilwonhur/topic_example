#include <stdint.h>

// p161
#define PWMSS0_ADDRESS 0x48300000
#define PWMSS1_ADDRESS 0x48302000
#define PWMSS2_ADDRESS 0x48304000

#define PWMSS0_SIZE 0x260
#define PWMSS1_SIZE 0x260
#define PWMSS2_SIZE 0x260

// eQEP Registers	//p1672
#define QPOSCNT 0x180
#define QPOSINT 0x184
#define QPOSMAX 0x188
#define QPOSCMP 0x18C
#define QPOSILAT 0x190
#define QPOSSLAT 0x194
#define QPOSLAT 0x198
#define QUTMR 0x19C
#define QUPRD 0x120

class eQEP{
private:
	uint8_t pwmss_num;
	int file;
	uint32_t *pwmss_addr;
	uint32_t pwmss_address;
	uint32_t pwmss_size;
public:
	int eqepPosition;

public:
	eQEP(unsigned int banks);
	~eQEP();
	int open();
	void readPosition();
	void initPosition(uint32_t val);
	int close();
};

