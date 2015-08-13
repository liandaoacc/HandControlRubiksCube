#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include <QObject>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "handdetector.h"
#include "handevent.h"
class QWidget;
class QTimer;

class ImageProcess : public QObject
{
	Q_OBJECT

public:
	ImageProcess(QObject *parent = NULL);
	~ImageProcess();

private:
	QWidget* m_cubikglwidget;
	QTimer* m_timer;
	CvCapture* m_capture;
	IplImage* m_frame;
	int m_frameWidth;
	int m_frameHeight;
	CvSize m_framesize;
	IplImage* m_graydst;
	IplImage* m_rgbdst;
	HandDetector* m_handDetector;

	CvSeq* m_bigContour;

	vector<CvPoint*> m_tipPts;
	vector<CvPoint*> m_foldPts;
	vector<float> m_depths;
	vector<CvPoint*> m_fingerTips;

	CvPoint m_cogPt;
	int m_contourAxisAngle;

	int m_fingerTipResult;

	bool isHandFistPressd;
	bool isHandPalmPressd;
	bool isConnect;
	void getCubikglWidget();
signals:
	void showRgbImage(IplImage* image);
	void showGrayImage(IplImage* image);
	
private slots:
	void captureFrame();
public slots:
	void setconnect();
	void setdisconnect();
};

#endif // IMAGEPROCESS_H
