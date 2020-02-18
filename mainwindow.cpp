#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    my_image = cv::imread("/home/ahmet/Desktop/euler_copy.jpg",CV_LOAD_IMAGE_COLOR);

    cv::resize(my_image,my_image, cv::Size(194,259),0,0,CV_INTER_LINEAR);
    cv::cvtColor(my_image,my_image,CV_BGR2RGB); //Qt reads in RGB whereas CV in BGR

    //cv::imshow("MY image",my_image);
    QImage show_my_image((uchar*)my_image.data, my_image.cols, my_image.rows, my_image.step, QImage::Format_RGB888);
    ui->label_image->setPixmap(QPixmap::fromImage(show_my_image));

    connect(ui->pushButton_negative,SIGNAL(clicked(bool)),this,SLOT(negative_euler()));

    connect(ui->horizontalSlider_angle,SIGNAL(valueChanged(int)),ui->spinBox_angle,SLOT(setValue(int)));
    connect(ui->spinBox_angle,SIGNAL(valueChanged(int)),ui->horizontalSlider_angle,SLOT(setValue(int)));
    connect(ui->spinBox_angle,SIGNAL(valueChanged(int)),this,SLOT(rotate_euler()));


}
void MainWindow::negative_euler(void){
    cv::Mat neg_im;
    cv::Mat res;
    cv::bitwise_not(my_image,neg_im,res);

    //cv::imshow("negative",neg_im);
    my_image = neg_im;
    QImage show_my_image((uchar*)my_image.data, my_image.cols, my_image.rows, my_image.step, QImage::Format_RGB888);
    ui->label_image->setPixmap(QPixmap::fromImage(show_my_image));

    rotate_euler();

}
void MainWindow::rotate_euler(void){
    double angle = ui->spinBox_angle->value();

    // get rotation matrix for rotating the image around its center in pixel coordinates
    cv::Point2f center((my_image.cols-1)/2.0, (my_image.rows-1)/2.0);
    cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
    // determine bounding rectangle, center not relevant
    cv::Rect bbox = cv::RotatedRect(cv::Point2f(), my_image.size(), angle).boundingRect();
    // adjust transformation matrix
    rot.at<double>(0,2) += bbox.width/2.0 - my_image.cols/2.0;
    rot.at<double>(1,2) += bbox.height/2.0 - my_image.rows/2.0;
    cv::Mat dst;
    cv::warpAffine(my_image, dst, rot, bbox.size());
    QImage show_my_image((uchar*)dst.data, dst.cols, dst.rows, dst.step, QImage::Format_RGB888);
    ui->label_image->setPixmap(QPixmap::fromImage(show_my_image));

}
MainWindow::~MainWindow()
{
    delete ui;
}
