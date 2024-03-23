#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QElapsedTimer>
#include <QDebug>
#include <core.hpp>
#include <highgui.hpp>
#include <imgproc.hpp>
#include <objdetect.hpp>
#include <opencv.hpp>
#include <face.hpp>

namespace Ui {
class MainWindow;
}
namespace cv
{
    using std::vector;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QTimer *periodic_timer;
    cv::Mat sample_image;
    cv::VideoCapture ip_cam_vid;

public slots:
    void rotate_euler(void);
    void negative_euler(void);
    void capture_video(void);
    void detect_my_face(void);
    void detectAndDraw(cv::Mat& img, cv::CascadeClassifier& cascade, cv::CascadeClassifier& nestedCascade, double scale );

};

#endif // MAINWINDOW_H
