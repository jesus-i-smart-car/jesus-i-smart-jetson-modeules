#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>

// create the TeleopTurtle class and define the joyCallback function that will take a joy msg
class TeleopTurtle
{
public:
  TeleopTurtle();

private:
  void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);

  ros::NodeHandle nh_;

  int linear_, angular_;   // used to define which axes of the joystick will control our turtle
  int l_scale_adj_, a_scale_adj_;
  double l_scale_, a_scale_;
  double max_linear_, max_angular_;
  ros::Publisher vel_pub_;
  ros::Subscriber joy_sub_;

};

TeleopTurtle::TeleopTurtle(): linear_(1), angular_(3), l_scale_(2), a_scale_(0.5), max_linear_(4), max_angular_(3.1415)
{
  //  initialize some parameters
  nh_.param("axis_linear", linear_, linear_);  
  nh_.param("axis_angular", angular_, angular_);
  nh_.param("max_linear", max_linear_, max_linear_);  
  nh_.param("max_angular", max_angular_, max_angular_);
  nh_.param("scale_angular", a_scale_, a_scale_);
  nh_.param("scale_linear", l_scale_, l_scale_);
  nh_.param("scale_angular_adj", a_scale_adj_, a_scale_adj_); // angular scale change
  nh_.param("scale_linear_adj", l_scale_adj_, l_scale_adj_);     // linear scale change 

  // create a publisher that will advertise on the command_velocity topic of the turtle
  vel_pub_ = nh_.advertise<geometry_msgs::Twist>("/cmd_vel/xbox360", 1);

  // subscribe to the joystick topic for the input to drive the turtle
  joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("joy", 10, &TeleopTurtle::joyCallback, this);
}

void TeleopTurtle::joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{
  geometry_msgs::Twist twist;
  // take the data from the joystick and manipulate it by scaling it and using independent axes to control the linear and angular velocities of the turtle
  l_scale_  += 0.1*joy->axes[l_scale_adj_];
  a_scale_  -= 0.01*joy->axes[a_scale_adj_];  twist.angular.z = a_scale_*joy->axes[angular_];
  twist.linear.x = l_scale_*joy->axes[linear_];

  a_scale_ = (a_scale_>= max_angular_) ? max_angular_ : a_scale_;
  a_scale_ = (a_scale_ <= 0.01) ? 0.01 : a_scale_;

  l_scale_ = (l_scale_ >= max_linear_) ? max_linear_ : l_scale_;
  l_scale_ = (l_scale_ <= 0.1) ? 0.1 : l_scale_;
/*
  for(int i=0;i<8;i++)
  {
    printf("%d %6.3lf \n",i,joy->axes[i]);   // 채널 확인용
  }
  */
//printf("%6.5lf %6.5lf  %6.5lf %6.5lf \n",l_scale_,a_scale_,  twist.linear.x , twist.angular.z );
  vel_pub_.publish(twist);
}

int main(int argc, char** argv)
{
  // initialize our ROS node, create a teleop_turtle, and spin our node 
    // until Ctrl-C is pressed
  ros::init(argc, argv, "teleop_turtle");
  TeleopTurtle teleop_turtle;

  ros::spin();
}
