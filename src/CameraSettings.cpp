#include "CameraSettings.hpp"
#include "ui_CameraSettings.h"

#include <QCamera>
#include <QSettings>

CameraSettings::CameraSettings(QWidget *parent)
	: QWidget(parent)
	, d_ui(new Ui::CameraSettings)
	, d_camera(nullptr) {
    d_ui->setupUi(this);

    connect(d_ui->resolutionBox,static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,&CameraSettings::updateFPSBox);

    connect(d_ui->fpsBox,static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,&CameraSettings::updateViewfinderSettings);

    connect(d_ui->focusBox,static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,&CameraSettings::updateFocusMode);

    connect(d_ui->opticalSlider,&QAbstractSlider::valueChanged,
            this, &CameraSettings::updateOpticalZoom);

    connect(d_ui->digitalSlider,&QAbstractSlider::valueChanged,
            this, &CameraSettings::updateDigitalZoom);

}

CameraSettings::~CameraSettings() {
    delete d_ui;
}

bool operator<(const QSize & a, const QSize & b) {
	if ( a.width() == b.width() ) {
		return a.height() < b.height();
	}
	return a.width() < b.width();
}

void CameraSettings::setCamera(QCamera * camera) {
	d_camera = nullptr;
	if ( camera == nullptr ) {
		setFocus(nullptr);
		setEnabled(false);
		return;
	}
	setEnabled(true);

	setFocus(camera->focus());


	d_resolutionAndFPS.clear();
	auto settings = camera->supportedViewfinderSettings();
	for ( const auto & s : settings ) {
		d_resolutionAndFPS[s.resolution()].push_back(s.maximumFrameRate());
	}



	for ( auto & [resolution,FPSs] : d_resolutionAndFPS ) {
		// TODO sort max to min
		std::sort(FPSs.begin(),FPSs.end(),
		          [](const qreal & a, const qreal & b) { return a > b;});

		d_ui->resolutionBox->addItem(QString("%1 x %2 FPS:(%3-%4)")
		                             .arg(resolution.width())
		                             .arg(resolution.height())
		                             .arg(FPSs.back())
		                             .arg(FPSs.front()),
		                             resolution);
	}
	d_ui->resolutionBox->setCurrentIndex(-1);
	d_camera = camera;
	if (settings.isEmpty() == false && loadSettings() == false ) {
		auto preferred = settings.front();
		d_ui->resolutionBox->setCurrentIndex(d_ui->resolutionBox->findData(preferred.resolution()));
	}

	if ( d_ui->focusBox->count() > 0 ) {
		d_ui->focusBox->setCurrentIndex(0);
	}
}

void CameraSettings::updateFPSBox() {
	if ( d_ui->resolutionBox->currentIndex() == -1 ) {
		return;
	}
	// preempt camera update
	auto camera = d_camera;
	d_camera = nullptr;

	d_ui->fpsBox->clear();
	for ( const auto & f : d_resolutionAndFPS[d_ui->resolutionBox->currentData().toSize()] ) {
		d_ui->fpsBox->addItem(tr("%1 FPS").arg(f),f);
	}
	d_ui->fpsBox->setCurrentIndex(-1);
	d_camera = camera;
	d_ui->fpsBox->setCurrentIndex(0);
}


void CameraSettings::updateViewfinderSettings() {
	if ( d_camera == nullptr ) {
		return;
	}
	if ( d_ui->resolutionBox->currentIndex() < 0
	     || d_ui->fpsBox->currentIndex() < 0 ) {
		return;
	}

	QCameraViewfinderSettings wanted;
	wanted.setResolution(d_ui->resolutionBox->currentData().toSize());
	wanted.setMaximumFrameRate(d_ui->fpsBox->currentData().toDouble());
	auto settings = d_camera->supportedViewfinderSettings(wanted);

	if ( settings.isEmpty() ) {
		return;
	}

	d_camera->setViewfinderSettings(settings[0]);
	writeSettings();
}

bool CameraSettings::loadSettings() {
	QSettings settings;
	QSize resolution = settings.value("camera/resolution").toSize();
	qreal fps = settings.value("camera/fps").toDouble();
	int resolutionIndex = d_ui->resolutionBox->findData(resolution);
	if ( resolutionIndex < 0 ) {
		return false;
	}
	auto camera  = d_camera;
	d_camera = nullptr;
	d_ui->resolutionBox->setCurrentIndex(resolutionIndex);
	int fpsIndex = d_ui->fpsBox->findData(fps);
	d_camera = camera;
	if ( fpsIndex < 0 ) {
		return false;
	}
	d_ui->fpsBox->setCurrentIndex(-1);
	d_ui->fpsBox->setCurrentIndex(fpsIndex);
	return true;
}

void CameraSettings::writeSettings() {
	QSettings settings;
	settings.setValue("camera/resolution",d_ui->resolutionBox->currentData().toSize());
	settings.setValue("camera/fps",d_ui->fpsBox->currentData().toDouble());
}

void CameraSettings::setFocus(QCameraFocus * focus) {
	d_focus = focus;
	d_ui->focusBox->clear();
	if ( focus == nullptr ) {
		d_ui->focusBox->setEnabled(false);
		d_ui->digitalSlider->setEnabled(false);
		d_ui->opticalSlider->setEnabled(false);
		return;
	}

	std::map<QString,QCameraFocus::FocusModes> modes
		= {
		   {tr("Manual Focus"),QCameraFocus::ManualFocus},
		   {tr("Hyperfocal Focus"),QCameraFocus::HyperfocalFocus},
		   {tr("Infinity Focus"),QCameraFocus::InfinityFocus},
		   {tr("Continuous Focus"),QCameraFocus::ContinuousFocus},
		   {tr("Macro Focus"),QCameraFocus::MacroFocus},
	};

	for ( const auto & [name,mode] : modes ) {
		if ( d_focus->isFocusModeSupported(mode) == false ) {
			continue;
		}
		d_ui->focusBox->addItem(name,int(mode));
	}
	d_ui->focusBox->setCurrentIndex(-1);
	d_ui->focusBox->setEnabled(d_ui->focusBox->count() > 0 );

	d_ui->opticalSlider->setEnabled(opticalZoomSupported());
	d_ui->digitalSlider->setEnabled(digitalZoomSupported());

}

void CameraSettings::updateFocusMode() {
	if ( d_camera == nullptr
	     || d_focus == nullptr
	     || d_ui->focusBox->currentIndex() < 0 ) {
		return;
	}

	d_focus->setFocusMode(QCameraFocus::FocusModes(d_ui->focusBox->currentData().toInt()));
}


void CameraSettings::updateOpticalZoom() {
	if ( d_camera == nullptr
	     || opticalZoomSupported() == false ) {
		return;
	}
	d_focus->zoomTo(opticalZoomValue(),
	                digitalZoomValue());
}

void CameraSettings::updateDigitalZoom() {
	if ( d_camera == nullptr
	     || digitalZoomSupported() == false ) {
		return;
	}
	d_focus->zoomTo(opticalZoomValue(),
	                digitalZoomValue());
}

qreal CameraSettings::opticalZoomValue() const {
	if ( opticalZoomSupported() == false ) {
		return 1.0;
	}
	return (d_ui->opticalSlider->value())/ 200.0 * d_focus->maximumOpticalZoom();
}

qreal CameraSettings::digitalZoomValue() const {
	if ( digitalZoomSupported() == false ) {
		return 1.0;
	}
	return (d_ui->digitalSlider->value())/ 200.0 * d_focus->maximumDigitalZoom();
}


bool CameraSettings::opticalZoomSupported() const {
	return d_focus != nullptr && d_focus->maximumOpticalZoom() != 1.0;
}

bool CameraSettings::digitalZoomSupported() const {
	return d_focus != nullptr && d_focus->maximumDigitalZoom() != 1.0;
}
