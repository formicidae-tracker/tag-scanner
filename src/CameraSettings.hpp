#pragma once

#include <QWidget>

class QCamera;
class QCameraFocus;

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
	void updateFocusMode();
	void updateOpticalZoom();
	void updateDigitalZoom();
private:
	bool opticalZoomSupported() const;
	bool digitalZoomSupported() const;

	qreal opticalZoomValue() const;
	qreal digitalZoomValue() const;

	void setFocus(QCameraFocus * focus);

	bool loadSettings();
	void writeSettings();

    Ui::CameraSettings * d_ui;
	QCamera             * d_camera;
	QCameraFocus        * d_focus;
	std::map<QSize,std::vector<qreal>> d_resolutionAndFPS;
};
