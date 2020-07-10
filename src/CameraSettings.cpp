#include "CameraSettings.hpp"
#include "ui_CameraSettings.h"

#include "Camera.hpp"

CameraSettings::CameraSettings(QWidget *parent)
	: QWidget(parent)
	, d_ui(new Ui::CameraSettings)
	, d_camera(nullptr) {
    d_ui->setupUi(this);


    connect(d_ui->gain,&QAbstractSlider::sliderMoved,
            this,
            [this](int value) {
	            emit gainChanged(gain());
            });

    connect(d_ui->exposure,&QAbstractSlider::sliderMoved,
            this,
            [this](int value) {
	            emit exposureChanged(exposure());
            });

    connect(d_ui->focus,&QAbstractSlider::sliderMoved,
            this,
            [this](int value) {
	            emit focusChanged(focus());
            });

    connect(d_ui->checkBox,&QCheckBox::stateChanged,
            this,
            [this](int state) {
	            emit autofocusChanged(state == Qt::Checked);
	            d_ui->focus->setEnabled(state == Qt::Unchecked);
            });
    setEnabled(false);
}

CameraSettings::~CameraSettings() {
    delete d_ui;
}



bool CameraSettings::autofocusEnabled() const {
	return d_ui->checkBox->checkState() == Qt::Checked;
}

qreal CameraSettings::gain() const {
	return qreal(d_ui->gain->value()) / 99.0;
}

qreal CameraSettings::exposure() const {
	return qreal(d_ui->exposure->value()) / 99.0;
}

qreal CameraSettings::focus() const {
	return qreal(d_ui->focus->value()) / 99.0;
}

void CameraSettings::setExposure(qreal exposure) {
	d_ui->exposure->setValue( 99 *  std::clamp(exposure,0.0,1.0));
}

void CameraSettings::setGain(qreal gain) {
	d_ui->gain->setValue( 99 *  std::clamp(gain,0.0,1.0));
}

void CameraSettings::setAutofocus(bool autofocus) {
	d_ui->checkBox->setCheckState(autofocus? Qt::Checked : Qt::Unchecked);
	d_ui->focus->setEnabled(autofocus == false);
}

void CameraSettings::setFocus(qreal focus) {
	d_ui->focus->setValue(99 * std::clamp(focus,0.0,1.0));
}


void CameraSettings::setCamera(Camera * camera) {
	if ( d_camera != nullptr ) {
		disconnect(d_camera,&Camera::autofocusChanged,
		           this,&CameraSettings::setAutofocus);
		disconnect(d_camera,&Camera::gainChanged,
		           this,&CameraSettings::setGain);
		disconnect(d_camera,&Camera::focusChanged,
		           this,&CameraSettings::setFocus);
		disconnect(d_camera,&Camera::exposureChanged,
		           this,&CameraSettings::setExposure);

		disconnect(this,&CameraSettings::autofocusChanged,
		           d_camera,&Camera::setAutofocus);
		disconnect(this,&CameraSettings::exposureChanged,
		           d_camera,&Camera::setExposure);
		disconnect(this,&CameraSettings::gainChanged,
		           d_camera,&Camera::setGain);
		disconnect(this,&CameraSettings::focusChanged,
		           d_camera,&Camera::setFocus);

	}

	d_camera = camera;
	if ( d_camera == nullptr )  {
		this->setEnabled(false);
		return;
	}
	this->setEnabled(true);

	connect(d_camera,&Camera::autofocusChanged,
	        this,&CameraSettings::setAutofocus);
	connect(d_camera,&Camera::gainChanged,
	        this,&CameraSettings::setGain);
	connect(d_camera,&Camera::focusChanged,
	        this,&CameraSettings::setFocus);
	connect(d_camera,&Camera::exposureChanged,
	        this,&CameraSettings::setExposure);


	connect(this,&CameraSettings::autofocusChanged,
		           d_camera,&Camera::setAutofocus);
	connect(this,&CameraSettings::exposureChanged,
		           d_camera,&Camera::setExposure);
	connect(this,&CameraSettings::gainChanged,
		           d_camera,&Camera::setGain);
	connect(this,&CameraSettings::focusChanged,
	        d_camera,&Camera::setFocus);

	camera->emitAllSignals();

}
