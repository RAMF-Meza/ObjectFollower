#include<ros/ros.h>
#include<std_msgs/String.h>
#include<std_msgs/Int8.h>

#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>  // Filtros
#include <opencv2/highgui.hpp>
#include<iostream>
#include <opencv2/opencv.hpp> // cv:mat

void callback(const std_msgs::String &msg);
void imgCallback(const sensor_msgs::ImageConstPtr& msg);        // Callback para poder recibir imagenes

int main(int argc, char** argv)
{
    ros::init(argc, argv, "controller_node");
    ros::NodeHandle nodeHandle; 

    ros::Subscriber subscriber = nodeHandle.subscribe("reference_color_signal", 1, callback);
    //ros::Publisher topic_pub = nodeHandle.advertise<std_msgs::Int8>("reference_signal", 1); 
    image_transport::ImageTransport imt(nodeh);     // -> Comunicador especial para OpenCV

    // Subscriber
    image_transport::Subscriber img_sub = imt.subscribe("/camera/img_raw", 1, imgCallback);

    ros::Rate loopRate(100);

     
    while(ros::ok())
    {
        ros::spinOnce();

        //
    }
    //ros::spin()  //-> Solo va a mostrar lo que recibe, pero no permitira hacer nada con esa informacion
    return 0;
}

void callback(const std_msgs::Int8 &msg)
{
    std_msgs::Int8 color = msg.data;
    switch (color)
    {
    case 1:
        // Azul
        int HighH = 125;
        int HighS = 255;
        int HighV = 255;

        int LowH = 100;
        int LowS = 100;
        int LowV = 20;
        break;
    case 2:
        // Rojo
        int HighH = 255;
        int HighS = 255;
        int HighV = 255;

        int LowH = 37;
        int LowS = 118;
        int LowV = 173;
        break;
    case 3:
        // Verde
        int HighH = 71;
        int HighS = 255;
        int HighV = 255;

        int LowH = 37;
        int LowS = 0;
        int LowV = 0;
        break;
    case 4:
        // Rosa
        int HighH = 255;
        int HighS = 255;
        int HighV = 255;

        int LowH = 37;
        int LowS = 118;
        int LowV = 173;
        break;
    
    default:
        // Azul
        int HighH = 125;
        int HighS = 255;
        int HighV = 255;

        int LowH = 100;
        int LowS = 100;
        int LowV = 20;
        break;
    }
}

void imgCallback(const sensor_msgs::ImageConstPtr& msg)
{
    try
    {
        // Convertir la imagen recibida por el topico para poder procesarla aqui
        cv::Mat image_msg = cv_bridge::toCvShare(msg, "bgr8") ->image;  // (->) Representa que es un puntero

        cv::Mat input_frame; 
        cv::Mat output_frame;
        cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);

        input_video.read(input_frame); // Leer la imagen guardada ES UNA FUNCION BLOQUEANTE

        std::vector<std::vector<cv::Point>> contours;
        

        cv::Mat imgHSV;
        cvtColor(input_frame, imgHSV, cv::COLOR_BGR2HSV);
        inRange(imgHSV, cv::Scalar(LowH, LowS, LowV), cv::Scalar(HighH, HighS, HighV), output_frame);
        findContours(output_frame, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
        
        for(int idx = 0; idx < (int)contours.size(); idx++ )
        {
            int area = cv::contourArea(contours[idx]);
            if(area >= 4000)
            {
                std::vector<std::vector<cv::Point>> contours_new;
                //contours_new = cv::convexHull(contours, contours_new, true)
                
                drawContours(input_frame, contours, idx, cv::Scalar(0,255,0),2);
                cv::Rect brect = cv::boundingRect(contours[idx]);
                cv::rectangle(input_frame, brect, cv::Scalar(0,0,255),2);

                cv::Moments moments;
                moments = cv::moments(contours[idx]);
                cv::Point center(moments.m10/moments.m00, moments.m01/moments.m00);
                cv::circle(input_frame, center, 5, cv::Scalar(0,255,255), cv::FILLED);

                x_val = moments.m10/moments.m00;
                y_val = moments.m01/moments.m00;
                std::cout <<"Center: " << center << std::endl;
                std::cout <<"x, y: " << x_val << ", " << y_val<< std::endl;
            }
        }
        

        imshow("Original", input_frame);
        imshow(window_name, output_frame);
        cv::waitKey(1);

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}