#include "Camera.hpp"

#include <opencv2/videoio.hpp>

Camera::Camera(QObject * parent)
	: QObject(parent) {
}

Camera::~Camera() {
}

void Camera::emitAllSignals() {
	emit autofocusChanged(autofocusEnabled());
	emit gainChanged(gain());
	emit focusChanged(focus());
	emit exposureChanged(exposure());
}

StubCamera::StubCamera(const std::string & filename,
                       QObject * parent)
	: Camera(parent) {
}

StubCamera::~StubCamera() {
}

bool StubCamera::autofocusEnabled() const {
	return true;
}

qreal StubCamera::gain() const {
	return 1.0;
}

qreal StubCamera::exposure() const {
	return 1.0;
}

qreal StubCamera::focus() const {
	return 1.0;
}

void StubCamera::setAutofocus(bool autofocus) {

}

void StubCamera::setGain(qreal gain) {

}

void StubCamera::setExposure(qreal exposure) {

}

void StubCamera::setFocus(qreal focus) {

}

void StubCamera::start() {

}

void StubCamera::stop() {

}


CVCamera::CVCamera(int interface,
                   QObject * parent)
	: Camera(parent) {
}

CVCamera::~CVCamera() {
}


bool CVCamera::autofocusEnabled() const {
	return true;
}

qreal CVCamera::gain() const {
	return 1.0;
}

qreal CVCamera::exposure() const {
	return 1.0;
}

qreal CVCamera::focus() const {
	return 1.0;
}

void CVCamera::setAutofocus(bool autofocus) {
}

void CVCamera::setGain(qreal gain) {

}

void CVCamera::setExposure(qreal exposure) {

}

void CVCamera::setFocus(qreal focus) {

}

void CVCamera::start() {

}

void CVCamera::stop() {

}

std::map<int,std::string> CVCamera::Enumerate() {
	std::map<int,std::string> res;

	int device = 0;
	while(true) {
		cv::VideoCapture camera(++device);
		if ( camera.isOpened() == false) {
			break;
		}
		res[device] = camera.getBackendName();
	}
	return res;
}
