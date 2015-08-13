#ifndef OPENCVGLWIDGET_H
#define OPENCVGLWIDGET_H

#include <QGLWidget>
#include <opencv/cv.h>

class OpencvGLWidget : public QGLWidget
{
	Q_OBJECT

public:
	OpencvGLWidget(QWidget *parent = NULL);
	~OpencvGLWidget();

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int w, int h);

private:
	int m_width;
	int m_height;
	int channel;
	IplImage* m_rgbImage;
	IplImage* m_grayImage;
	bool imagetoshowcreated;

private slots:
	void updateImage(IplImage* image);
};

#endif // OPENCVGLWIDGET_H
