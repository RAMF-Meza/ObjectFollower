/*
This node receives the input from the keyboard to select the direction
in which the robot will move and then publishes it in the reference_signal topic.
*/
#include<ros/ros.h>
#include<std_msgs/String.h>
#include<std_msgs/Int8.h>
#include<geometry_msgs/PointStamped.h>
#include<iostream>

int dir = 0;
std_msgs::Int8 men;

int main(int argc, char** argv)
{
    ros::init(argc, argv, "reference_node");
    ros::NodeHandle node_comms; 

    ros::Publisher topic_pub = node_comms.advertise<std_msgs::Int8>("reference_signal", 1); 
    ros::Rate loopRate(10);

    std::cout << "Select your desired direction: \n5.Stop\n 8.Forward\n 6.Right\n 4.Lef\n 2.Backward\n";
    

    while(ros::ok())
    {
        std::cin >> dir;
        std::cout << "You have selected: " << dir<< "\n";
        std_msgs::Int8 msg;
        msg.data = dir;   
        topic_pub.publish(msg);

        loopRate.sleep();
        if(dir == 9)
        {
            break;
        }
    }

    return 0;
}