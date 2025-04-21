#include <ros.h>
#include <std_msgs/Float64.h>

// Node handle
ros::NodeHandle nh;

// Messages
std_msgs::Float64 right_feedback_msg;
std_msgs::Float64 left_feedback_msg;

// Publishers
ros::Publisher pub_right("right_wheel_feedback", &right_feedback_msg);
ros::Publisher pub_left("left_wheel_feedback", &left_feedback_msg);

void setup() {
  Serial.begin(57600);  // match with rosserial
  nh.initNode();
  nh.advertise(pub_right);
  nh.advertise(pub_left);
}

void loop() {
  // Dummy feedback values (replace with encoder readings)
  right_feedback_msg.data = analogRead(A0) * (5.0 / 1023.0);  // example value
  left_feedback_msg.data = analogRead(A1) * (5.0 / 1023.0);   // example value

  pub_right.publish(&right_feedback_msg);
  pub_left.publish(&left_feedback_msg);

  nh.spinOnce();
  delay(100);  // publish rate ~10Hz
}
