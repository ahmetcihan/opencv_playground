#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() << "geldi";
    //my_vid.open("rtsp://192.168.1.240/stream2");
    my_image = cv::imread("/home/ahmet/Desktop/euler_copy.jpg",cv::IMREAD_COLOR);
    qDebug() << "geldi 2";

    cv::resize(my_image,my_image, cv::Size(194,259),0,0,cv::INTER_LINEAR);
    cv::cvtColor(my_image,my_image,cv::COLOR_BGR2RGB); //Qt reads in RGB whereas CV in BGR

    cv::imshow("MY image",my_image);
    QImage show_my_image((uchar*)my_image.data, my_image.cols, my_image.rows, my_image.step, QImage::Format_RGB888);
    ui->label_image->setPixmap(QPixmap::fromImage(show_my_image));

    connect(ui->pushButton_negative,SIGNAL(clicked(bool)),this,SLOT(negative_euler()));

    connect(ui->horizontalSlider_angle,SIGNAL(valueChanged(int)),ui->spinBox_angle,SLOT(setValue(int)));
    connect(ui->spinBox_angle,SIGNAL(valueChanged(int)),ui->horizontalSlider_angle,SLOT(setValue(int)));
    connect(ui->spinBox_angle,SIGNAL(valueChanged(int)),this,SLOT(rotate_euler()));

    periodic_timer = new QTimer(this);
    periodic_timer->setInterval(200);
    //connect(periodic_timer,SIGNAL(timeout()),this,SLOT(capture_video()));
    //connect(periodic_timer,SIGNAL(timeout()),this,SLOT(detect_my_face()));
    //periodic_timer->start();

    //my_vid.set(CV_CAP_PROP_FPS,10);

    //detect_my_face();
}
void MainWindow::detectAndDraw( cv::Mat& img, cv::CascadeClassifier& cascade, cv::CascadeClassifier& nestedCascade, double scale){
    cv::vector<cv::Rect> faces, faces2;
    cv::Mat gray, smallImg;

    cv::cvtColor( img, gray, cv::COLOR_BGR2GRAY ); // Convert to Gray Scale
    double fx = 1 / scale;

    // Resize the Grayscale Image
    cv::resize( gray, smallImg, cv::Size(), fx, fx, cv::INTER_LINEAR );
    cv::equalizeHist( smallImg, smallImg );

    // Detect faces of different sizes using cascade classifier
    cascade.detectMultiScale( smallImg, faces, 1.1, 2, 0|cv::CASCADE_SCALE_IMAGE, cv::Size(50, 50) );

    // Draw circles around the faces
    for ( size_t i = 0; i < faces.size(); i++ ){
        cv::Rect r = faces[i];
        cv::Mat smallImgROI;
        cv::vector<cv::Rect> nestedObjects;
        cv::Point center;
        cv::Scalar color = cv::Scalar(255, 0, 0); // Color for Drawing tool
        int radius;

        double aspect_ratio = (double)r.width/r.height;
        if( 0.75 < aspect_ratio && aspect_ratio < 1.3 )
        {
            center.x = cvRound((r.x + r.width*0.5)*scale);
            center.y = cvRound((r.y + r.height*0.5)*scale);
            radius = cvRound((r.width + r.height)*0.25*scale);
            circle( img, center, radius, color, 3, 8, 0 );
        }
        else
            rectangle( img, cv::Point(cvRound(r.x*scale), cvRound(r.y*scale)),
                    cv::Point(cvRound((r.x + r.width-1)*scale),
                    cvRound((r.y + r.height-1)*scale)), color, 3, 8, 0);
        if( nestedCascade.empty() )
            continue;
        smallImgROI = smallImg( r );

//        // Detection of eyes int the input image
//        nestedCascade.detectMultiScale( smallImgROI, nestedObjects, 1.1, 2, 0|cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30) );

//        // Draw circles around eyes
//        for ( size_t j = 0; j < nestedObjects.size(); j++ )
//        {
//            cv::Rect nr = nestedObjects[j];
//            center.x = cvRound((r.x + nr.x + nr.width*0.5)*scale);
//            center.y = cvRound((r.y + nr.y + nr.height*0.5)*scale);
//            radius = cvRound((nr.width + nr.height)*0.25*scale);
//            circle( img, center, radius, color, 3, 8, 0 );
//        }
    }

    // Show Processed Image with detected faces
    imshow( "Face Detection", img );
}
void MainWindow::detect_my_face(void){
    QElapsedTimer my_timer;
    my_timer.start();
    cv::CascadeClassifier cascade, nestedCascade;
    double scale = 1;

    nestedCascade.load("/usr/local/share/OpenCV/haarcascades/haarcascade_eye_tree_eyeglasses.xml");
    cascade.load("/usr/local/share/OpenCV/haarcascades/haarcascade_frontalcatface.xml");

    //cv::Mat frame1 = cv::imread("/home/ahmet/Desktop/ahmet.jpg",CV_LOAD_IMAGE_COLOR);

    cv::Mat frame1;
    my_vid >> frame1;
    //cv::resize(frame1,frame1, cv::Size(480,270),0,0,CV_INTER_LINEAR);
    //cv::resize(frame1,frame1, cv::Size(720,405),0,0,CV_INTER_LINEAR);

    detectAndDraw( frame1, cascade, nestedCascade, scale );

    qDebug() << "elpased" << my_timer.elapsed();
}
void MainWindow::capture_video(void){
    cv::Mat my_frame;
    my_vid >> my_frame;

    //find fps of the vid
    qDebug() << "fps" << my_vid.get(cv::CAP_PROP_FPS);

    cv::resize(my_frame,my_frame, cv::Size(480,270),0,0,cv::INTER_LINEAR);

    /////////////////////////////////////////
    double angle = ui->spinBox_angle->value();

    cv::Point2f center((my_frame.cols-1)/2.0, (my_frame.rows-1)/2.0);
    cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
    cv::Rect bbox = cv::RotatedRect(cv::Point2f(), my_frame.size(), angle).boundingRect();
    rot.at<double>(0,2) += bbox.width/2.0 - my_frame.cols/2.0;
    rot.at<double>(1,2) += bbox.height/2.0 - my_frame.rows/2.0;
    cv::Mat dst;
    cv::warpAffine(my_frame, dst, rot, bbox.size());
    QImage show_my_image((uchar*)dst.data, dst.cols, dst.rows, dst.step, QImage::Format_RGB888);
    ui->label_video->setPixmap(QPixmap::fromImage(show_my_image));
    /////////////////////////////////////////

//    QImage show_my_vid((uchar*)my_frame.data, my_frame.cols, my_frame.rows, my_frame.step, QImage::Format_RGB888);
//    ui->label_video->setPixmap(QPixmap::fromImage(show_my_vid));

    my_frame.release();
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
