// SUbscriber de OpenCV

#include <ros/ros.h>

#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>  // Filtros
#include <opencv2/highgui.hpp>
#include<iostream>
#include <opencv2/opencv.hpp> // cv:mat

void imgCallback(const sensor_msgs::ImageConstPtr& msg);        // Callback para poder recibir imagenes

int main(int argc, char** argv)
{
    ros::init(argc, argv, "opencv_ros");
    ros::NodeHandle nodeh;

    image_transport::ImageTransport imt(nodeh);     // -> Comunicador especial para OpenCV

    // Subscriber
    image_transport::Subscriber img_sub = imt.subscribe("/topic_name", 1, imgCallback);

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
    while(ros::ok())
    {
        ros::spinOnce();
        input_video.read(cvi.image);
        cvi.header.stamp = ros::Time::now();
        pub_rgb.publish(cvi.toImageMsg());
        loopRate.sleep();
    }


    return 0;
}

void imgCallback(const sensor_msgs::ImageConstPtr& msg)
{
    try
    {
        // Convertir la imagen recibida por el topico para poder procesarla aqui
        cv::Mat image_msg = cv_bridge::toCvShare(msg, "bgr8") ->image;  // (->) Representa que es un puntero

        // En esta seccion se implementarian los filtros, procesamientos, etc.
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}