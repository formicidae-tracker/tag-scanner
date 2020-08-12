#include "CameraSettings.hpp"
#include "ui_CameraSettings.h"

CameraSettings::CameraSettings(QWidget *parent)
	: QWidget(parent)
	, d_ui(new Ui::CameraSettings)
	, d_camera(nullptr) {
    d_ui->setupUi(this);
}

CameraSettings::~CameraSettings() {
    delete d_ui;
}

void CameraSettings::setCamera(QCamera * camera) {

}
