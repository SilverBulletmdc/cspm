__author__ = 'meng'
#!/usr/bin/env python
import roslib

import rospy
import tf.transformations as tfs
import tf
from geometry_msgs.msg import Twist
import std_msgs.msg
from nav_msgs.msg import Odometry
# init

class CSPMRobot:
    def __init__(self):
        self.tt = Twist()
        self.refresh()

        self.cmd_pub = rospy.Publisher('turtle1/cmd_vel',Twist, queue_size=50)
    def move_forword(self,speed):
        self.tt.linear.x = speed
        self.cmd_pub.publish(self.tt)
        self.refresh()
    def move_back(self,speed):
        self.tt.linear.x = -speed
        self.cmd_pub.publish(self.tt)
        self.refresh()
    def turn_left(self,speed):
        self.tt.angular.z = speed
        self.cmd_pub.publish(self.tt)
        self.refresh()
    def turn_right(self,speed):
        self.tt.angular.z = -speed
        self.cmd_pub.publish(self.tt)
        self.refresh()
    def stop(self):
        self.refresh()
        self.cmd_pub.publish(self.tt)
    def refresh(self):
        self.tt.angular.x = 0;
        self.tt.angular.y = 0;
        self.tt.angular.z = 0;
        self.tt.linear.x = 0;
        self.tt.linear.y = 0;
        self.tt.linear.z = 0;

def callback(msg):
    global cr
    message = msg.data

    cr = CSPMRobot()
    if message == "qian-jin":
        cr.move_forword(0.5)
        print 'qian jin'
    if message == "hou-tui":
        cr.move_back(0.5)
    if message == "zuo-zhuan":
        cr.turn_left(0.5)
    if message == "you-zhuan":
        cr.turn_right(0.5)
    if message == "ting-zhi":
        cr.stop()

rospy.init_node('imulistener', anonymous=True)
rospy.Subscriber('voice_py', std_msgs.msg.String, callback)
cr = CSPMRobot()

rospy.spin()
