#ifndef _ROS_HEADER_
#define _ROS_DEADER_

#include <string>

//ROSの関数をmROSにマッピングする

namespace ros{

typedef class Publisher{
public:
	char topic;
	char node;
	void publish(char *data);
}Publisher;

typedef class Subscriber{
public:
	char topic;
	char node;

}Subscriber;

void init(int argc,char *argv,const char *node_name);


class NodeHandle{
	Subscriber sub;
	Publisher pub;
public:
	Subscriber subscriber(std::string topic,int queue_size,void(*fp)(std::string));
	Publisher advertise(std::string topic,int queue_size);
};

class Rate{
	int rate;
public:
	Rate(int rate){this->rate = rate;};
	void sleep();
};

void spine();
//bool ok();
//void spineOnce();

}
/*
class std_msgs{
public:
	class String{
	public:
		typedef class ConstPtr{
		public:
		std::string data;
		}ConstPtr;
	};
};
*/
void ros_info(const char c,char cc);


#endif
