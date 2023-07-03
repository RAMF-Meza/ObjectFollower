#include<ros/ros.h>


int main(int argc, char** argv)
{
    // Me registro con el maestro
    ros::init(argc, argv, "hello_world");
    // Crear instancia de ros
    ros::NodeHandle nodeHandle; 

    ros::Rate loopRate(10);

    int counter = 0;
    while(ros::ok())
    {
        ROS_INFO_STREAM("Hello world: " << counter);
        counter++;
        //
        loopRate.sleep();
    }

    return 0;
}