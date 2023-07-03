#include<ros/ros.h>
#include<std_msgs/String.h>
#include<geometry_msgs/PointStamped.h>

void callback(const std_msgs::String &msg);

int main(int argc, char** argv)
{
    // Me registro con el maestro
    ros::init(argc, argv, "subscriber_node");
    // Crear instancia de ros
    ros::NodeHandle nodeHandle; 

    ros::Subscriber subscriber = nodeHandle.subscribe("itesm_topic", 1, callback);

    ros::Rate loopRate(10);

    int counter = 0; 
    while(ros::ok())
    {
        ros::spinOnce();
        //
    }
    //ros::spin()  //-> Solo va a mostrar lo que recibe, pero no permitira hacer nada con esa informacion
    return 0;
}

void callback(const std_msgs::String &msg)
{
    ROS_INFO(" Recibi [%s]", msg.data.c_str());
}