#include <ros.h>
#include <std_msgs/Float64.h>

// Create a ROS node handle
ros::NodeHandle nh;

// Messages
std_msgs::Float64 right_feedback_msg;
std_msgs::Float64 left_feedback_msg;


// Publishers
ros::Publisher pub_right("right_wheel_feedback", &right_feedback_msg);
ros::Publisher pub_left("left_wheel_feedback", &left_feedback_msg);



// Float64 message variables to store the received data
float right_wheel_value = 0.0;
float left_wheel_value = 0.0;



// Callback function for the right wheel command
void rightWheelCallback(const std_msgs::Float64& msg) {
  right_wheel_value = (msg.data);
  // You can add motor control logic here
  //Serial1.print("Right Wheel Command: ");
  // Serial1.println(right_wheel_value);
}

// Callback function for the left wheel command
void leftWheelCallback(const std_msgs::Float64& msg) {
  left_wheel_value = (msg.data);
  // You can add motor control logic here
  //Serial1.print("Left Wheel Command: ");
  //Serial1.println(left_wheel_value);
}

// Create subscribers
ros::Subscriber<std_msgs::Float64> sub_right("right_wheel_command", &rightWheelCallback);
ros::Subscriber<std_msgs::Float64> sub_left("left_wheel_command", &leftWheelCallback);

void setup() {
  Serial.begin(57600);  // Make sure this matches the baud rate used in rosserial
  Serial1.begin(57600);
  nh.initNode();
  nh.subscribe(sub_right);
  nh.subscribe(sub_left);
  nh.advertise(pub_right);
  nh.advertise(pub_left);
}

void loop() {
  int speedLeftCmd = left_wheel_value;    // desired left speed
  int speedRightCmd = right_wheel_value;  // desired right speed

  // Send speeds to Uno (format: "LxxxRxxx")
  Serial1.print("L");
  Serial1.print(speedLeftCmd);
  Serial1.print("R");
  Serial1.print(speedRightCmd);
  Serial1.println();

  if (Serial1.available()) {
    String feedback = Serial1.readStringUntil('\n');
    if (feedback.startsWith("FB")) {
      int lPos = feedback.indexOf('L');
      int rPos = feedback.indexOf('R');
      if (lPos >= 0 && rPos >= 0) {
        // Parse feedback values
        right_feedback_msg.data = feedback.substring(rPos + 1).toFloat();       // Right motor (Rxxx)
        left_feedback_msg.data = feedback.substring(lPos + 1, rPos).toFloat();  // Left motor (Lxxx)

        // Publish to ROS
        pub_right.publish(&right_feedback_msg);
        pub_left.publish(&left_feedback_msg);
      }
    }
  }

  nh.spinOnce();  // Handle callbacks
  delay(10);
}