#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<ros/ros.h>
#include<synchronizer/Combined.h>
#include<thruster_controller/ThrusterSpeeds.h>
#include <ros/package.h>
#include <fstream>
#include <iostream>
#include<QKeyEvent>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(ros::NodeHandle _nh,QWidget *parent=0 );
    ~MainWindow();
     void setPID();
     void callback(const synchronizer::Combined::ConstPtr& msg);
     void setforces();
private slots:
     void  setlabels();
     void  on_save_clicked();

private:
    Ui::MainWindow *ui;
    ros::NodeHandle nh;
    ros::Publisher pub;
    ros::Subscriber sub;
    enum{ROLL,PITCH,YAW,DEPTH};
    enum{SURGE,SWAY};
    float kp[4];
    float ki[4];
    float kd[4];
    float output[4];
    float error[4];
    float ierror[4];
    float derror[4];
    float preverror[4];
    thruster_controller::ThrusterSpeeds mg;
    float setpoints[4];
    float thrust[2];
    float base;
    int flag=0;
protected:
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
};

#endif // MAINWINDOW_H
