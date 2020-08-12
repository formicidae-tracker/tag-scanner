#pragma once

#include <QWidget>

class QCamera;

namespace Ui {
class CameraSettings;
}

class CameraSettings : public QWidget {
    Q_OBJECT
public:
    explicit CameraSettings(QWidget *parent = nullptr);
    ~CameraSettings();

	void setCamera(QCamera * camera);

private slots:
	void updateViewfinderSettings();
	void updateFPSBox();

private:
	bool loadSettings();
	void writeSettings();

    Ui::CameraSettings * d_ui;
	QCamera             * d_camera;

	std::map<QSize,std::vector<qreal>> d_resolutionAndFPS;
};
