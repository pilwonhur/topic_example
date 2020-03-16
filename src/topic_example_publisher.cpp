#include "ros/ros.h"
#include "topic_example/myMessage.h"

int main(int argc,char **argv){
	ros::init(argc,argv,"topic_example_publisher");
	ros::NodeHandle nh;

	ros::Publisher pub=nh.advertise<topic_example::myMessage>("topic_example_msg",100);	// topic name
	ros::Rate loop_rate(10);

	int count=0;

	while(ros::ok())
	{
		topic_example::myMessage msg;
		msg.data=(float)count;

		ROS_INFO("send msg = %f", (float)count);
		pub.publish(msg);
		loop_rate.sleep();
		count++;
	}
	return 0;
}