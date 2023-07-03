/*
This node will obtain the raw image from the camera and then post it in the "reference_signal"\
topic.
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

int x_val{};
int y_val{};
int x_coor{};

// Azul
int HighH = 125;
int HighS = 255;
int HighV = 255;

int LowH = 100;
int LowS = 100;
int LowV = 20;

int main(int argc, char **argv)
{
    ros::init(argc, argv, "reference_node");
    ros::NodeHandle node_comms;

    ros::Publisher topic_pub = node_comms.advertise<std_msgs::Int16>("reference_signal", 1);
    ros::Rate loopRate(10);

    // std::cout << "Select your desired direction: \n5.Stop\n 8.Forward\n 6.Right\n 4.Lef\n 2.Backward\n";

          
//-------------------------------------------------------------------------------------------------

    const int width = 640;
    const int height = 480;
    const int fps = 30;

    const char *window_name = "Tracking objects";

    cv::VideoCapture input_video;

    // Abrir camara
    input_video.open(0); // Open Video

    if (!input_video.isOpened())
    {
        std::cout << "Cam not opened... \n";
        return -1;
    }

    input_video.set(cv::CAP_PROP_FRAME_WIDTH, width);
    input_video.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    input_video.set(cv::CAP_PROP_FPS, fps);

    // Leer una imagen
    cv::Mat input_frame; // Se guarda una imagen en input_frame
    cv::Mat output_frame;
    cv::Mat temporal;
    cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);

    while (ros::ok())
    {
        

        input_video.read(input_frame); // Leer la imagen guardada ES UNA FUNCION BLOQUEANTE

        std::vector<std::vector<cv::Point>> contours;

        cv::Mat imgHSV;
        cvtColor(input_frame, imgHSV, cv::COLOR_BGR2HSV);
        inRange(imgHSV, cv::Scalar(LowH, LowS, LowV), cv::Scalar(HighH, HighS, HighV), output_frame);
        findContours(output_frame, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

        for (int idx = 0; idx < (int)contours.size(); idx++)
        {
            int area = cv::contourArea(contours[idx]);
            if (area >= 4000)
            {
                std::vector<std::vector<cv::Point>> contours_new;
                // contours_new = cv::convexHull(contours, contours_new, true)

                drawContours(input_frame, contours, idx, cv::Scalar(0, 255, 0), 2);
                cv::Rect brect = cv::boundingRect(contours[idx]);
                cv::rectangle(input_frame, brect, cv::Scalar(0, 0, 255), 2);

                cv::Moments moments;
                moments = cv::moments(contours[idx]);
                cv::Point center(moments.m10 / moments.m00, moments.m01 / moments.m00);
                cv::circle(input_frame, center, 5, cv::Scalar(0, 255, 255), cv::FILLED);

                x_val = moments.m10 / moments.m00;
                y_val = moments.m01 / moments.m00;

                // std::cout <<"Center: " << center << std::endl;
                // std::cout <<"x, y: " << x_val << ", " << y_val<< std::endl;

                std::cout << "x_val: " << x_val << std::endl;

                std_msgs::Int16 msg;
                msg.data = x_val;
                topic_pub.publish(msg);
                break;
            }

        }

        imshow("Original", input_frame);
        imshow(window_name, output_frame);
        if ((cv::waitKey(1) == 'q') || (cv::waitKey(1) == 'Q'))
        {
            break;
        }
    }

    input_video.release(); // Release camera

//-------------------------------------------------------------------------------------------
        loopRate.sleep();
    

    return 0;
}