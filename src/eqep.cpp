#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include "eqep.h"
#include <unistd.h>
#include <stdio.h>

using namespace std;


eQEP::eQEP(unsigned int pwmss) {
	this->pwmss_num=pwmss;
	this->open();
}

int eQEP::open(){
   	if(this->pwmss_num==0) {
   		this->pwmss_address = PWMSS0_ADDRESS;
   		this->pwmss_size=PWMSS0_SIZE;
   	}
   	else if(this->pwmss_num==1) {
   		this->pwmss_address = PWMSS1_ADDRESS;
   		this->pwmss_size=PWMSS1_SIZE;
   	}
   	else {
   		this->pwmss_address = PWMSS2_ADDRESS;
   		this->pwmss_size=PWMSS2_SIZE;
   	}

   	if((this->file=::open("/dev/mem",O_RDWR)) < 0){
   		perror("eQEP: failed to open the memory\n");
   		return 1;
   	}

   	//this->eqep_addr=(uint32_t *)::mmap(0,this->eqep_size,PROT_READ | PROT_WRITE, MAP_SHARED,this->file, this->eqep_address);
      this->pwmss_addr=(uint32_t *)::mmap(0,this->pwmss_size,PROT_READ | PROT_WRITE, MAP_SHARED,this->file, this->pwmss_address);
      if(this->pwmss_addr==MAP_FAILED){
         perror("eqep_addr: Failed to map the memory.\n");
      }
      
	return 0;
}

void eQEP::readPosition(){
	this->eqepPosition=this->pwmss_addr[QPOSCNT/4];
   //return this->eqepPosition;
   //cout << this->eqep_addr << endl;
}

void eQEP::initPosition(uint32_t val){
   this->pwmss_addr[QPOSINT/4]=val;
   //return this->eqepPosition;
   //cout << this->eqep_addr << endl;
}

int eQEP::close(){
	::munmap((void *)this->pwmss_addr,this->pwmss_size);	
	::close(this->file);
	this->file=-1;
}

eQEP::~eQEP(){
   if(file!=-1) this->close();
}
