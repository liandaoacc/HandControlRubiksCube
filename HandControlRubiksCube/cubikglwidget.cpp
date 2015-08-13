#include "cubikglwidget.h"
#include <QTextStream>
#include <QDebug>

CubikGLWidget::CubikGLWidget(QWidget *parent)
	: QGLWidget(parent)
{
	installEventFilter(this);
	timer = new QTimer(this);
	needrotation = false;
	connect(timer, SIGNAL(timeout()), this, SLOT(nextframe()));
	timer->start(30);
}

CubikGLWidget::~CubikGLWidget()
{

}

void CubikGLWidget::initializeGL()
{
	glClearColor(0.03,0.2,0.03,0);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
}

void CubikGLWidget::resizeGL( int width, int height )
{
	glViewport(0,0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// gluPerspective(45,float(width)/(height+1),1,20);
	GLfloat zNear = 1.0;
	GLfloat zFar = 20.0;
	GLfloat aspect = (GLfloat)width/(GLfloat)(height+1);
	GLfloat fH = tan(GLfloat(90.0/360.0*3.14159))*zNear;
	GLfloat fW = fH * aspect;
	glFrustum(-fW, fW, -fH, fH, zNear, zFar);

	glMatrixMode(GL_MODELVIEW);
}

void CubikGLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslated(0.0, 0.0, -11.0);
	glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
	glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
	glRotated(zRot / 16.0, 0.0, 0.0, 1.0);
	cubik.draw();
}

void CubikGLWidget::normalizeAngle( int *angle )
{
	while (*angle < 0)
		*angle += 360 * 16;
	while (*angle > 360 * 16)
		*angle -= 360 * 16;
}

void CubikGLWidget::setXRotation( int angle )
{
	normalizeAngle(&angle);
	if (angle != xRot) {
		xRot = angle;
		updateGL();
	}
}

void CubikGLWidget::setYRotation( int angle )
{
	normalizeAngle(&angle);
	if (angle != yRot) {
		yRot = angle;
		updateGL();
	}
}

void CubikGLWidget::setZRotation( int angle )
{
	normalizeAngle(&angle);
	if (angle != zRot) {
		zRot = angle;
		updateGL();
	}
}

void CubikGLWidget::mousePressEvent( QMouseEvent *event )
{
	mouseLastPos=event->pos();
}

void CubikGLWidget::mouseMoveEvent( QMouseEvent *event )
{
	int dx = event->x() - mouseLastPos.x();
	int dy = event->y() - mouseLastPos.y();

	if (event->buttons() & Qt::LeftButton) {
		setXRotation(xRot + 8 * dy);
		setYRotation(yRot + 8 * dx);
	} else if (event->buttons() & Qt::RightButton) {
		setXRotation(xRot + 8 * dy);
		setZRotation(zRot + 8 * dx);
	}
	mouseLastPos = event->pos();
}

bool CubikGLWidget::eventFilter( QObject* watched, QEvent* event )
{
	//if (event->type() == HandFistPress) 
	//{
	//	qDebug() << "event() is dispatching HandFistPress.";
	//	handFistPressEvent((HandEvent*)event);
	//	if (event->isAccepted())
	//	{
	//		qDebug() << "event() dispatch successfully.";
	//		return true;
	//	} 
	//	else
	//	{
	//		qDebug() << "event() does not dispatch.";
	//		return false;
	//	}
	//} 
	//else 
	//{
	//	// pass the event on to the parent class
	//	return QGLWidget::eventFilter(watched, event);
	//}

	switch(event->type())
	{
	case HandFistPress:
		qDebug() << "HandFistPress is dispatching HandFistPress.";
		handFistPressEvent((HandEvent*)event);
		if (event->isAccepted())
		{
			qDebug() << "HandFistPress dispatch successfully.";
			return true;
		} 
		else
		{
			qDebug() << "HandFistPress does not dispatch.";
			return false;
		}
		break;

	case HandFistMove:
		qDebug() << "HandFistMove is dispatching HandFistPress.";
		handFistMoveEvent((HandEvent*)event);
		if (event->isAccepted())
		{
			qDebug() << "HandFistMove dispatch successfully.";
			return true;
		} 
		else
		{
			qDebug() << "HandFistMove does not dispatch.";
			return false;
		}
		break;

	case HandPalmPress:
		qDebug() << "HandPalmPress is dispatching HandFistPress.";
		handPalmPressEvent((HandEvent*)event);
		if (event->isAccepted())
		{
			qDebug() << "HandPalmPress dispatch successfully.";
			return true;
		} 
		else
		{
			qDebug() << "HandPalmPress does not dispatch.";
			return false;
		}
		break;

	case HandPalmMove:
		qDebug() << "HandPalmMove is dispatching HandFistPress.";
		handPalmMoveEvent((HandEvent*)event);
		if (event->isAccepted())
		{
			qDebug() << "HandPalmMove dispatch successfully.";
			return true;
		} 
		else
		{
			qDebug() << "HandPalmMove does not dispatch.";
			return false;
		}
		break;

	case HandOneFinger:
		qDebug() << "HandOneFinger is dispatching HandFistPress.";
		handOneFingerEvent((HandEvent*)event);
		if (event->isAccepted())
		{
			qDebug() << "HandOneFinger dispatch successfully.";
			return true;
		} 
		else
		{
			qDebug() << "HandOneFinger does not dispatch.";
			return false;
		}
		break;

	case HandTwoFinger:
		qDebug() << "HandTwoFinger is dispatching HandFistPress.";
		handTwoFingerEvent((HandEvent*)event);
		if (event->isAccepted())
		{
			qDebug() << "HandTwoFinger dispatch successfully.";
			return true;
		} 
		else
		{
			qDebug() << "HandTwoFinger does not dispatch.";
			return false;
		}
		break;

	case HandThreeFinger:
		qDebug() << "HandThreeFinger is dispatching HandFistPress.";
		handThreeFingerEvent((HandEvent*)event);
		if (event->isAccepted())
		{
			qDebug() << "HandThreeFinger dispatch successfully.";
			return true;
		} 
		else
		{
			qDebug() << "HandThreeFinger does not dispatch.";
			return false;
		}
		break;
	default:
		// pass the event on to the parent class
		return QGLWidget::eventFilter(watched, event);
	}
}

void CubikGLWidget::handFistPressEvent( HandEvent* event )
{
	handLastPos = event->pos();
	startrotation();
	needrotation = false;
	event->accept();
}

void CubikGLWidget::handFistMoveEvent( HandEvent* event )
{
	int dx = event->x() - handLastPos.x();
	int dy = event->y() - handLastPos.y();
	setXRotation(xRot + 8 * dy);
	setYRotation(yRot + 8 * dx);
	handLastPos = event->pos();
	event->accept();
}

void CubikGLWidget::handPalmPressEvent( HandEvent* event )
{
	handLastPos = event->pos();
	event->accept();
}

void CubikGLWidget::handPalmMoveEvent( HandEvent* event )
{
	int dx = event->x() - handLastPos.x();
	int dy = event->y() - handLastPos.y();
	setXRotation(xRot + 8 * dy);
	setZRotation(zRot + 8 * dx);
	handLastPos = event->pos();
	event->accept();
}

void CubikGLWidget::handOneFingerEvent( HandEvent* event )
{
	int dx = event->x();
	int dy = event->y();
	fwaxis();
	selectlayout(dx);
	needrotation = true;
	event->accept();
}

void CubikGLWidget::handTwoFingerEvent( HandEvent* event )
{
	int dx = event->x();
	int dy = event->y();
	sdaxis();
	selectlayout(dx);
	needrotation = true;
	event->accept();
}

void CubikGLWidget::handThreeFingerEvent( HandEvent* event )
{
	int dx = event->x();
	int dy = event->y();
	upaxis();
	selectlayout(dx);
	needrotation = true;
	event->accept();
}

void CubikGLWidget::nextframe()
{
	cubik.animationframe();
	updateGL();
}

void CubikGLWidget::selectlayout(int layout)
{
	cubik.selh(layout);
	updateGL();
}

void CubikGLWidget::fwaxis()
{
	cubik.selax(FD);
	updateGL();
}

void CubikGLWidget::sdaxis()
{
	cubik.selax(SD);
	updateGL();
}

void CubikGLWidget::upaxis()
{
	cubik.selax(UP);
	updateGL();
}

void CubikGLWidget::startrotation()
{
	cubik.startanimation();
	updateGL();
}

void CubikGLWidget::randcubik()
{
	cubik.randomize();
	updateGL();
}


