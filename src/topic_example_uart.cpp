#include "ros/ros.h"
#include "topic_example/myMessage.h"

#include <iostream>
#include <sstream>
#include "uart.h"
#include <cmath>
#define PI 3.141592

int main(int argc,char **argv){
	ros::init(argc,argv,"topic_example_publisher");
	ros::NodeHandle nh;

	ros::Publisher pub=nh.advertise<topic_example::myMessage>("topic_example_msg",100);	// topic name
	ros::Rate loop_rate(1000);

	int count=0;
	
	UART uart1((char*)"/dev/ttyO",1,B1000000);


	
	string tempStr;	

	while(ros::ok())
	{
		topic_example::myMessage msg;
		msg.data=(float)count;
		
		double tempVal=sin(count/1000.0*PI)*0.3;
		uart1.write((float)tempVal);
		
		if(uart1.dataAvailable())
		{
			tempStr=uart1.read();
			if((int)stof(tempStr)%1000==0)
				printf("%s\n",tempStr.c_str());	// If I/O or screen out is performed, performance slows down.
		}

//		if(count%200==0)
//			ROS_INFO("send msg = %f", (float)count);

		pub.publish(msg);
		count++;
		loop_rate.sleep();
	}
	uart1.close();
	return 0;
}
