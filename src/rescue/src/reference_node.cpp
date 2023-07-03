/*
This node will obtain the raw image from the camera and then post it in the topic.
*/
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int16.h>
#include <geometry_msgs/PointStamped.h>
#include <iostream>

#include <opencv2/core.hpp>    // Video Capture
#include <opencv2/imgproc.hpp> // Filtros
#include <opencv2/highgui.hpp>
#include <iostream>
#include <opencv2/opencv.hpp> // cv:mat


int main(int argc, char **argv)
{
    ros::init(argc, argv, "reference_node");
    ros::NodeHandle node_comms;

    image_transport::ImageTransport imt(nodeh);     // -> Comunicador especial para OpenCV

    ros::Publisher topic_pub = node_comms.advertise<std_msgs::Int16>("reference_signal", 1);
    
    cv::VideoCapture input_video;
    input_video.open(0);       // Open Video

    if(!input_video.isOpened())
    {
        ROS_ERROR("Could not open camera");
        ros::shutdown();
    }

    cv_bridge::CvImage cvi;
    cvi.header.frame_id = "RGB";        // Se da el nombre del tipo de mensaje
    cvi.encoding = sensor_msgs::image_encodings::BGR8;

    //Publisher
    image_transport::Publisher pub_rgb = imt.advertise("/camera/img_raw", 1);
    ros::Rate loopRate(30);

    while (ros::ok())
    {
        ros::spinOnce();
        input_video.read(cvi.image);
        cvi.header.stamp = ros::Time::now();
        pub_rgb.publish(cvi.toImageMsg());
//-------------------------------------------------------------------------------------------
        loopRate.sleep();
    }

    return 0;
}