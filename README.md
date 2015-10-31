# cspm
This is a ros package for cspm project.
##introduction
CSPM means computer, smart phone and microcontrollers. We developed a simple robot, and put the smart phone on it.Then we read the data from the smart phone, just like ths imu data, camera, gps and geomagnitic.

We use ROS to manage these messages. And this repository is the ROS package we created. At the same time, we developed several android app to controll the robot, and the apk file will be uploaded recently.

##tutorials
###install
download this package, cd to cspm folder, then type

```
catkin_make
source devel/setup.bash
```

download CSPMRobot.apk,CSPMRemoter.apk and joystick.apk and install them in your smart phone.

###quick to start
open CSPMRobot, then put your mobile phone in your robot. type
```rosrun face_detect facedetect _param:=/camera/image```
then you will see a opencv windows which mark your face apeared.
