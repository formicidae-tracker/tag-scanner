#pragma once

#include <memory>

class Detection;

#include <QMainWindow>

#include <fort/myrmidon/Experiment.hpp>

class QCamera;
class ApriltagDetector;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
	void newTag(quint32 );

protected:
    void closeEvent(QCloseEvent *event) override;


private slots:
	void setCamera(QCamera * camera);

	void on_actionLoadImage_triggered();
	void on_actionSaveDataAsCSV_triggered();

	void on_actionLoadMyrmidonFile_triggered();
	void on_actionUnloadMyrmidonFile_triggered();
	void on_myrmidonButton_clicked();

	void togglePlayPause();

private:
	typedef std::shared_ptr<fort::myrmidon::TrackingSolver> TrackingSolverPtr;
	bool maybeSave();
	void saveSettings();
	void loadSettings();

    Ui::MainWindow * d_ui;

	QCamera * d_camera;
	ApriltagDetector * d_detector;

	std::shared_ptr<Detection> d_lastDetection;
	size_t                     d_lastDetectionCount;
	bool                       d_playing;
	bool                       d_needSave;


	TrackingSolverPtr d_trackingSolver;
};
