#include "imageprocess.h"

#include <QTimer>
#include <QMessageBox>
#include <QWidget>
#include <QApplication>
#include <QDebug>
#include <QTextStream>

ImageProcess::ImageProcess(QObject *parent)
	: QObject(parent)
{
	//Initialize variable members
	m_timer = new QTimer();
	m_handDetector = new HandDetector();
	m_fingerTipResult = 100;
	isHandFistPressd = false;
	isHandPalmPressd = true;
	isConnect = false;
	getCubikglWidget();

	m_capture = cvCaptureFromCAM(0);
	if(!m_capture)
		QMessageBox::critical(NULL, "Error", "Error initializing capture from WebCam");

	m_frame = cvQueryFrame(m_capture);
	m_frameWidth = m_frame->width;
	m_frameHeight = m_frame->height;
	m_framesize = cvSize(m_frameWidth, m_frameHeight);
	m_graydst = cvCreateImage(m_framesize, IPL_DEPTH_8U, 1);
	m_rgbdst = cvCreateImage(m_framesize, IPL_DEPTH_8U, 3);

	m_bigContour = NULL;
	

	//Connect the timer signal with the capture action
	connect(m_timer, SIGNAL(timeout()), this, SLOT(captureFrame()));
	//Start the timer scheduled for firing every 33ms (30fps)
	m_timer->start(33);	
}

ImageProcess::~ImageProcess()
{
	delete m_handDetector;
	delete m_timer;
	//Release the capture
	cvReleaseCapture(&m_capture);
	cvReleaseImage(&m_graydst);
	cvReleaseImage(&m_rgbdst);
}

void ImageProcess::getCubikglWidget()
{
	foreach(QWidget *widget, QApplication::allWidgets()) {
		if(widget->objectName() == "m_cubikGLWidget")
		{
			m_cubikglwidget = widget;
			qDebug() << "Find CubikglWidget.";
		}	
	}
}

void ImageProcess::captureFrame()
{	
	qDebug() << "captureFrame begin";
	//Get an image from the webcam
	m_frame = cvQueryFrame(m_capture);

	// 肤色检测
	cvFlip(m_frame, NULL, -1);
	m_handDetector->skinDetect(m_frame, m_graydst);
	emit showGrayImage(m_graydst);

	// 寻找最大轮廓
	CvMemStorage* m_bigcontourStorage = cvCreateMemStorage(0);;
	m_handDetector->findBigContour(m_graydst, m_bigContour, m_bigcontourStorage);

	if (m_bigContour ==0)
	{
		return;
	}

	// 绘制轮廓
	// 外框红色
	// 内框紫色
	cvDrawContours(m_frame, m_bigContour, cvScalar(0, 0, 255), cvScalar(128, 0, 64), 2, 2, 8, cvPoint(0,0));

	CvMemStorage* m_approxStorage = cvCreateMemStorage(0);
	CvMemStorage* m_hullStorage = cvCreateMemStorage(0);
	CvMemStorage* m_defectsStorage = cvCreateMemStorage(0);

	m_handDetector->findFingerTips(m_bigContour, m_approxStorage, 
		m_hullStorage, m_defectsStorage, &m_tipPts, &m_foldPts, &m_depths);
	m_handDetector->reduceFingerTips(m_tipPts, m_foldPts, m_depths, &m_fingerTips);

	int m_tipPtsize = m_tipPts.size();
	for (int i = 0; i < m_tipPtsize; i++)
	{
		// 蓝色
		cvCircle(m_frame, *m_tipPts[i], 5, cvScalar(255, 0, 0), -1);
	}

	int m_foldPtsize = m_foldPts.size();
	for (int j = 0; j < m_foldPtsize; j++)
	{
		// 黄色
		cvCircle(m_frame, *m_foldPts[j], 5, cvScalar(0, 255, 255), -1);
	}

	int m_fingerTipsize = m_fingerTips.size();
	for (int n = 0; n < m_fingerTipsize; n++)
	{
		// 绿色
		cvCircle(m_frame, *m_fingerTips[n], 5, cvScalar(0, 255, 0), -1);
	}

	m_fingerTipResult = m_handDetector->getFingerTipsResult(m_fingerTipsize);

	QString text;
	QTextStream(&text) << "FingerTipsResult = " << m_fingerTipResult;
	qDebug() << text;

	m_handDetector->getContourInfo( m_bigContour, m_cogPt, m_contourAxisAngle, m_fingerTips);
	// 橙色
	cvCircle(m_frame, m_cogPt, 5, cvScalar(0, 128, 255), -1);

	// 释放内存
	cvReleaseMemStorage(&m_bigcontourStorage);
	cvReleaseMemStorage(&m_approxStorage);
	cvReleaseMemStorage(&m_hullStorage);
	cvReleaseMemStorage(&m_defectsStorage);

	emit showRgbImage(m_frame);
	
	if (isConnect)
	{
		switch(m_fingerTipsize)
		{
		case 0://Fist
			{
				if (isHandFistPressd)
				{
					isHandPalmPressd = false;
					QPoint pos = QPoint(m_cogPt.x, m_cogPt.y);
					QApplication::postEvent(m_cubikglwidget, new HandEvent(HandFistMove, pos));
				} 
				else
				{
					isHandFistPressd = true;
					isHandPalmPressd = false;
					QPoint pos = QPoint(m_cogPt.x, m_cogPt.y);
					QApplication::postEvent(m_cubikglwidget, new HandEvent(HandFistPress, pos));
				}
			}
			break;
		case 1://One finger
			{
				isHandFistPressd = false;
				isHandPalmPressd = false;
				switch (3 * m_cogPt.x / m_frameWidth)
				{
				case 0:
					{
						QPoint pos = QPoint(0, 0);
						QApplication::postEvent(m_cubikglwidget, new HandEvent(HandOneFinger, pos));
					}
					break;
				case 1:
					{
						QPoint pos = QPoint(1, 0);
						QApplication::postEvent(m_cubikglwidget, new HandEvent(HandOneFinger, pos));
					}
					break;
				case 3:
					{
						QPoint pos = QPoint(2, 0);
						QApplication::postEvent(m_cubikglwidget, new HandEvent(HandOneFinger, pos));
					}
					break;
				default:
					break;
				}
			}
			break;
		case 2://Two finger
			{
				isHandFistPressd = false;
				isHandPalmPressd = false;
				switch (3 * m_cogPt.x / m_frameWidth)
				{
				case 0:
					{
						QPoint pos = QPoint(0, 0);
						QApplication::postEvent(m_cubikglwidget, new HandEvent(HandTwoFinger, pos));
					}
					break;
				case 1:
					{
						QPoint pos = QPoint(1, 0);
						QApplication::postEvent(m_cubikglwidget, new HandEvent(HandTwoFinger, pos));
					}
					break;
				case 3:
					{
						QPoint pos = QPoint(2, 0);
						QApplication::postEvent(m_cubikglwidget, new HandEvent(HandTwoFinger, pos));
					}
					break;
				default:
					break;
				}
			}
			break;
		case 3://Three finger
			{
				isHandFistPressd = false;
				isHandPalmPressd = false;
				switch (3 * m_cogPt.x / m_frameWidth)
				{
				case 0:
					{
						QPoint pos = QPoint(0, 0);
						QApplication::postEvent(m_cubikglwidget, new HandEvent(HandThreeFinger, pos));
					}
					break;
				case 1:
					{
						QPoint pos = QPoint(1, 0);
						QApplication::postEvent(m_cubikglwidget, new HandEvent(HandThreeFinger, pos));
					}
					break;
				case 3:
					{
						QPoint pos = QPoint(2, 0);
						QApplication::postEvent(m_cubikglwidget, new HandEvent(HandThreeFinger, pos));
					}
					break;
				default:
					break;
				}
			}
			break;
		case 4://Palm
		case 5://Palm
			{
				if (isHandPalmPressd)
				{
					isHandFistPressd = false;
					QPoint pos = QPoint(m_cogPt.x, m_cogPt.y);
					QApplication::postEvent(m_cubikglwidget, new HandEvent(HandPalmMove, pos));
				} 
				else
				{
					isHandFistPressd = false;
					isHandPalmPressd = true;
					QPoint pos = QPoint(m_cogPt.x, m_cogPt.y);
					QApplication::postEvent(m_cubikglwidget, new HandEvent(HandPalmPress, pos));
				}
			}
			break;
		default:
			{
				isHandFistPressd = false;
				isHandPalmPressd = false;
			}
			break;
		}
	}
	qDebug() << "captureFrame end";
}

void ImageProcess::setconnect()
{
	isConnect = true;
}

void ImageProcess::setdisconnect()
{
	isConnect = false;
}
