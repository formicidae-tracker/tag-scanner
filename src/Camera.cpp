#include "Camera.hpp"

#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>

#include <QTimer>

Camera::Camera(QObject * parent)
	: QObject(parent){

}

Camera::~Camera() {
}

void Camera::emitAllSignals() {
	emit autofocusChanged(autofocusEnabled());
	emit gainChanged(gain());
	emit focusChanged(focus());
	emit exposureChanged(exposure());
	emit playing(false);
}

StubCamera::StubCamera(const std::string & filename,
                       QObject * parent)
	: Camera(parent)
	, d_timer(new QTimer(this)) {
	d_mat = cv::imread(filename);
	connect(d_timer,&QTimer::timeout,
	        this,[this]() {
		             emit newFrame(d_mat.clone());
	             });
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
	if ( gain != 1.0 ) {
		emit gainChanged(1.0);
	}
}

void StubCamera::setExposure(qreal exposure) {
	if ( exposure != 1.0 ) {
		emit exposureChanged(1.0);
	}
}

void StubCamera::setFocus(qreal focus) {
	if ( focus != 1.0 ) {
		emit focusChanged(1.0);
	}
}

void StubCamera::start() {
	d_timer->start(100);
	emit playing(true);
}

void StubCamera::stop() {
	d_timer->stop();
	emit playing(false);
}


CVCamera::CVCamera(int interface,
                   QObject * parent)
	: Camera(parent)
	, d_capture(interface)
	, d_timer(new QTimer(this)) {
	if ( d_capture.isOpened() == false ) {
		throw std::runtime_error("Could not open interface");
	}

	d_capture.set(cv::CAP_PROP_FPS,10);

	connect(d_timer,&QTimer::timeout,
	        this,[this]() {
		             cv::Mat mat;
		             d_capture >> mat;
		             if ( mat.empty() ) {
			             return;
		             }
		             emit newFrame(mat);
	             });
}

CVCamera::~CVCamera() {
}

bool CVCamera::autofocusEnabled() const {
	return d_capture.get(cv::CAP_PROP_AUTOFOCUS);
}

qreal CVCamera::gain() const {
	return d_capture.get(cv::CAP_PROP_GAIN);;
}

qreal CVCamera::exposure() const {
	return d_capture.get(cv::CAP_PROP_EXPOSURE);
}

qreal CVCamera::focus() const {
	return d_capture.get(cv::CAP_PROP_FOCUS);;
}

void CVCamera::setAutofocus(bool autofocus) {
	d_capture.set(cv::CAP_PROP_AUTOFOCUS,autofocus ? 1.0 : 0.0);
}

void CVCamera::setGain(qreal gain) {
	d_capture.set(cv::CAP_PROP_GAIN,gain);
}

void CVCamera::setExposure(qreal exposure) {
	d_capture.set(cv::CAP_PROP_EXPOSURE,exposure);
}

void CVCamera::setFocus(qreal focus) {
	d_capture.set(cv::CAP_PROP_FOCUS,focus);
}

void CVCamera::start() {
	d_timer->start(0);
	emit playing(true);
}

void CVCamera::stop() {
	d_timer->stop();
	emit playing(false);
}

std::map<int,std::string> CVCamera::Enumerate() {
	std::map<int,std::string> res;

	int device = -1;
	while(true) {
		cv::VideoCapture camera(++device);
		if ( camera.isOpened() == false) {
			break;
		}
		res[device] = "OpenCV camera " + std::to_string(device);
	}
	return res;
}
