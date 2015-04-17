#ifndef Quadrado_H_
#define Quadrado_H_

#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>
#include <tf/transform_datatypes.h>



#define PI 3.14159
 

class Quadrado
{
public:
	Quadrado();
	~Quadrado();
	void spin();

private:


	double curr_x_, curr_y_, curr_phi_, prev_phi_;
	double dist_moved_x_, dist_moved_y_, dist_rotated_;
	double start_x_, start_y_, start_phi_;

	bool odom_set_;


	ros::NodeHandle nh_;
	ros::Subscriber odometry_sub_; 
	ros::Publisher cmd_vel_pub_;


	geometry_msgs::Twist cmd_vel_msg_;
	nav_msgs::Odometry current_odom_msg_;
	nav_msgs::Odometry start_odom_msgs_;

	double setVelocity (double x, double phi);
	void odometryCallback(const nav_msgs::OdometryConstPtr& msg);
	void fazerQuadrado();
};




#endif /* Quadrado_H_ */
