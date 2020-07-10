#include "Camera.hpp"


Camera::Camera(QObject * parent)
	: QObject(parent) {
}

Camera::~Camera() {
}

bool Camera::autofocusEnabled() const {
	return true;
}

qreal Camera::gain() const {
	return 1.0;
}

qreal Camera::exposure() const {
	return 1.0;
}

qreal Camera::focus() const {
	return 1.0;
}

void Camera::setAutofocus(bool autofocus) {

}

void Camera::setGain(qreal gain) {

}

void Camera::setExposure(qreal exposure) {

}

void Camera::setFocus(qreal focus) {

}

void Camera::start() {

}

void Camera::stop() {

}
