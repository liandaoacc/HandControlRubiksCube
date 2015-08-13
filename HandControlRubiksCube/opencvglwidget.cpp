#include "opencvglwidget.h"


OpencvGLWidget::OpencvGLWidget(QWidget *parent)
	: QGLWidget(parent)
{
	m_width = 0;
	m_height = 0;
	m_rgbImage = NULL;
	m_grayImage = NULL;
	imagetoshowcreated = false;
}

OpencvGLWidget::~OpencvGLWidget()
{
	cvReleaseImage(&m_rgbImage);
	cvReleaseImage(&m_grayImage);
}

void OpencvGLWidget::initializeGL()
{

}

void OpencvGLWidget::paintGL()
{	
	//Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2i(0, 0);
	//Draw image from OpenCV capture
	switch(channel)
	{
	case 1:
		glDrawPixels(m_width, m_height, GL_LUMINANCE, GL_UNSIGNED_BYTE, m_grayImage->imageData);
		break;
	case 2:
		break;
	case 3:
		glDrawPixels(m_width, m_height, GL_BGR_EXT, GL_UNSIGNED_BYTE, m_rgbImage->imageData);
		break;
	case 4:
		break;
	default:
		break;
	}
}

void OpencvGLWidget::resizeGL( int w, int h )
{
	if(h == 0)
		h = 1;
	//Adjust the viewport
	glViewport(0, 0, m_width, m_height);	
	//Adjust the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, m_width, 0, m_height, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void OpencvGLWidget::updateImage(IplImage* image)
{
	channel = image->nChannels;
	m_width = image->width;
	m_height = image->height;

	if (!imagetoshowcreated)
	{
		m_rgbImage = cvCreateImage(cvSize(m_width, m_height), IPL_DEPTH_8U, 3);
		m_grayImage = cvCreateImage(cvSize(m_width, m_height), IPL_DEPTH_8U, 1);
		imagetoshowcreated = true;
	}

	if (channel == 1)
	{
		cvCopy(image, m_grayImage);
	} 
	else
	{
		cvCopy(image, m_rgbImage);
	}
	
	glDraw();
}

