#include "ros/ros.h"
#include "topic_example/myMessage.h"

#include <iostream>
#include <sstream>
#include "SPIDevice.h"


int main(int argc,char **argv){
	ros::init(argc,argv,"topic_example_publisher");
	ros::NodeHandle nh;

	ros::Publisher pub=nh.advertise<topic_example::myMessage>("topic_example_msg",100);	// topic name
	ros::Rate loop_rate(200);

	int count=0;
	
	SPIDevice *busDevice = new SPIDevice(1,0); //Using second SPI bus (both loaded)
   	busDevice->setSpeed(4000000);      // Have access to SPI Device object
   	busDevice->setMode(SPIDevice::MODE0);

	unsigned char send[9], receive[9];


	while(ros::ok())
	{
		topic_example::myMessage msg;
		msg.data=(float)count;
		
		sprintf((char *)send,"%f",0.01);
		busDevice->transfer(send, receive, 9);

//		write(fd,&a,1); // write a single a character
//		fflush(stdout); // need to flush the output, as no

		
//		if(count%200==0)
//			ROS_INFO("send msg = %f", (float)count);

		pub.publish(msg);
		count++;
		loop_rate.sleep();
	}
	return 0;
}
