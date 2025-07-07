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

	void setCamera(QCamera *camera);

private slots:
	void updateCameraFormat();
	void updateFocusMode();
	void updateZoom();
	void updateFocus();

private:
	bool zoomSupported() const;
	bool focusAvailable() const;

	// qreal zoomValue() const;
	// qreal focusValue() const;

	void setupFocus();

	bool loadSettings();
	void writeSettings();

	Ui::CameraSettings *d_ui;
	QCamera	        *d_camera;
	//	QCameraFocus	                   *d_focus;
};
