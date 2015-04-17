#include "Quadrado.h"




int main(int argc, char** argv)
{
	ros::init(argc, argv,"quadrado_node");
	Quadrado msg;
	msg.spin();

	return 0;
}

		
		



