#pragma once

#include <memory>

class Detection;

#include <QMainWindow>

class Camera;
class ApriltagDetector;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

	void setCamera(Camera * camera);

	void on_actionLoadImage_triggered();
	void on_actionSaveDataAsCSV_triggered();

	void togglePlayPause();

signals:

	void newTag(quint32 );

private:
    Ui::MainWindow * d_ui;

	Camera * d_camera;
	ApriltagDetector * d_detector;

	std::shared_ptr<Detection> d_lastDetection;
	size_t                     d_lastDetectionCount;
	bool                       d_playing;
};
