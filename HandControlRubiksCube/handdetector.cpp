#include "handdetector.h"
#include <QMessageBox>
HandDetector::HandDetector(QObject *parent)
	: QObject(parent)
{
	
}

HandDetector::~HandDetector()
{

}
// 效果不好
void HandDetector::colorRegulate(IplImage* src,IplImage* dst)
{
	IplImage* R=cvCreateImage(cvGetSize(src),8,1);
	IplImage* G=cvCreateImage(cvGetSize(src),8,1);
	IplImage* B=cvCreateImage(cvGetSize(src),8,1);

	cvCvtPixToPlane(src,R,G,B,0);

	CvScalar aveR=cvAvg(R,NULL);
	CvScalar aveG=cvAvg(G,NULL);
	CvScalar aveB=cvAvg(B,NULL);

	float aGray=(float)(aveR.val[0]+aveG.val[0]+aveB.val[0])/3;
	float aR=(float)(aGray/aveR.val[0]);
	float aG=(float)(aGray/aveG.val[0]);
	float aB=(float)(aGray/aveB.val[0]);


	int width=src->width;//IplImage这个结构，看一下就明白了
	int height=src->height;
	int stepR=R->widthStep/sizeof(uchar);
	int stepG=G->widthStep/sizeof(uchar);
	int stepB=B->widthStep/sizeof(uchar);
	uchar* dataR=(uchar*)R->imageData;
	uchar* dataG=(uchar*)G->imageData;
	uchar* dataB=(uchar*)B->imageData;

	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			dataR[i*stepR+j]=(unsigned char)(aR*dataR[i*stepR+j]);
			dataG[i*stepG+j]=(unsigned char)(aG*dataG[i*stepG+j]);
			dataB[i*stepB+j]=(unsigned char)(aB*dataB[i*stepB+j]);
		}
	}

	cvMerge(R,G,B,0,dst);

	cvReleaseImage(&R);
	cvReleaseImage(&G);
	cvReleaseImage(&B);
}

void HandDetector::skinDetect( IplImage* src, IplImage* dst )
{
	assert(dst->nChannels == 1);
	assert(src->nChannels == 3);

	CvSize srcSize = cvGetSize(src);
	IplImage* src_tmp1 = cvCreateImage(srcSize, IPL_DEPTH_8U ,3);

	//用于存图像的一个中间变量，是用来分通道用的，分成hsv通道
	IplImage* hsv = cvCreateImage(srcSize, IPL_DEPTH_8U, 3);
	IplImage* H = cvCreateImage(srcSize, IPL_DEPTH_8U, 1);
	IplImage* S = cvCreateImage(srcSize, IPL_DEPTH_8U, 1);
	IplImage* V = cvCreateImage(srcSize, IPL_DEPTH_8U, 1);

	//通道的中间变量，用于肤色检测的中间变量
	IplImage* tmpH1 = cvCreateImage(srcSize, IPL_DEPTH_8U, 1);
	IplImage* tmpS1 = cvCreateImage(srcSize, IPL_DEPTH_8U, 1);
	IplImage* tmpH2 = cvCreateImage(srcSize, IPL_DEPTH_8U, 1);
	IplImage* tmpS2 = cvCreateImage(srcSize, IPL_DEPTH_8U, 1);
	IplImage* tmpH3 = cvCreateImage(srcSize, IPL_DEPTH_8U, 1);
	IplImage* tmpS3 = cvCreateImage(srcSize, IPL_DEPTH_8U, 1);


	//高斯模糊
	cvSmooth(src, src_tmp1, CV_GAUSSIAN, 3, 3);
	//颜色转换
	cvCvtColor(src_tmp1, hsv, CV_BGR2HSV);
	//分为3个通道
	cvSplit(hsv, H, S, V, 0);

	///*********************肤色检测部分**************/
	// 阈值法
	cvInRangeS(H, cvScalar(0.0 ,0.0, 0,0), cvScalar(20.0, 0.0, 0,0), tmpH1);
	cvInRangeS(S, cvScalar(75.0, 0.0, 0,0), cvScalar(200.0, 0.0, 0,0), tmpS1);
	cvAnd(tmpH1, tmpS1, tmpH1, 0);

	// Red Hue with Low Saturation
	// Hue 0 to 26 degree and Sat 20 to 90
	cvInRangeS(H, cvScalar(0.0 ,0.0, 0,0), cvScalar(13.0, 0.0, 0,0), tmpH2); 
	cvInRangeS(S, cvScalar(20.0, 0.0, 0,0), cvScalar(90.0, 0.0, 0,0), tmpS2);
	cvAnd(tmpH2, tmpS2, tmpH2, 0);

	// Red Hue to Pink with Low Saturation
	// Hue 340 to 360 degree and Sat 15 to 90
	cvInRangeS(H, cvScalar(170.0, 0.0, 0,0), cvScalar(180.0, 0.0, 0,0), tmpH3);
	cvInRangeS(S, cvScalar(15.0, 0.0, 0,0), cvScalar(90.0 ,0.0, 0,0), tmpS3);
	cvAnd(tmpH3, tmpS3, tmpH3, 0);

	//// Combine the Hue and Sat detections
	cvOr(tmpH3, tmpH2, tmpH2, 0);
	cvOr(tmpH1, tmpH2, tmpH1, 0);

	cvCopy(tmpH1, dst);

	cvReleaseImage(&src_tmp1);
	cvReleaseImage(&hsv);
	cvReleaseImage(&H);
	cvReleaseImage(&S);
	cvReleaseImage(&V);
	cvReleaseImage(&tmpH1);
	cvReleaseImage(&tmpS1);
	cvReleaseImage(&tmpH2);
	cvReleaseImage(&tmpS2);
	cvReleaseImage(&tmpH3);
	cvReleaseImage(&tmpS3);
}

void HandDetector::findBigContour( IplImage* src, CvSeq* (&contour), CvMemStorage* storage )
{
	CvSeq* contour_tmp = NULL;
	CvSeq* contourPos;
	int contourcount = cvFindContours (src, storage, &contour_tmp, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_NONE );
	if(contourcount == 0)
		return;
	CvRect bndRect = cvRect(0, 0, 0, 0);
	double contourArea, maxcontArea = 0;
	for( ; contour_tmp != 0; contour_tmp = contour_tmp->h_next )
	{
		bndRect = cvBoundingRect( contour_tmp, 0 );
		contourArea = bndRect.width * bndRect.height;
		if(contourArea >= maxcontArea)//提取最大轮廓
		{
			maxcontArea = contourArea;
			contourPos = contour_tmp;
		}
	}
	contour = contourPos;
}

void HandDetector::findBigContour2( IplImage* src, CvSeq* (&contour), CvMemStorage* storage )
{
	CvSeq* contour_tmp;
	CvSeq* contourPos;
	int contourcount = cvFindContours (src, storage, &contour_tmp, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE );
	if(contourcount == 0)
		return;

	float maxcontArea = SMALLEST_AREA;

	for( ; contour_tmp != 0; contour_tmp = contour_tmp->h_next )
	{
		if (contour_tmp->elem_size > 0) 
		{
			CvBox2D box = cvMinAreaRect2(contour_tmp, storage);
			CvSize2D32f size = box.size;
			float contourArea = size.width * size.height;
			if(contourArea > maxcontArea)//提取最大轮廓
			{
				maxcontArea = contourArea;
				contourPos = contour_tmp;
			}
		}
	}
	contour = contourPos;
}

void HandDetector::findFingerTips( CvSeq * bigContour, CvMemStorage *approxStorage, CvMemStorage *hullStorage, 
	CvMemStorage *defectsStorage, vector<CvPoint*> *tipPts, vector<CvPoint*> *foldPts, vector<float> *depths )
{
	// reduce number of points in the contour
	// 曲线逼近轮廓
	CvSeq* approxContour = cvApproxPoly(bigContour, sizeof(CvContour), approxStorage, CV_POLY_APPROX_DP, 3, 1);
	// find the convex hull around the contour
	// 查找凸包
	CvSeq* hullSeq = cvConvexHull2(approxContour, hullStorage, CV_COUNTER_CLOCKWISE, 0);
	// find the defect differences between the contour and hull
	// 查找凸包缺陷
	CvSeq* defects = cvConvexityDefects(approxContour, hullSeq, defectsStorage);
	int defectsTotal = defects->total;
	if (defectsTotal > MAX_POINTS)
	{
		/*cout << "Only processing 20 defect points" << endl;*/
		defectsTotal = MAX_POINTS;
	}
	tipPts->clear();
	foldPts->clear();
	depths->clear();
	for (int i = 0; i < defectsTotal; i++)
	{		
		CvConvexityDefect* cdf = (CvConvexityDefect*) cvGetSeqElem(defects, i);
		CvPoint* startPt = cdf->start;
		CvPoint* depthPt = cdf->depth_point;
		tipPts->push_back(startPt);
		foldPts->push_back(depthPt);
		depths->push_back(cdf->depth);
	}
}

void HandDetector::reduceFingerTips( const vector<CvPoint*> tipPts, const vector<CvPoint*> foldPts, 
	const vector<float> depths, vector <CvPoint*>* fingerTips )
{
#ifdef FINGERDATA_OUT
	fstream fs("fingerdata.txt", fstream::out | fstream::app);
	if (!fs.is_open())
	{
		return;
	}

	fs << "tx" << "\t" << "ty" << "\t" << "fx" << "\t" << "fy" << "\t" << "deps" << endl;
#endif

	fingerTips->clear();
	int numPoints = depths.size();

#ifdef FINGERDATA_OUT
	for (int j = 0; j < numPoints; j++)
	{
		fs << tipPts[j]->x << "\t" << tipPts[j]->y << "\t" << foldPts[j]->x << "\t" << foldPts[j]->y << "\t" << depths[j] << endl;
	}

	fs << "deps" << "\t" << "prex" << "\t" << "prey" << "\t"<< "nex" << "\t" << "ney" << "\t" << "ang" << "\t" << "tix" << "\t" << "tiy" << "\t" << "get" << endl; 
#endif

	for (int i = 0; i < numPoints; i++)
	{
		// defect too shallow
		if (depths[i] < MIN_FINGER_DEPTH)
		{
			continue;
		}
		// look at fold points on either side of a tip
		int pdx = (i == 0) ? (numPoints-1) : (i - 1);   // predecessor of i
		int sdx = (i == numPoints-1) ? 0 : (i + 1);     // successor of i
		int angle = angleBetween(tipPts[i], foldPts[pdx], foldPts[sdx]);
#ifdef FINGERDATA_OUT
		fs << depths[i] << "\t" << foldPts[pdx]->x << "\t" << foldPts[pdx]->y << "\t"<< foldPts[sdx]->x << "\t" << foldPts[sdx]->y << "\t" << angle << "\t" << tipPts[i]->x << "\t" << tipPts[i]->y << "\t"; 
		// angle between finger and folds too wide
#endif
		if (angle >= MAX_FINGER_ANGLE)
		{
#ifdef FINGERDATA_OUT
			fs << "false" << endl;
#endif
			continue;
		}
#ifdef FINGERDATA_OUT
		fs << "true" << endl;
#endif
		// this point probably is a finger tip, so add to list
		fingerTips->push_back(tipPts[i]);
	}
#ifdef FINGERDATA_OUT
	fs.close();
#endif
}

int HandDetector::angleBetween( const CvPoint* tip, const CvPoint* prev, const CvPoint* next )
{
	double PI = 3.14;
	double tip_prev2 = (tip->x - prev->x) * (tip->x - prev->x) + (tip->y - prev->y) * (tip->y - prev->y);
	double tip_next2 = (tip->x - next->x) * (tip->x - next->x) + (tip->y - next->y) * (tip->y - next->y);
	double prev_next2 = (prev->x - next->x) * (prev->x - next->x) + (prev->y - next->y) * (prev->y - next->y);
	double cos_tip = (tip_prev2 + tip_next2 - prev_next2) / (2 * sqrt(tip_prev2) * sqrt(tip_next2));
	double theta = acos(cos_tip) * 180.0 / PI;
	int tilt = (int) theta;
	return abs(tilt);
}

void HandDetector::getContourInfo( CvSeq* (&bigContour), CvPoint &cogPt, int &contourAxisAngle, const vector <CvPoint*> fingerTips )
{
#ifdef FINGERDATA_OUT
	fstream fs("fingerdata.txt", fstream::out | fstream::app);
	if (!fs.is_open())
	{
		return;
	}
#endif
	CvMoments moments;
	cvMoments(bigContour, &moments, 1);

	// 质心
	double m00 = cvGetSpatialMoment(&moments, 0, 0);
	double m10 = cvGetSpatialMoment(&moments, 1, 0);
	double m01 = cvGetSpatialMoment(&moments, 0, 1);

	double X = m10 / m00;
	double Y = m01 / m00;
	cogPt.x = (int) X;
	cogPt.y = (int) Y;

	// this angle assumes that the positive y-axis is down the screen 
	double m11 = cvGetCentralMoment(&moments, 1, 1);
	double m20 = cvGetCentralMoment(&moments, 2, 0);
	double m02 = cvGetCentralMoment(&moments, 0, 2);

	contourAxisAngle = calculateTilt(m11, m20, m02);
	int sz = fingerTips.size();
	if (sz > 0)
	{
		int yTotal = 0;
		for (int i = 0; i < sz; i++)
		{
			yTotal += fingerTips[i]->y;
		}
		int avgYFinger = yTotal / sz;
		if (avgYFinger > cogPt.y)
		{
			contourAxisAngle += 180;
		}
	}
	contourAxisAngle = 180 - contourAxisAngle;
#ifdef FINGERDATA_OUT
	fs << "cox" << "\t" << "coy" << "\t" << "contourAxisAngle" << endl;
	fs << cogPt.x << "\t" << cogPt.y << "\t" << contourAxisAngle << endl;
	fs.close();
#endif
}

int HandDetector::calculateTilt( double m11, double m20, double m02 )
{
	double diff = m20 - m02;
	if (diff == 0) 
	{
		if (m11 == 0)
			return 0;
		else if (m11 > 0)
			return 45;
		else   // m11 < 0
			return -45;
	}
	double PI = 3.14;
	double theta = 0.5 * atan2(2 * m11, diff) * 180.0 / PI;
	int tilt = (int) theta;

	if ((diff > 0) && (m11 == 0))
		return 0;
	else if ((diff < 0) && (m11 == 0))
		return -90;
	else if ((diff > 0) && (m11 > 0))  // 0 to 45 degrees
		return tilt;
	else if ((diff > 0) && (m11 < 0))  // -45 to 0
		return (180 + tilt);   // change to counter-clockwise angle measure
	else if ((diff < 0) && (m11 > 0))   // 45 to 90
		return tilt;
	else if ((diff < 0) && (m11 < 0))   // -90 to -45
		return (180 + tilt);  // change to counter-clockwise angle measure

	/*cout << "Error in moments for tilt angle" << endl;*/
	QMessageBox::critical(NULL, "Error", "Error in moments for tilt angle");
	return 0;
}

int HandDetector::getFingerTipsResult( int fingerTipsize )
{
	fingertipDeque.push_back(fingerTipsize);
	int sz = fingertipDeque.size();
	int result = 100;		// 初始化为100,如果还是100就说明没有结果
	if( sz == FINGER_DEQUE_NUM)
	{
		result = analyseFingerTipDeque();
		fingertipDeque.pop_front();
	}
	return result;
}

int HandDetector::analyseFingerTipDeque()
{
	int count=0;
	int tmp;
	int result = 100;		// 初始化为100,如果还是100就说明没有结果
	int sz = fingertipDeque.size();
	for(int i=0; i < sz; i++)
	{
		tmp = fingertipDeque[i];
		for(int j = 0; j < sz; j++)
		{
			if(tmp == fingertipDeque[j])
				count ++;
			if(count > FINGER_DEQUE_COUNT)
			{
				result = fingertipDeque[j];
				break;
			}
		}
		if(count > FINGER_DEQUE_COUNT)
			break;
		count = 0;
	}
	return result;
}
