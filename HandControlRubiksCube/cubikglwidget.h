#ifndef CUBIKGLWIDGET_H
#define CUBIKGLWIDGET_H

#include <QGLWidget>
#include <QMouseEvent>
#include <QTimer>
#include "cubik.h"
#include "handevent.h"

class CubikGLWidget : public QGLWidget
{
	Q_OBJECT

public:
	CubikGLWidget(QWidget *parent=0);
	~CubikGLWidget();

	void setXRotation(int angle);
	void setYRotation(int angle);
	void setZRotation(int angle);

protected:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();

	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

	bool eventFilter (QObject* watched, QEvent* event);
	void handFistPressEvent(HandEvent* event);
	void handFistMoveEvent(HandEvent* event);
	void handPalmPressEvent(HandEvent* event);
	void handPalmMoveEvent(HandEvent* event);
	void handOneFingerEvent(HandEvent* event);
	void handTwoFingerEvent(HandEvent* event);
	void handThreeFingerEvent(HandEvent* event);
private:
	Cubik cubik;
	int xRot;
	int yRot;
	int zRot;
	QPoint mouseLastPos;
	QPoint handLastPos;
	QTimer* timer;
	bool needrotation;
	void normalizeAngle(int *angle);

public slots:
	void selectlayout(int layout);
	void fwaxis();
	void sdaxis();
	void upaxis();
	void startrotation();
	void randcubik();

private slots:
	void nextframe();
};

#endif // CUBIKGLWIDGET_H
