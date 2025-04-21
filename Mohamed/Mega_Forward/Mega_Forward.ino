#include <ros.h>
#include <std_msgs/Float64.h>

// Create a ROS node handle
ros::NodeHandle nh;

// Float64 message variables to store the received data
float right_wheel_value = 0.0;
float left_wheel_value = 0.0;

// Callback function for the right wheel command
void rightWheelCallback(const std_msgs::Float64& msg) {
  right_wheel_value = msg.data;
  // You can add motor control logic here
  Serial1.print("Right Wheel Command: ");
  Serial1.println(right_wheel_value);
}

// Callback function for the left wheel command
void leftWheelCallback(const std_msgs::Float64& msg) {
  left_wheel_value = msg.data;
  // You can add motor control logic here
  Serial1.print("Left Wheel Command: ");
  Serial1.println(left_wheel_value);
}

// Create subscribers
ros::Subscriber<std_msgs::Float64> sub_right("right_wheel_command", &rightWheelCallback);
ros::Subscriber<std_msgs::Float64> sub_left("left_wheel_command", &leftWheelCallback);

void setup() {
  Serial.begin(57600);  // Make sure this matches the baud rate used in rosserial
  Serial1.begin(9600);
  nh.initNode();
  nh.subscribe(sub_right);
  nh.subscribe(sub_left);
}

void loop() {
  nh.spinOnce();  // Handle callbacks
  delay(10);
}
