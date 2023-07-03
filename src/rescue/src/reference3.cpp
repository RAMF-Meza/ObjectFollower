/*
This node will obtain the raw image from the camera and then post it in the "reference_signal"\
topic.
Also, the node will get the color of the object to be segmented.
It publishes and subscribes in the same node
*/
#include<ros/ros.h>
#include<std_msgs/Int8.h>
#include<iostream>

#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>  // Filtros
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp> // cv:mat


int color{};

void imgCallback(const sensor_msgs::ImageConstPtr& msg);

int main(int argc, char** argv)
{
    ros::init(argc, argv, "reference_node");
    ros::NodeHandle node_comms; 
    // Publisher color
    ros::Publisher color_pub = node_comms.advertise<std_msgs::Int8>("reference_color_signal", 1); 
    std::cout << "Select the color of the object:\n1.Azul\n 2.Rojo\n 3.Verde\n 4.Rosa\n";
    
    image_transport::ImageTransport video(node_comms);     // -> Comunicador especial para OpenCV
    // Subscriber
    image_transport::Subscriber img_sub = video.subscribe("/camera/img_raw", 1, imgCallback);

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

    //Publisher video
    image_transport::Publisher pub_rgb = video.advertise("/camera/img_raw", 1);

    ros::Rate loopRate(10);

    while(ros::ok())
    {   
        ros::spinOnce();
        std::cin >> color;
        std::cout << "You have selected: " << color<< "\n";
        std_msgs::Int8 msg;
        msg.data = color;   
        color_pub.publish(msg);

        input_video.read(cvi.image);
        cvi.header.stamp = ros::Time::now();
        pub_rgb.publish(cvi.toImageMsg());

        loopRate.sleep();
        if(color == 5)
        {
            break;
        }
    }

    return 0;
}

void imgCallback(const sensor_msgs::ImageConstPtr& msg)
{
        // Azul
    int HighH = 125;
    int HighS = 255;
    int HighV = 255;

    int LowH = 100;
    int LowS = 100;
    int LowV = 20;

    int x_val{};
    int y_val{};

    const char *window_name = "Tracking objects";
    try
    {
        // Convertir la imagen recibida por el topico para poder procesarla aqui
        cv::Mat image_msg = cv_bridge::toCvShare(msg, "bgr8") ->image;  // (->) Representa que es un puntero

            // En esta seccion se implementarian los filtros, procesamientos, etc.
            // Leer una imagen
        cv::Mat input_frame; // Se guarda una imagen en input_frame
        cv::Mat output_frame;
        cv::Mat temporal;
        cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);

        while (1)
        {
            //image_msg.read(input_frame); // Leer la imagen guardada ES UNA FUNCION BLOQUEANTE

            std::vector<std::vector<cv::Point>> contours;
            

            cv::Mat imgHSV;
            cvtColor(image_msg, imgHSV, cv::COLOR_BGR2HSV);
            inRange(imgHSV, cv::Scalar(LowH, LowS, LowV), cv::Scalar(HighH, HighS, HighV), output_frame);
            findContours(output_frame, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
            
            for(int idx = 0; idx < (int)contours.size(); idx++ )
            {
                int area = cv::contourArea(contours[idx]);
                if(area >= 4000)
                {
                    std::vector<std::vector<cv::Point>> contours_new;
                    //contours_new = cv::convexHull(contours, contours_new, true)
                    
                    drawContours(image_msg, contours, idx, cv::Scalar(0,255,0),2);
                    cv::Rect brect = cv::boundingRect(contours[idx]);
                    cv::rectangle(image_msg, brect, cv::Scalar(0,0,255),2);

                    cv::Moments moments;
                    moments = cv::moments(contours[idx]);
                    cv::Point center(moments.m10/moments.m00, moments.m01/moments.m00);
                    cv::circle(image_msg, center, 5, cv::Scalar(0,255,255), cv::FILLED);

                    x_val = moments.m10/moments.m00;
                    y_val = moments.m01/moments.m00;
                    std::cout <<"Center: " << center << std::endl;
                    std::cout <<"x, y: " << x_val << ", " << y_val<< std::endl;
                }
            }
            

            imshow("Original", image_msg);
            imshow(window_name, output_frame);
            if((cv::waitKey(1) == 'q') || (cv::waitKey(1) == 'Q'))
            {
                break;
            }
        }

        image_msg.release(); // Release camera
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}