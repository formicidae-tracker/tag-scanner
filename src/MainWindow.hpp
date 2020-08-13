#pragma once

#include <memory>

class Detection;

#include <QMainWindow>

#include <fort/myrmidon/Experiment.hpp>

class QCamera;
class DetectionProcess;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;


signals:
	void needSaveChanged(bool needSave);

private slots:
	void setCamera(QCamera * camera);

	void on_actionLoadImage_triggered();
	void on_actionSaveDataAsCSV_triggered();

	void on_actionLoadMyrmidonFile_triggered();
	void on_actionUnloadMyrmidonFile_triggered();
	void on_myrmidonButton_clicked();

	void toggleDetection();

	void onDataModification();

private:
	bool maybeSave();
	void saveSettings();
	void loadSettings();

	void setNeedSave(bool needSave);

    Ui::MainWindow * d_ui;

	QCamera          * d_camera;
	DetectionProcess * d_detectionProcess;
	bool               d_needSave;
	bool               d_cameraLoaded;

};
