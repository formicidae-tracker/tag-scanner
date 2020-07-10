#include "CameraSettings.hpp"
#include "ui_CameraSettings.h"

CameraSettings::CameraSettings(QWidget *parent)
	: QWidget(parent)
	, d_ui(new Ui::CameraSettings) {
    d_ui->setupUi(this);
}

CameraSettings::~CameraSettings() {
    delete d_ui;
}



bool CameraSettings::autofocusEnabled() const {
	return false;
}

qreal CameraSettings::gain() const {
	return 1.0;
}

qreal CameraSettings::exposure() const {
	return 1.0;
}

qreal CameraSettings::focus() const {
	return 1.0;
}

void CameraSettings::setExposure(qreal exposure) {

}

void CameraSettings::setGain(qreal gain) {

}

void CameraSettings::setAutofocus(bool autofocus) {

}

void CameraSettings::setFocus(qreal focus) {

}
