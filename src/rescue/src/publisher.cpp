#include<ros/ros.h>
#include<std_msgs/String.h>
#include<geometry_msgs/PointStamped.h>



int main(int argc, char** argv)
{
    // Me registro con el maestro
    ros::init(argc, argv, "publisher_node");
    // Crear instancia de ros
    ros::NodeHandle node_comms; 

    //(A donde se publicara, cantidad de mensajes a publicar)
    ros::Publisher topic_pub = node_comms.advertise<std_msgs::String>("itesm_topic", 1); 
    ros::Publisher point_pub = node_comms.advertise<geometry_msgs::PointStamped>("point",1);

    ros::Rate loopRate(10);

    int counter = 0; 
    while(ros::ok())
    {
        std_msgs::String msg;           // Objeto tipo std_msgs para almacenar strings
        msg.data = "hola " + std::to_string(counter);   //Crear mensaje y concatenar con el contador

        topic_pub.publish(msg);     // Publicar mansaje el topico
        
        geometry_msgs::PointStamped msg_point;
        msg_point.point.x = 0;
        msg_point.point.y = std::cos(counter);
        msg_point.point.z = std::sin(counter);

        msg_point.header.stamp = ros::Time::now();   // Estampa de tiempo

        point_pub.publish(msg_point);

        counter++;
        loopRate.sleep();
    }

    return 0;
}