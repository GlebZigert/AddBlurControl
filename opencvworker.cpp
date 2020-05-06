#include "opencvworker.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <QDebug>

OpenCvWorker::OpenCvWorker(QObject *parent) :
    QObject(parent),
    status(false),
    toogleStream(false),
    binaryThresholdEnable(false),
    binaryThreshold(127)

{

    cap = new cv::VideoCapture();

}
OpenCvWorker::~OpenCvWorker()
{
    if(cap->isOpened())cap->release();
    delete cap;
}

void OpenCvWorker::receiveGrabFrame()
{
if (!toogleStream) return;
(*cap)>>_frameOriginal;
if (_frameOriginal.empty()) return;

process();


putText(_frameProcessed, "Differencing the two images.", cvPoint(30,30),
    cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 1, CV_AA);

//cv::putText(_frameProcessed, "{}: {:.2f}".format(text, fm), (10, 30),
//    cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 0, 255), 3);

QImage output(
            (const unsigned char *)_frameProcessed.data,
            _frameProcessed.cols,
            _frameProcessed.rows,
            QImage::Format_Indexed8
            );
emit send_frame(output);

}

void OpenCvWorker::process()
{

    cv::cvtColor(_frameOriginal,_frameProcessed,cv::COLOR_BGR2GRAY);
    if (binaryThresholdEnable)
    {
      cv::threshold(_frameProcessed,_frameProcessed,binaryThreshold,255,cv::THRESH_BINARY);

    }

}

void OpenCvWorker::checkIfDeviceAlreadyOpened(const int device)
{
    if(cap->isOpened())cap->release();
    cap->open(device);
}

int OpenCvWorker::variance_of_laplacian(cv::Mat image)
{
    return 12345;
}

void OpenCvWorker::receiveSetup(const int device)
{
checkIfDeviceAlreadyOpened(device);
if(!cap->isOpened())
    {
    status=false;
    return;

    }
status=true;
}

void OpenCvWorker::receiveToggleStream()
{
toogleStream=!toogleStream;
}

void OpenCvWorker::receiveEnableBinaryThreshold()
{
binaryThresholdEnable=!binaryThresholdEnable;
}

void OpenCvWorker::receiveBinaryThreshold(int threshold)
{
binaryThreshold=threshold;
qDebug()<<binaryThreshold;
}









