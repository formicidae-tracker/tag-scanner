#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "Camera.hpp"
#include "ApriltagDetector.hpp"

#include <iostream>

#include <QFileDialog>
#include <QApplication>

#include <opencv2/imgproc.hpp>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, d_ui(new Ui::MainWindow)
	, d_camera(nullptr)
	, d_detector(new ApriltagDetector(this)){
    d_ui->setupUi(this);

    auto cameras = CVCamera::Enumerate();
    for ( const auto & [i,name] : cameras ) {
	    auto loadCameraAction = new QAction(tr("Load camera %1").arg(name.c_str()),this);
	    connect(loadCameraAction,&QAction::triggered,
	            [this,i]() {
		            setCamera(new CVCamera(i,this));
	            });
	    d_ui->menuDevices->addAction(loadCameraAction);

    }

    d_ui->apriltagSettings->setup(d_detector);

}

MainWindow::~MainWindow() {
    delete d_ui;
}


void MainWindow::setCamera(Camera * camera) {
	if ( !d_camera ) {
		delete d_camera;
		d_camera = nullptr;
	}

	d_camera = camera;
	d_lastDetection.reset();
	d_lastDetectionCount = 0;
	d_ui->cameraSettings->setCamera(camera);

	if ( d_camera == nullptr ) {
		return;
	}

	connect(camera,&Camera::newFrame,
	        d_detector,&ApriltagDetector::processFrame,
	        Qt::QueuedConnection);
	connect(d_detector,&ApriltagDetector::frameProcessed,
	        this,
	        [this] ( cv::Mat frame, Detection::Ptr detection) {
		        static size_t i = 0;
		        cv::cvtColor(frame,frame,cv::COLOR_BGR2RGB);
		        d_ui->liveView->setPixmap(QPixmap::fromImage(QImage(frame.data,frame.cols,frame.rows,frame.step,QImage::Format_RGB888)));
		        if (!detection) {
			        d_lastDetection.reset();
			        d_lastDetectionCount = 0;
		        } else if ( !d_lastDetection ) {
			        d_lastDetection = detection;
			        d_lastDetectionCount = 1;
		        } else {
			        if ( detection->TagID == d_lastDetection->TagID ) {
				        if (++d_lastDetectionCount >= 4 ) {
					        QApplication::beep();
					        d_camera->stop();
					        d_lastDetectionCount = 1;
				        }
			        } else {
				        d_lastDetection = detection;
				        d_lastDetectionCount = 1;
			        }
		        }
	        });

	d_camera->start();

}

void MainWindow::on_actionLoadImage_triggered() {
	auto filename = QFileDialog::getOpenFileName(this,
	                                             tr("Open an image file"),
	                                             QString(),
	                                             tr("Images (*.png *.jpg)"));
	if (filename.isEmpty()) {
		return;
	}
	try {
		setCamera(new StubCamera(filename.toUtf8().constData(),this));
	} catch ( const std::exception & e)  {
		d_ui->statusbar->showMessage(tr("Could not open %1: %2").arg(filename).arg(e.what()),2000);
	}
}
