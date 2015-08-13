#include "mainwindow.h"

#include <QSplitter>
#include <QLabel>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	//eachWidgetSize = QSize(this->width() / 2, this->height() / 2);

	cubikGLWidget = new CubikGLWidget();
	cubikGLWidget->setObjectName("m_cubikGLWidget");
	//cubikGLWidget->resize(eachWidgetSize);

	resultLabel = new QLabel();
	//resultLabel->resize(eachWidgetSize);

	rgbGLWidget = new OpencvGLWidget();
	//rgbGLWidget->resize(eachWidgetSize);

	grayGLWidget = new OpencvGLWidget();
	//grayGLWidget->resize(eachWidgetSize);

	h1Splitter = new QSplitter();
	h2Splitter = new QSplitter();
	h1Splitter->addWidget(cubikGLWidget);
	h1Splitter->addWidget(resultLabel);
	h2Splitter->addWidget(rgbGLWidget);
	h2Splitter->addWidget(grayGLWidget);

	//h1Splitter->addWidget(cubikGLWidget);
	//h1Splitter->addWidget(rgbGLWidget);

	vSplitter = new QSplitter();
	vSplitter->setOrientation(Qt::Vertical);
	vSplitter->addWidget(h1Splitter);
	vSplitter->addWidget(h2Splitter);

	layout = new QHBoxLayout();
	layout->addWidget(vSplitter);
	/*layout->addWidget(h1Splitter);*/

	centreWidget = new QWidget();
	centreWidget->setLayout(layout);
	
	m_imageProcess = new ImageProcess();
	connect(m_imageProcess, SIGNAL(showRgbImage(IplImage*)), rgbGLWidget, SLOT(updateImage(IplImage*)));
	connect(m_imageProcess, SIGNAL(showGrayImage(IplImage*)), grayGLWidget, SLOT(updateImage(IplImage*)));
	connect(ui.actionConnect, SIGNAL(triggered()), m_imageProcess, SLOT(setconnect()));
	connect(ui.actionDisconnect, SIGNAL(triggered()), m_imageProcess, SLOT(setdisconnect()));
	setCentralWidget(centreWidget);
	setWindowTitle(tr("Hand Control Rubik's Cube"));
	resize(1310, 580);
}

MainWindow::~MainWindow()
{
	delete m_imageProcess;
	delete cubikGLWidget;
	delete rgbGLWidget;
	delete grayGLWidget;
	delete resultLabel;
	delete h1Splitter;
	delete h2Splitter;
	delete vSplitter;
	delete layout;
	delete centreWidget;
}

