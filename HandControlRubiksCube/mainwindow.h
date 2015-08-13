#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_mainwindow.h"
#include "cubikglwidget.h"
#include "opencvglwidget.h"
#include "imageprocess.h"

class QSplitter;
class QLabel;
class QHBoxLayout;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MainWindow();

private:
	Ui::MainWindowClass ui;
	//QSize eachWidgetSize;
	QSplitter* h1Splitter;
	QSplitter* h2Splitter;
	QSplitter* vSplitter;
	CubikGLWidget* cubikGLWidget;
	OpencvGLWidget* rgbGLWidget;
	OpencvGLWidget* grayGLWidget;
	QLabel* resultLabel;
	QHBoxLayout* layout;
	QWidget* centreWidget;
	ImageProcess* m_imageProcess;
};

#endif // MAINWINDOW_H
