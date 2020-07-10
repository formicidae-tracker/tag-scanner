#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "Camera.hpp"

#include <iostream>

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, d_ui(new Ui::MainWindow)
	, d_camera(nullptr) {
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
	d_ui->settings->setCamera(camera);

	if ( d_camera == nullptr ) {
		return;
	}



}

void MainWindow::on_actionLoadImage_triggered() {
	auto filename = QFileDialog::getOpenFileName(this,
	                                             tr("Open an image file"),
	                                             QString(),
	                                             tr("Images (*.png *.jpg)"));
	if (filename.isEmpty()) {
		return;
	}
	setCamera(new StubCamera(filename.toUtf8().constData(),this));
}
