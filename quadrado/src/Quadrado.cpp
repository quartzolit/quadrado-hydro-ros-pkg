#include "Quadrado.h"

Quadrado::Quadrado():
	curr_x_( 0.0 ),
	curr_y_( 0.0 ),
	curr_phi_( 0.0 ),
	prev_phi_( 0.0 ),
	dist_moved_x_( 0.0 ),
	dist_moved_y_( 0.0 ),
	dist_rotated_( 0.0 ),
	start_x_( 0.0 ),
	start_y_( 0.0 ),
	start_phi_( 0.0 ),
	girando_(false),
	odom_set_(false)

{

	odometry_sub_ = nh_.subscribe("odom", 1, &Quadrado::odometryCallback, this);
	cmd_vel_pub_ = nh_.advertise<geometry_msgs::Twist>("cmd_vel", 1 );

}


Quadrado::~Quadrado()
{
	odometry_sub_.shutdown();
	cmd_vel_pub_.shutdown();
}


void Quadrado::setVelocity(double x, double phi)
{
	cmd_vel_msg_.linear.x=x;
	cmd_vel_msg_.angular.z=phi;

}

void Quadrado::odometryCallback( const nav_msgs::OdometryConstPtr& msg )
{
	curr_x_ = msg->pose.pose.position.x;
	curr_y_ = msg->pose.pose.position.y;
	curr_phi_ = tf::getYaw( msg->pose.pose.orientation );

	if( odom_set_ == false )
	{
		ROS_INFO( "Odometry initialized" );
		odom_set_ = true;
		prev_phi_ = curr_phi_;

		
	}

	while( curr_phi_ - prev_phi_ < PI )
	{
		curr_phi_ += 2 * PI;
	}

	while( curr_phi_ - prev_phi_ > PI )
	{
		curr_phi_ -= 2 * PI;
	}

	dist_rotated_ += curr_phi_ - prev_phi_;
	prev_phi_ = curr_phi_;
	dist_moved_x_ = curr_x_ - start_x_;
	dist_moved_y_ = curr_y_ - start_y_;

	double distance_moved_x_temp = dist_moved_x_;

	dist_moved_x_ = dist_moved_x_ * cos( -start_phi_ ) - dist_moved_y_ * sin( -start_phi_ );
	dist_moved_y_ = dist_moved_y_ * cos( -start_phi_ ) + distance_moved_x_temp * sin( -start_phi_ );

ROS_INFO("Vo X: %f  Vo em Y: %f e Vo phi: %f", dist_moved_x_, dist_moved_y_, dist_rotated_);

ROS_INFO("Start X: %f  Start em Y: %f e Start phi: %f", start_x_, start_y_, start_phi_);
ROS_INFO("Curr X: %f  Curr Y: %f e Curr phi: %f", curr_x_, curr_y_, curr_phi_);
}


void Quadrado::fazerQuadrado()
{
	/*if(!girando_) 
	{
		if(xhor(fabs(dist_moved_x_) <2  fabs(dist_moved_y_) < 2))
		{
		
			setVelocity(1,0);
		}
		else
		{
			ROS_INFO("Girando");
			setVelocity(0,0);
			girando_=true;
			start_x_ = curr_x_;
			start_y_ = curr_y_;
			start_phi_ = curr_phi_;
		}
	}
	else
	{
		if(dist_rotated_ < PI/2)
		{
			setVelocity(0,1);
		}
		else
		{
			ROS_INFO("Andando");
			setVelocity(0,0);
			girando_=false;
			start_x_ = curr_x_;
			start_y_ = curr_y_;
			start_phi_ = curr_phi_;
		}
	}*/

	

	if(!girando_)
	{
		
		if (dist_moved_x_ < 1)
		{
			setVelocity((1-dist_moved_x_),0);
		}

		else if(dist_moved_x_ >= 1 && dist_rotated_ < (PI/2))
		{
			
			setVelocity(0,(1.59-dist_rotated_));

		}

		else
		{

			setVelocity(0,0);
					
			girando_ = true;					
			
		}
			
	}
	else
	{
		start_x_ = curr_x_ ;
		start_y_ = curr_y_;
		start_phi_ = curr_phi_;
		dist_moved_x_ = 0.0;
		dist_rotated_ = 0.0;
		girando_ = false;
	}

	
	

}

void Quadrado::spin()
{
	ros::Rate loop_rate(5);
	

	while(nh_.ok())
	{

		fazerQuadrado();
		cmd_vel_pub_.publish(cmd_vel_msg_);
		ros::spinOnce();
		loop_rate.sleep();
	}

}








