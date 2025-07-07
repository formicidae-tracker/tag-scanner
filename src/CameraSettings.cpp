#include "CameraSettings.hpp"
#include "ui_CameraSettings.h"

#include <QCamera>
#include <QSettings>
#include <qcameradevice.h>
#include <qnamespace.h>

CameraSettings::CameraSettings(QWidget *parent)
    : QWidget(parent)
    , d_ui(new Ui::CameraSettings)
    , d_camera(nullptr) {
	d_ui->setupUi(this);

	connect(
	    d_ui->formatBox,
	    static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
	    this,
	    &CameraSettings::updateCameraFormat
	);

	connect(
	    d_ui->focusBox,
	    static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
	    this,
	    &CameraSettings::updateFocusMode
	);

	connect(
	    d_ui->focusSlider,
	    &QAbstractSlider::valueChanged,
	    this,
	    &CameraSettings::updateFocus
	);

	connect(
	    d_ui->zoomSlider,
	    &QAbstractSlider::valueChanged,
	    this,
	    &CameraSettings::updateZoom
	);
}

CameraSettings::~CameraSettings() {
    delete d_ui;
}

bool operator<(const QSize &a, const QSize &b) {
	if (a.width() == b.width()) {
		return a.height() < b.height();
	}
	return a.width() < b.width();
}

QString FormatToString(const QCameraFormat &format) {
	return QString("%1 x %2 FPS:(%3-%4) %5")
	    .arg(format.resolution().width())
	    .arg(format.resolution().height())
	    .arg(format.minFrameRate())
	    .arg(format.maxFrameRate())
	    .arg(format.pixelFormat());
}

void CameraSettings::setCamera(QCamera *camera) {
	d_camera = nullptr;
	if (camera == nullptr) {
		//		setFocus(nullptr);
		setEnabled(false);
		return;
	}
	setEnabled(true);

	setFocus();
	const auto &formats = camera->cameraDevice().videoFormats();
	for (const auto &format : formats) {

		d_ui->formatBox->addItem(FormatToString(format));
	}

	d_ui->formatBox->setCurrentIndex(-1);

	d_camera = camera;

	if (formats.isEmpty() == false && loadSettings() == false) {
		auto preferred = formats.front();
		d_ui->formatBox->setCurrentIndex(0);
	}

	if (d_ui->focusBox->count() > 0) {
		d_ui->focusBox->setCurrentIndex(0);
	}
}

void CameraSettings::updateCameraFormat() {
	if (d_camera == nullptr) {
		return;
	}
	const auto formats =
	    QVector<QCameraFormat>(d_camera->cameraDevice().videoFormats());
	const auto idx = d_ui->formatBox->currentIndex();
	if (idx < 0 || idx >= formats.size()) {
		return;
	}

	d_camera->setCameraFormat(formats[idx]);

	writeSettings();
}

bool CameraSettings::loadSettings() {
	QSettings  settings;
	const auto format   = settings.value("camera/format");
	int resolutionIndex = d_ui->formatBox->findData(format, Qt::DisplayRole);
	if (resolutionIndex < 0) {
		return false;
	}
	auto camera = d_camera;
	d_camera    = nullptr;
	d_ui->formatBox->setCurrentIndex(resolutionIndex);

	return true;
}

void CameraSettings::writeSettings() {
	QSettings settings;
	settings.setValue("camera/format", d_ui->formatBox->currentData());
}

void CameraSettings::setupFocus() {
	d_ui->focusBox->clear();

	std::map<QString, QCamera::FocusMode> modes = {
	    {tr("Manual Focus"), QCamera::FocusModeManual},
	    {tr("Hyperfocal Focus"), QCamera::FocusModeHyperfocal},
	    {tr("Infinity Focus"), QCamera::FocusModeInfinity},
	    {tr("Continuous Focus"), QCamera::FocusModeAuto},
	    {tr("Continuous Focus - Near"), QCamera::FocusModeAutoNear},
	    {tr("Continuous Focus - Far"), QCamera::FocusModeAutoFar},
	};

	for (const auto &[name, mode] : modes) {
		if (d_camera->isFocusModeSupported(mode) == false) {
			continue;
		}
		d_ui->focusBox->addItem(name, int(mode));
	}

	d_ui->focusBox->setCurrentIndex(-1);
	d_ui->focusBox->setEnabled((d_ui->focusBox->count() > 0));

	const auto features = d_camera->supportedFeatures();

	d_ui->zoomSlider->setEnabled(zoomSupported());
}

void CameraSettings::updateFocusMode() {
	if (d_camera == nullptr || d_ui->focusBox->currentIndex() < 0) {
		return;
	}
	auto focusMode = QCamera::FocusMode(d_ui->focusBox->currentData().toInt());
	d_camera->setFocusMode(focusMode);
	d_ui->focusSlider->setEnabled(focusAvailable());
}

void CameraSettings::updateZoom() {
	if (d_camera == nullptr || zoomSupported() == false) {
		return;
	}
	auto wanted =
	    (d_camera->maximumZoomFactor() - d_camera->minimumZoomFactor()) /
	        100.0 * qreal(d_ui->zoomSlider->value()) +
	    d_camera->minimumZoomFactor();

	d_camera->setZoomFactor(wanted);
}

void CameraSettings::updateFocus() {
	if (d_camera == nullptr || focusAvailable() == false) {
		return;
	}
	auto wanted = qreal(d_ui->focusSlider->value()) / 100.0;
	d_camera->setFocusDistance(wanted);
}

bool CameraSettings::zoomSupported() const {
	return d_camera->maximumZoomFactor() != 1.0;
}

bool CameraSettings::focusAvailable() const {
	return d_camera->focusMode() == QCamera::FocusModeManual;
}
