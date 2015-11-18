#!/usr/bin/env python
import roslib
import sys
import rospy
import cv2
from std_msgs.msg import String
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError
import color
import geometry_msgs.msg

class image_converter:

  def __init__(self):

    self.cmd_pub = rospy.Publisher("/cmd_vel",geometry_msgs.msg.Twist,queue_size= 1)
    self.image_sub = rospy.Subscriber("/camera/image",Image,self.callback,queue_size = 1 )
    cv2.namedWindow("Image window", cv2.CV_WINDOW_AUTOSIZE)
    self.bridge = CvBridge()


  def callback(self,data):
    twist = geometry_msgs.msg.Twist()
    try:
      cv_image = self.bridge.imgmsg_to_cv2(data, "bgr8")
    except CvBridgeError, e:
      print e

    (rows,cols,channels) = cv_image.shape
    (vx,vy,vz,wx,wy,wz) = color.action(cv_image,640,360)
    print (vx,vy,vz,wx,wy,wz)
    twist.linear.x = 0
    twist.linear.y = vy
    twist.linear.z = vz
    twist.angular.x = wx
    twist.angular.y = wy
    twist.angular.z = wz
    self.cmd_pub.publish(twist)
    #cv2.imshow("Image window", cv_image)
    cv2.waitKey(3)


def main(args):
  ic = image_converter()
  rospy.init_node('color_tracker', anonymous=True)
  try:
    rospy.spin()
  except KeyboardInterrupt:
    print "Shutting down"
  cv2.destroyAllWindows()

if __name__ == '__main__':
    main(sys.argv)
