#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(ros::NodeHandle _nh,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),nh(_nh)
{
    ui->setupUi(this);
     pub=nh.advertise<thruster_controller::ThrusterSpeeds>("thruster_speeds",1);
     sub=nh.subscribe("combined",1000,&MainWindow::callback,this);
     for(int i=0;i<6;i++)
     {
       mg.data.push_back(1500);
       mg.reverse.push_back(false);
     }
     connect(ui->kp_pitch, SIGNAL(valueChanged(int)), SLOT(setlabels()));
     connect(ui->ki_pitch, SIGNAL(valueChanged(int)), SLOT(setlabels()));
     connect(ui->kd_pitch, SIGNAL(valueChanged(int)), SLOT(setlabels()));
     connect(ui->kp_roll, SIGNAL(valueChanged(int)), SLOT(setlabels()));
     connect(ui->ki_roll, SIGNAL(valueChanged(int)), SLOT(setlabels()));
     connect(ui->kd_roll, SIGNAL(valueChanged(int)), SLOT(setlabels()));
     connect(ui->kp_yaw, SIGNAL(valueChanged(int)), SLOT(setlabels()));
     connect(ui->ki_yaw, SIGNAL(valueChanged(int)), SLOT(setlabels()));
     connect(ui->kd_yaw, SIGNAL(valueChanged(int)), SLOT(setlabels()));
    connect(ui->kp_depth, SIGNAL(valueChanged(int)), SLOT(setlabels()));
    connect(ui->ki_depth, SIGNAL(valueChanged(int)), SLOT(setlabels()));
    connect(ui->kd_depth, SIGNAL(valueChanged(int)), SLOT(setlabels()));
    //connect(ui->Dept, SIGNAL(valueChanged(int)), SLOT(setlabels()));

    connect(ui->save,SIGNAL(clicked()),SLOT(on_save_clicked()));
    std::string confFileName = ros::package::getPath("ctrl") + "/conf/cfile.conf";
    std::ifstream confFile(confFileName.c_str());
    float val;
    confFile>>val;
    ui->kp_pitch->setValue(int(val));
    confFile>>val;
    ui->ki_pitch->setValue(int(val));
    confFile>>val;
    ui->kd_pitch->setValue(int(val));
    confFile>>val;
    ui->kp_roll->setValue(int(val));
    confFile>>val;
    ui->ki_roll->setValue(int(val));
    confFile>>val;
    ui->kd_roll->setValue(int(val));
    confFile>>val;
    ui->kp_yaw->setValue(int(val));
    confFile>>val;
    ui->ki_yaw->setValue(int(val));
    confFile>>val;
    ui->kd_yaw->setValue(int(val));
    confFile>>val;
    ui->kp_depth->setValue(int(val));
    confFile>>val;
    ui->ki_depth->setValue(int(val));
    confFile>>val;
    ui->kd_depth->setValue(int(val));
    setpoints[PITCH]=0;
    setpoints[ROLL]=0;
    setpoints[YAW]=-90;
    setpoints[DEPTH]=950;
    //ui->Dept->setValue(int(setpoints[DEPTH]));
    setlabels();
    for(int i=0;i<4;i++)
    preverror[i]=0;
    thrust[SURGE]=0;
    thrust[SWAY]=0;
    base=1500;


}
void MainWindow::setlabels()
{
    ui->kp_pitch_txt->setText(QString::number(ui->kp_pitch->value()/10)+'.'+QString::number(ui->kp_pitch->value()%10));
    ui->ki_pitch_txt->setText(QString::number(ui->ki_pitch->value()/10)+'.'+QString::number(ui->ki_pitch->value()%10));
    ui->kd_pitch_txt->setText(QString::number(ui->kd_pitch->value()/10)+'.'+QString::number(ui->kd_pitch->value()%10));
    ui->kp_roll_txt->setText(QString::number(ui->kp_roll->value()/10)+'.'+QString::number(ui->kp_roll->value()%10));
    ui->ki_roll_txt->setText(QString::number(ui->ki_roll->value()/10)+'.'+QString::number(ui->ki_roll->value()%10));
    ui->kd_roll_txt->setText(QString::number(ui->kd_roll->value()/10)+'.'+QString::number(ui->kd_roll->value()%10));
    ui->kp_yaw_txt->setText(QString::number(ui->kp_yaw->value()/10)+'.'+QString::number(ui->kp_yaw->value()%10));
    ui->ki_yaw_txt->setText(QString::number(ui->ki_yaw->value()/10)+'.'+QString::number(ui->ki_yaw->value()%10));
    ui->kd_yaw_txt->setText(QString::number(ui->kd_yaw->value()/10)+'.'+QString::number(ui->kd_yaw->value()%10));
    ui->kp_depth_txt->setText(QString::number(ui->kp_depth->value()/10)+'.'+QString::number(ui->kp_depth->value()%10));
    ui->ki_depth_txt->setText(QString::number(ui->ki_depth->value()/10)+'.'+QString::number(ui->ki_depth->value()%10));
    ui->kd_depth_txt->setText(QString::number(ui->kd_depth->value()/10)+'.'+QString::number(ui->kd_depth->value()%10));
    //ui->dept_txt->setText(QString::number(ui->Dept->value()));
    setPID();
}
void MainWindow::on_save_clicked()
{
  std::string confFileName = ros::package::getPath("ctrl") + "/conf/cfile.conf";
  std::ofstream confFile(confFileName.c_str());
  confFile<<ui->kp_pitch->value()<<'\n';
  confFile<<ui->ki_pitch->value()<<'\n';
  confFile<<ui->kd_pitch->value()<<'\n';
  confFile<<ui->kp_roll->value()<<'\n';
  confFile<<ui->ki_roll->value()<<'\n';
  confFile<<ui->kd_roll->value()<<'\n';
  confFile<<ui->kp_yaw->value()<<'\n';
  confFile<<ui->ki_yaw->value()<<'\n';
  confFile<<ui->kd_yaw->value()<<'\n';
  confFile<<ui->kp_depth->value()<<'\n';
  confFile<<ui->ki_depth->value()<<'\n';
  confFile<<ui->kd_depth->value()<<'\n';
}
void MainWindow::setPID()
{
  kp[PITCH]=float(ui->kp_pitch->value()/10.0);
  ki[PITCH]=float(ui->ki_pitch->value()/10.0);
  kd[PITCH]=float(ui->kd_pitch->value()/10.0);
  kp[ROLL]=float(ui->kp_roll->value()/10.0);
  ki[ROLL]=float(ui->ki_roll->value()/10.0);
  kd[ROLL]=float(ui->kd_roll->value()/10.0);
  kp[YAW]=float(ui->kp_yaw->value()/10.0);
  ki[YAW]=float(ui->ki_yaw->value()/10.0);
  kd[YAW]=float(ui->kd_yaw->value()/10.0);
  kp[DEPTH]=float(ui->kp_depth->value()/10.0);
  ki[DEPTH]=float(ui->ki_depth->value()/10.0);
  kd[DEPTH]=float(ui->kd_depth->value()/10.0);
  //setpoints[DEPTH]=ui->Dept->value();
}
void MainWindow::callback(const synchronizer::Combined::ConstPtr& msg)
{

     for(int i=0;i<3;i++)
     {
       if(i==YAW)
       continue;
       error[i]=msg->angular[i]-setpoints[i];
       ierror[i]=error[i]+preverror[i];
       derror[i]=error[i]-preverror[i];
     }
     error[DEPTH]=msg->depth-setpoints[DEPTH];
     ierror[DEPTH]=error[DEPTH]+preverror[DEPTH];
     derror[DEPTH]=error[DEPTH]-preverror[DEPTH];
     error[YAW]=msg->angular[YAW]-setpoints[YAW];
     if(error[YAW]>=180)
     error[YAW]-=360;
     else if(error[YAW]<=-180)
     error[YAW]+=360;
     ierror[YAW]=error[YAW]+preverror[YAW];
     derror[YAW]=error[YAW]-preverror[YAW];
     for(int i=0;i<4;i++)
     {
      output[i]=kp[i]*error[i]+ki[i]*ierror[i]+kd[i]*derror[i];
      preverror[i]=error[i];
     }
     std::cout<<setpoints[DEPTH]<<" ";
     std::cout<<setpoints[YAW]<<std::endl;
     setforces();
     pub.publish(mg);
}
void MainWindow::keyPressEvent(QKeyEvent *e)
{
  if(e->key()==Qt::Key_W)
  {
    thrust[SURGE]=200;
  }
  else if(e->key()==Qt::Key_S)
  {
    thrust[SURGE]=-200;
  }
  else if(e->key()==Qt::Key_D)
  {
    thrust[SWAY]=200;
  }
  else if(e->key()==Qt::Key_A)
  {
    thrust[SWAY]=-200;
  }
  else if(e->key()==Qt::Key_Z)
  {
    setpoints[DEPTH]-=5;
    if(setpoints[DEPTH]<=970)
    setpoints[DEPTH]=970;
  }
  else if(e->key()==Qt::Key_C)
  {
    setpoints[DEPTH]+=5;
  }
  else if(e->key()==Qt::Key_E)
  {
    setpoints[YAW]+=5;
    if(setpoints[YAW]>=180)
    setpoints[YAW]=-180;
  }
  else if(e->key()==Qt::Key_Q)
  {
    setpoints[YAW]-=5;
    if(setpoints[YAW]<=-180)
    setpoints[YAW]=180;
  }
}
void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
  if(e->key()==Qt::Key_W||e->key()==Qt::Key_S||e->key()==Qt::Key_A||e->key()==Qt::Key_D||e->key()==Qt::Key_E||e->key()==Qt::Key_Q)
  {
    thrust[SURGE]=0;
    thrust[SWAY]=0;
}
}
void MainWindow::setforces()
{
   mg.data[0]=int(base+output[PITCH]+output[ROLL]-output[DEPTH]);
   mg.data[1]=int(base+output[PITCH]-output[ROLL]-output[DEPTH]);
   mg.data[2]=int(base-2*output[PITCH]-2*output[DEPTH]);
   mg.data[3]=int(base+thrust[SURGE]-output[YAW]);
   mg.data[4]=int(base+thrust[SURGE]+output[YAW]);
   mg.data[5]=int(base+thrust[SWAY]);
   for(int i=0;i<6;i++)
   {
     if(mg.data[i]>=1750)
     mg.data[i]=1750;
     if(i<3)
     {
       if(mg.data[i]<=1500)
       mg.data[i]=1500;
     }
     else
     {
       if(mg.data[i]<=1250)
       mg.data[i]=1250;
     }
   }

}
MainWindow::~MainWindow()
{
    delete ui;
}
