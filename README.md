# ObjectFollower

A mobile platform that uses a logitech webcam as a sensor to follow blue objects using ROS Noetic in Ubutu 20.8 running ina RPi 4.

# What I Learned

* Create a object tracking script with OpenCV using C++ and sharing the centroid of the object through a ROS topic
* Implement a low-level node to perform the PID Control for the motors speeds
* Integrated Rosserial comunication to create the closed loop control between the camera node that was running on the RPi and the PID node running on the Arduino UNO
