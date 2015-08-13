#ifndef HANDDETECTOR_H
#define HANDDETECTOR_H

#include <QObject>
#include <opencv/cv.h>
#include <vector>
#include <deque>
#include <fstream>
#include <cmath>

#define SMALLEST_AREA 600.0f		// was 100.0f, ignore smaller contour areas
#define MAX_POINTS 20				// max number of points stored in an array
#define MIN_FINGER_DEPTH 20			// used for simplifying the defects list
#define MAX_FINGER_ANGLE 70			// degrees
#define FINGER_DEQUE_NUM 10			// the number of fingertipDeque which store the number of fingertip
#define FINGER_DEQUE_COUNT 8
using namespace std;

class HandDetector : public QObject
{
	Q_OBJECT  

public:
	HandDetector(QObject *parent = NULL);
	~HandDetector();
	void colorRegulate(IplImage* src,IplImage* dst);
	void skinDetect(IplImage* src, IplImage* dst);
	void findBigContour(IplImage* src, CvSeq* (&contour), CvMemStorage* storage);
	void findBigContour2(IplImage* src, CvSeq* (&contour), CvMemStorage* storage);
	void findFingerTips(CvSeq * bigContour, CvMemStorage *approxStorage, 
		CvMemStorage *hullStorage, CvMemStorage *defectsStorage, vector<CvPoint*> *tipPts,
		vector<CvPoint*> *foldPts, vector<float> *depths);
	void reduceFingerTips(const vector<CvPoint*> tipPts, const vector<CvPoint*> foldPts, 
		const vector<float> depths, vector <CvPoint*>* fingerTips);
	int angleBetween(const CvPoint* tip, const CvPoint* next, const CvPoint* prev);
	void getContourInfo(CvSeq* (&bigContour), CvPoint &cogPt, int &contourAxisAngle, const vector <CvPoint*> fingerTips);
	int calculateTilt(double m11, double m20, double m02);
	int getFingerTipsResult(int fingerTipsize);
	int analyseFingerTipDeque();
private:
	deque<int> fingertipDeque;
	
};

#endif // HANDDETECTOR_H
