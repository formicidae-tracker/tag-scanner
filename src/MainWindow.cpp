#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "DetectionProcess.hpp"

#include <iostream>

#include <QApplication>
#include <QCamera>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMediaCaptureSession>
#include <QMediaDevices>
#include <QMessageBox>
#include <QSettings>
#include <QShortcut>
#include <QStandardItemModel>
#include <QVideoSink>
//#include <fort/time/Time.hpp>

#include <fstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , d_ui(new Ui::MainWindow)
    , d_camera(nullptr)
    , d_session(new QMediaCaptureSession(this))
    , d_sink(new QVideoSink(this))
    , d_detectionProcess(new DetectionProcess(this))
    , d_needSave(false)
    , d_cameraLoaded(false) {
	d_ui->setupUi(this);

	d_session->setVideoSink(d_sink);
	connect(
	    d_sink,
	    &QVideoSink::videoFrameChanged,
	    d_detectionProcess,
	    &DetectionProcess::pushNewFrame
	);

	auto cameraInfos = QMediaDevices::videoInputs();
	for (const auto &cInfo : cameraInfos) {
		auto loadCameraAction =
		    new QAction(tr("Use %1").arg(cInfo.description()), this);
		connect(loadCameraAction, &QAction::triggered, [this, cInfo]() {
			setCamera(new QCamera(cInfo, this));
		});
		d_ui->menuDevices->addAction(loadCameraAction);
	}

	d_detectionProcess->setApriltagSettings(d_ui->apriltagSettings);
	d_detectionProcess->setView(d_ui->liveView);

	auto togglePlayPauseShortcut = new QShortcut(tr("Space"), this);
	connect(
	    togglePlayPauseShortcut,
	    &QShortcut::activated,
	    this,
	    &MainWindow::toggleDetection
	);
	connect(
	    d_ui->detectButton,
	    &QPushButton::clicked,
	    this,
	    &MainWindow::toggleDetection
	);
	connect(
	    d_detectionProcess,
	    &DetectionProcess::detectionActiveChanged,
	    this,
	    [this](bool active) {
		    d_ui->detectButton->setText(
		        active ? tr("Pause Detection") : tr("Detect")
		    );
	    }
	);

	d_ui->detectButton->setEnabled(false);

	connect(d_ui->actionQuit, &QAction::triggered, [this]() { close(); });

	auto header = d_ui->tableView->horizontalHeader();
	header->setSectionResizeMode(QHeaderView::ResizeToContents);
	header->setStretchLastSection(true);

	d_ui->tableView->setModel(d_detectionProcess->model());

	connect(
	    d_detectionProcess->model(),
	    &QStandardItemModel::itemChanged,
	    this,
	    &MainWindow::onDataModification
	);

	connect(
	    d_detectionProcess->model(),
	    &QStandardItemModel::rowsInserted,
	    this,
	    &MainWindow::onDataModification
	);

	loadSettings();
	on_actionUnloadMyrmidonFile_triggered();

	connect(this, &MainWindow::needSaveChanged, this, [this](bool needSave) {
		setWindowTitle(
		    needSave ? tr("*FORT Tag Scanner") : tr("FORT Tag Scanner")
		);
	});
	setWindowTitle(tr("FORT Tag Scanner"));
}

MainWindow::~MainWindow() {
    delete d_ui;
}

void MainWindow::setCamera(QCamera *camera) {
	if (d_camera != nullptr) {
		delete d_camera;
		d_camera = nullptr;
	}

	d_camera = camera;
	d_session->setCamera(d_camera);

	if (d_camera == nullptr) {
		d_ui->detectButton->setEnabled(false);
		return;
	}

	d_ui->detectButton->setEnabled(true);
	d_camera->setActive(true);
	d_camera->start();
	d_detectionProcess->setDetectionActive(true);
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

	d_detectionProcess->setDetectionActive(!d_detectionProcess->isDetectionActive());
}


void MainWindow::on_actionSaveDataAsCSV_triggered() {
	auto filename = QFileDialog::getSaveFileName(this,tr("Save scan data as CSV"),
	                                             "",
	                                             tr("CSV (*.csv)"));

	if ( filename.isEmpty() ) {
		return;
	}
	QFileInfo fileInfo(filename);
	if ( fileInfo.suffix().isEmpty() ) {
		filename += ".csv";
	}

	const auto model = d_detectionProcess->model();

	std::ofstream file(filename.toUtf8().constData());
	file << "#ScanTime,TagID,AntID,Comment" << std::endl;
	for ( size_t i = 0; i < model->rowCount(); ++i) {
		file << "\"" \
		     << model->item(i,0)->text().toUtf8().constData()
		     << "\",\""
		     << model->item(i,1)->text().toUtf8().constData()
		     << "\",\""
		     << model->item(i,2)->text().toUtf8().constData()
		     << "\",\""
		     << model->item(i,3)->text().toUtf8().constData()
		     << "\""
		     << std::endl;
	}

	setNeedSave(false);
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
	if ( d_detectionProcess->hasTrackingSolver() == true ) {
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
		auto experiment = Experiment::OpenDataLess(filename.toUtf8().constData());
		auto trackingSolver = experiment->CompileTrackingSolver(true);
		d_detectionProcess->setTrackingSolver(std::move(trackingSolver));

		d_ui->myrmidonFileLabel->setText(tr("File loaded: %1").arg(experiment->AbsoluteFilePath().c_str()));
		d_ui->actionUnloadMyrmidonFile->setEnabled(true);
		d_ui->myrmidonButton->setText(tr("Unload Myrmidon File"));
	} catch ( const std::exception & e) {
		QMessageBox::warning(this,tr("Could not open file"),tr("Could not open file %1: %2").arg(filename).arg(e.what()));
		return;
	}
}

void MainWindow::on_actionUnloadMyrmidonFile_triggered() {
	d_detectionProcess->setTrackingSolver(nullptr);
	d_ui->myrmidonFileLabel->setText(tr("File loaded: None"));
	d_ui->actionUnloadMyrmidonFile->setEnabled(false);
	d_ui->myrmidonButton->setText(tr("Load Myrmidon File"));
}

void MainWindow::on_myrmidonButton_clicked() {
	if ( d_detectionProcess->hasTrackingSolver() == false ) {
		on_actionLoadMyrmidonFile_triggered();
	} else {
		on_actionUnloadMyrmidonFile_triggered();
	}
}

void MainWindow::onDataModification() {
	setNeedSave(true);
}


void MainWindow::setNeedSave(bool needSave) {
	if ( d_needSave == needSave) {
		return;
	}
	d_needSave = needSave;
	emit needSaveChanged(needSave);
}
