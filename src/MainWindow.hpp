#pragma once

#include <memory>

class Detection;

#include <QMainWindow>

#include <fort/myrmidon/Experiment.hpp>

class QCamera;
class DetectionView;

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


private slots:
	void setCamera(QCamera * camera);

	void on_actionLoadImage_triggered();
	void on_actionSaveDataAsCSV_triggered();

	void on_actionLoadMyrmidonFile_triggered();
	void on_actionUnloadMyrmidonFile_triggered();
	void on_myrmidonButton_clicked();

	void onNewTag(quint32 tag);

	void toggleDetection();



private:
	typedef std::shared_ptr<fort::myrmidon::TrackingSolver> TrackingSolverPtr;
	bool maybeSave();
	void saveSettings();
	void loadSettings();

    Ui::MainWindow * d_ui;

	QCamera       * d_camera;
	DetectionView * d_detectionView;
	bool            d_needSave;
	bool            d_cameraLoaded;

	TrackingSolverPtr d_trackingSolver;
};
