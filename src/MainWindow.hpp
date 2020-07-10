#pragma once

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

private:
    Ui::MainWindow * d_ui;

	Camera * d_camera;
	ApriltagDetector * d_detector;
};
