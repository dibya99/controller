#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{   ros::init(argc,argv,"nod");
    ros::NodeHandle nh;
    QApplication a(argc, argv);
    MainWindow w(nh);
    w.show();

    ros::Rate rate(30);
    while(ros::ok() && w.isVisible())
    {
        rate.sleep();
        ros::spinOnce();
        a.processEvents();
    }
}
