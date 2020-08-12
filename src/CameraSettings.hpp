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

private:
    Ui::CameraSettings * d_ui;
	QCamera             * d_camera;
};
