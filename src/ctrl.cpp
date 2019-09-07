#include<ros/ros.h>
#include<synchronizer/Combined.h>
#include<thruster_controller/ThrusterSpeeds.h>
#include<iostream>
#include<opencv2/opencv.hpp>
using namespace std;
int flag=0;
float kp=0.01;
float error=0;
int dep=1000;
char ch='c';
//#include<ctrl/ThrusterSpeeds.h>
void callback(const synchronizer::Combined::ConstPtr& msg)
{

  error=(int)(dep-msg->depth);
}
int main(int argc,char **argv)
{
  ros::init(argc,argv,"controller");
  ros::NodeHandle nh;
  ros::Publisher pub=nh.advertise<thruster_controller::ThrusterSpeeds>("thruster_speeds",1);
  ros::Subscriber sub=nh.subscribe("combined",1000,callback);
  ros::Rate r(30);
  thruster_controller::ThrusterSpeeds mg;
  for(int i=0;i<6;i++)
  {
    mg.data.push_back(1500);
    mg.reverse.push_back(false);
  }

float thrust[6];
for(int i=0;i<6;i++)
thrust[i]=1500;
  while(ros::ok())
{
  /*ch=getchar();
  //cin>>ch;
  cout<<(int)ch<<endl;
  if(ch!=-1)
    cout<<"Hello"<<endl;
  if(ch=='A')
  {
    mg.data[5]=1600;
    cout<<"2"<<endl;
  }
  else if(ch=='D')
    mg.data[5]=1400;
    else if(ch=='W')
    {
      mg.data[3]=1600;
      mg.data[4]=1600;
    }
    else if(ch=='S')
    {
      mg.data[3]=1400;
      mg.data[4]=1400;
    }
  else
  {
    mg.data[5]=1500;
    mg.data[3]=1500;
    mg.data[4]=1500;
  }*/
  thrust[0]=1500;
  thrust[1]=1500;
  thrust[2]+=error*kp;
  mg.data[0]=1500;
  mg.data[1]=1500;
  mg.data[2]=1500;
  mg.data[3]=1540;
  mg.data[4]=1460;
  pub.publish(mg);
  ros::spinOnce();
  r.sleep();
}
}
