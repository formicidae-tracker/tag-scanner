#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "DetectionView.hpp"

#include <iostream>

#include <QFileDialog>
#include <QApplication>
#include <QShortcut>
#include <QCloseEvent>
#include <QSettings>
#include <QMessageBox>
#include <QCamera>
#include <QCameraInfo>

#include <opencv2/imgproc.hpp>

#include <fort/myrmidon/Time.hpp>

#include <fstream>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, d_ui(new Ui::MainWindow)
	, d_camera(nullptr)
	, d_detectionView(new DetectionView(this))
	, d_needSave(false)
	, d_cameraLoaded(false) {
    d_ui->setupUi(this);

    auto cameraInfos = QCameraInfo::availableCameras();
    for ( const auto & cInfo : cameraInfos ) {
	    auto loadCameraAction = new QAction(tr("Load camera %1").arg(cInfo.deviceName()),this);
	    connect(loadCameraAction,&QAction::triggered,
	            [this,cInfo]() {
		            setCamera(new QCamera(cInfo,this));
	            });
	    d_ui->menuDevices->addAction(loadCameraAction);
    }

    d_detectionView->setApriltagSettings(d_ui->apriltagSettings);
    d_detectionView->setView(d_ui->liveView);


    auto togglePlayPauseShortcut = new QShortcut(tr("Space"),this);
    connect(togglePlayPauseShortcut,&QShortcut::activated,
            this,&MainWindow::toggleDetection);
    connect(d_ui->detectButton,&QPushButton::clicked,
            this,&MainWindow::toggleDetection);
    connect(d_detectionView,&DetectionView::detectionActiveChanged,
            this,[this](bool active) {
	                 d_ui->detectButton->setText(active ? tr("Pause Detection") : tr("Detect"));
                 });

    d_ui->detectButton->setEnabled(false);

    connect(d_ui->actionQuit,&QAction::triggered,
            [this]() { close(); });

	connect(d_detectionView,&DetectionView::newTag,
	        this,&MainWindow::onNewTag,
	        Qt::QueuedConnection);


    d_ui->actionSaveDataAsCSV->setEnabled(false);

    loadSettings();
    on_actionUnloadMyrmidonFile_triggered();
}

MainWindow::~MainWindow() {
    delete d_ui;
}


void MainWindow::setCamera(QCamera * camera) {
	if ( d_camera != nullptr ) {
		delete d_camera;
		d_camera = nullptr;
	}
	d_cameraLoaded = false;
	d_camera = camera;

	if ( d_camera == nullptr ) {
		d_ui->detectButton->setEnabled(false);
		return;
	}

	connect(d_camera,&QCamera::statusChanged,
	        this,[this](QCamera::Status status) {
		            if ( status != QCamera::LoadedStatus || d_cameraLoaded == true) {
			            return;
		            }
		            d_cameraLoaded = true;
		            d_ui->cameraSettings->setCamera(d_camera);
		            d_camera->start();
	            });

	d_ui->detectButton->setEnabled(true);
	d_camera->setViewfinder(d_detectionView);
	d_camera->load();
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
		setCamera(nullptr);
	} catch ( const std::exception & e)  {
		d_ui->statusbar->showMessage(tr("Could not open %1: %2").arg(filename).arg(e.what()),2000);
	}
}


void MainWindow::toggleDetection() {
	if ( d_camera == nullptr ) {
		return;
	}

	d_detectionView->setDetectionActive(!d_detectionView->isDetectionActive());
}


void MainWindow::on_actionSaveDataAsCSV_triggered() {
	auto filename = QFileDialog::getSaveFileName(this,tr("Save data"),
	                                             "",
	                                             tr("CSV (*.csv)"));

	if ( filename.isEmpty() ) {
		return;
	}

	std::ofstream file(filename.toUtf8().constData());
	file << "#Time,TagID,AntID,Comment" << std::endl;
	for ( size_t i = 0; i < d_ui->tableWidget->rowCount(); ++i) {
		file << "\"" \
		     << d_ui->tableWidget->item(i,0)->text().toUtf8().constData()
		     << "\",\""
		     << d_ui->tableWidget->item(i,1)->text().toUtf8().constData()
		     << "\",\""
		     << d_ui->tableWidget->item(i,2)->text().toUtf8().constData()
		     << "\",\""
		     << d_ui->tableWidget->item(i,3)->text().toUtf8().constData()
		     << "\"" << std::endl;
	}

	d_needSave = false;
	d_ui->actionSaveDataAsCSV->setEnabled(false);

}

void MainWindow::closeEvent(QCloseEvent *event) {
	if ( maybeSave() ) {
		saveSettings();
		event->accept();
	} else {
		event->ignore();
	}
}


bool MainWindow::maybeSave() {
	if ( d_needSave == false ) {
		return true;
	}

	const QMessageBox::StandardButton ret
        = QMessageBox::warning(this, tr("FORT Tag Scanner"),
                               tr("New scanned data has not been saved yet.\n"
                                  "Do you want to save the acquired scans?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Save:
        on_actionSaveDataAsCSV_triggered();
        return d_needSave == false;
    case QMessageBox::Cancel:
        return false;
    case QMessageBox::Discard:
	    return true;
    default:
        break;
    }
    return false;
}

void MainWindow::loadSettings() {
	QSettings settings;
	auto geometry = settings.value("geometry", QByteArray()).toByteArray();
	if ( geometry.isEmpty() == false ) {
		restoreGeometry(geometry);
	}
	d_ui->apriltagSettings->loadSettings();

}

void MainWindow::saveSettings() {
	QSettings settings;
	settings.setValue("geometry", saveGeometry());
	d_ui->apriltagSettings->saveSettings();
}


void MainWindow::on_actionLoadMyrmidonFile_triggered() {
	if ( !d_trackingSolver == false ) {
		on_actionUnloadMyrmidonFile_triggered();
	}

	auto filename = QFileDialog::getOpenFileName(this,
	                                             tr("Open a myrmidon file"),
	                                             QString(),
	                                             tr("Images (*.myrmidon)"));

	if ( filename.isEmpty() ) {
		return;
	}
	using namespace fort::myrmidon;
	try {
		auto experiment = std::make_shared<CExperiment>(Experiment::OpenDataLess(filename.toUtf8().constData()));
		d_trackingSolver = std::make_shared<TrackingSolver>(experiment->CompileTrackingSolver());
		d_ui->myrmidonFileLabel->setText(tr("File loaded: %1").arg(experiment->AbsoluteFilePath().c_str()));
		d_ui->actionUnloadMyrmidonFile->setEnabled(true);
		d_ui->myrmidonButton->setText(tr("Unload Myrmidon File"));
	} catch ( const std::exception & e) {
		QMessageBox::warning(this,tr("Could not open file"),tr("Could not open file %1: %2").arg(filename).arg(e.what()));
		return;
	}

}

void MainWindow::on_actionUnloadMyrmidonFile_triggered() {
	d_trackingSolver.reset();
		d_ui->myrmidonFileLabel->setText(tr("File loaded: None"));
		d_ui->actionUnloadMyrmidonFile->setEnabled(false);
		d_ui->myrmidonButton->setText(tr("Load Myrmidon File"));

}

void MainWindow::on_myrmidonButton_clicked() {
	if ( !d_trackingSolver == true ) {
		on_actionLoadMyrmidonFile_triggered();
	} else {
		on_actionUnloadMyrmidonFile_triggered();
	}
}

void MainWindow::onNewTag(quint32 tagID) {
	QApplication::beep();
	auto now = fort::myrmidon::Time::Now();
	std::ostringstream nowStr;
	nowStr << now.Round(fort::myrmidon::Duration::Second);
	auto tagStr = fort::myrmidon::FormatTagID(tagID);

	auto row = d_ui->tableWidget->rowCount();
	d_ui->tableWidget->insertRow(row);
	d_ui->tableWidget->setItem(row,0,new QTableWidgetItem(nowStr.str().c_str()));
	d_ui->tableWidget->setItem(row,1,new QTableWidgetItem(tagStr.c_str()));

	if ( !d_trackingSolver == true ) {
		d_ui->tableWidget->setItem(row,2,new QTableWidgetItem(""));
	} else {
		auto antID = d_trackingSolver->IdentifyTag(tagID,now);
		QString antIDStr;
		if ( antID != 0 ) {
			antIDStr = QString::number(int(antID));
		}

		d_ui->tableWidget->setItem(row,2,new QTableWidgetItem(antIDStr));
	}

	d_ui->tableWidget->setItem(row,3,new QTableWidgetItem(""));
	d_needSave = true;
	d_ui->actionSaveDataAsCSV->setEnabled(true);
}
