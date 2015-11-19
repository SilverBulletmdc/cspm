#include "stdafx.h"
#include <vector>
#include "sensor_msgs/Image.h"

using namespace std;
using namespace cv;

static const char WINDOW[]="RGB Image";
static const std_msgs::Empty e;




class cspm_robot{

    ros::Publisher takeoff_pub;
    ros::Publisher land_pub;
    image_transport::Subscriber image_sub;
    geometry_msgs::Twist cmd_vel;
    geometry_msgs::Twist fresh_vel;
    ros::Publisher vel_pub;
    Mat last_image;//上一张图片
    Mat current_image;//下一张图片
    Mat edges;
    CascadeClassifier cascade,nestedCascade;
    bool stop;
    Point ctr;
    int pix_sum;

    //pid
    int Kpx;
    int Kpy;
    int Kdx;
    int Kdy;
    int Kix;
    int Kiy;
    int pix_sum_old;
    int pix_sum_0;
    int idx;
    int err;
    bool detect_flag;





    bool flag;

    void forward(float i){
        cmd_vel = fresh_vel;
        cmd_vel.linear.x = i;
        vel_pub.publish(cmd_vel);
    }
    void back(float i){
        cmd_vel = fresh_vel;

        cmd_vel.linear.x = -i;
        vel_pub.publish(cmd_vel);

    }
    void rotation_clock(float i){
        cmd_vel = fresh_vel;

        cmd_vel.angular.z = i;
        vel_pub.publish(cmd_vel);

    }
    void rotation_inclock(float i){
        cmd_vel = fresh_vel;

        cmd_vel.angular.z = -i;
        vel_pub.publish(cmd_vel);

    }
    void move(float x, float y, float z, float rx, float ry, float rz){
        cmd_vel = fresh_vel;
        cmd_vel.linear.x = x;
        cmd_vel.linear.y = y;
        cmd_vel.linear.z = z;
        cmd_vel.angular.x = rx;
        cmd_vel.angular.y = ry;
        cmd_vel.angular.z = rz;
        vel_pub.publish(cmd_vel);
    }
    double pid(double err,double Kp){
        return Kp*err;
    }
    double dst(int ctr,int siz){
        siz = siz/2;
        return double((ctr - siz)) / double(siz);
    }
    void process(const sensor_msgs::ImageConstPtr& cam_image){
        cv_bridge::CvImagePtr cv_ptr;


        try
        {
          cv_ptr = cv_bridge::toCvCopy(cam_image,sensor_msgs::image_encodings::BGR8);
        } catch (cv_bridge::Exception& e){
          ROS_ERROR("cv_bridge exception:%s",e.what());

          return;
        }
        current_image = cv_ptr->image;
        detectAndDraw(current_image,cascade,nestedCascade,2,0,pix_sum,ctr);
        cout << "ctr:" << ctr.x << " " << ctr.y << endl;
        idx++;
        if(idx % 15 == 1 || (ctr.x != 0 && idx % 8 ==1)){

            cmd_vel = fresh_vel;
            cout << "rows" << current_image.rows;
            cout << "cols" << current_image.cols;
            double erry = dst(ctr.x,current_image.cols);
            double errx = dst(ctr.y,current_image.rows);
            cout << "erry:" << erry << endl;


            cmd_vel.linear.y = -pid(erry,Kpy);
            cmd_vel.linear.x = pid(errx,Kpx);
            vel_pub.publish(cmd_vel);
            ctr.x = current_image.cols/2;
            ctr.y = current_image.rows/2;



        }


        resize(current_image,current_image,Size(640,360));
        imshow(WINDOW,current_image);
        int k = waitKey(1);
        if(k != -1)
            cout << k << endl;

        if(k == UP)//up for forward
            forward(1);
        if(k == DOWN)//down for back
            back(1);
        if(k == LEFT)//left for turn_left
            rotation_clock(1);
        if(k == RIGHT)//right for turn_right
            rotation_inclock(1);
        if(k == SPACE)
            detect_flag = true;

        return;

        }
public:
    /*
     *init the subscribers and publishers
     */
    cspm_robot(char* topic_name){


        ros::NodeHandle n;
        image_transport::ImageTransport it(n);
        image_sub = it.subscribe("/camera/image",1,&cspm_robot::process,this);
        vel_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel",1);
        cascade.load("haarcascade_frontalface_alt.xml");
        nestedCascade.load("haarcascade_eye_tree_eyeglasses.xml");
        Kpx = 0.25;
        Kpy = 1;
        Kdx = 0.25;
        Kdy = 0.25;
        Kix = 0;
        Kiy = 0;
        pix_sum_old = 1;
        pix_sum_0 = 1;
        idx = 0;
        detect_flag = false;
    }



};



int main(int argc, char **argv){
    ros::init(argc,argv,"facedetect");
    cv::namedWindow(WINDOW);


    cspm_robot cr("/image/camera");



    ros::spin();
    return 0;
    }
