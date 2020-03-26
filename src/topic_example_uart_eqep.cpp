#include "ros/ros.h"
#include "topic_example/myMessage.h"

#include <iostream>
#include <sstream>
#include "uart.h"
#include <cmath>
#include "eqep.h"

#define ENCCOUNT1 1632 
#define ENCCOUNT2 979.2
#define PI 3.141592
#define KP 50
#define KD 1
//5, 1.5

// for motor1, KP5(20,..), KD1.5, filtered: u
// for motor2, KP50, KD1, filtered: err
int main(int argc,char **argv){
	ros::init(argc,argv,"topic_example_publisher");
	ros::NodeHandle nh;

	ros::Publisher pub1=nh.advertise<topic_example::myMessage>("topic_example_error",8);	// topic name
	ros::Publisher pub2=nh.advertise<topic_example::myMessage>("topic_example_current",8);
	ros::Rate loop_rate(1000);

	int count=0;
	float curAngle=0.0;
	double u=0.0;
	double uprev=0.0;
	
	UART uart1((char*)"/dev/ttyO",1,B1000000);

	eQEP eqep1(2);
  	eqep1.open();
	eqep1.initPosition(0);
	eqep1.readPosition();
	int eqepOffset=eqep1.eqepPosition;
	double err=0.0;
	double errPrev=0.0;
	double desTraj=0.0;
	double errdot=0.0;
	double errdotPrev=0.0;
	string tempStr;	

	while(ros::ok())
	{
		topic_example::myMessage msg1, msg2;
		
		eqep1.readPosition();
		curAngle=(eqep1.eqepPosition-eqepOffset)/ENCCOUNT2*2*PI;

		// round to 100th
		// curAngle=floor(curAngle*100+0.5)/100.0;


		if(count%1000==0)
	    		cout << "angle: " <<  curAngle << " rad" << endl;

		desTraj=sin(count/1000.0*PI)*PI/2*7/9;
		// desTraj=0.0;

		err=-(desTraj-curAngle);
		err=1/11.0*err+10/11.0*errPrev; // LPF
		errdot=err-errPrev;
		//errdot=1.0/10.0*errdot+9.0/10.0*errdotPrev; //LPF
		errdotPrev=errdot;
		u=KP*err+KD*errdot*1000.0;
		errPrev=err;
		//u=1/11.0*u+10/11.0*uprev; // LPF
		uprev=u;
		uart1.write((float)u);
		
/*
		if(uart1.dataAvailable())
		{
			tempStr=uart1.read();
			if((int)stof(tempStr)%1000==0)
				printf("%s\n",tempStr.c_str());	// If I/O or screen out is performed, performance slows down.
		}
*/

//		if(count%200==0)
//			ROS_INFO("send msg = %f", (float)count);

		msg1.data=(float)err;
		msg2.data=(float)u;
		pub1.publish(msg1);
		pub2.publish(msg2);
		count++;
		loop_rate.sleep();
	}
	uart1.close();
	return 0;
}
