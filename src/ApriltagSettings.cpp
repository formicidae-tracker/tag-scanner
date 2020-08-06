#include "ApriltagSettings.hpp"
#include "ui_ApriltagSettings.h"

#include "ApriltagDetector.hpp"

#include <QSettings>

#include <fort/tags/fort-tags.h>

ApriltagSettings::ApriltagSettings(QWidget *parent)
	: QWidget(parent)
	, d_ui(new Ui::ApriltagSettings) {
    d_ui->setupUi(this);

    d_ui->family->addItem("16h5",quint32(fort::tags::Family::Tag16h5));
    d_ui->family->addItem("25h9",quint32(fort::tags::Family::Tag25h9));
    d_ui->family->addItem("36ARTag",quint32(fort::tags::Family::Tag36ARTag));
    d_ui->family->addItem("36h10",quint32(fort::tags::Family::Tag36h10));
    d_ui->family->addItem("36h11",quint32(fort::tags::Family::Tag36h11));
    d_ui->family->addItem("Standard41h12",quint32(fort::tags::Family::Standard41h12));
    d_ui->family->addItem("Standard52h13",quint32(fort::tags::Family::Standard52h13));
    d_ui->family->addItem("Circle21h7",quint32(fort::tags::Family::Circle21h7));
    d_ui->family->addItem("Circle49h12",quint32(fort::tags::Family::Circle49h12));
    d_ui->family->addItem("Custom48h12",quint32(fort::tags::Family::Custom48h12));

    d_ui->family->setCurrentIndex(-1);


    connect(d_ui->nbDetections,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,[this](int value) {
	            emit numberDetectionsChanged(value);
            });

}

ApriltagSettings::~ApriltagSettings(){
    delete d_ui;
}


void ApriltagSettings::setup(ApriltagDetector *d) {
	d_ui->threshold->setValue(d->threshold());
	d_ui->minCluster->setValue(d->clusterMinSize());

	connect(d_ui->family,static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            d,[this,d](int index) {
	            if ( index < 0 ) {
		            d->setFamily(fort::tags::Family::Undefined);
	            }
	            d->setFamily(fort::tags::Family(d_ui->family->currentData().toInt()));
              },
	        Qt::QueuedConnection);

	connect(d_ui->threshold,static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
	        d,[this,d](int value) {
		          d->setThreshold(value);
	          },
	        Qt::QueuedConnection);

	connect(d_ui->minCluster,static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
	        d,[this,d](int value) {
		          d->setClusterMinSize(value);
	          },
	        Qt::QueuedConnection);


}


int ApriltagSettings::numberDetections() const {
	return d_ui->nbDetections->value();
}


void ApriltagSettings::setNumberDetections(int nbDetections) {
	if ( nbDetections == d_ui->nbDetections->value() ) {
		return;
	}
	d_ui->nbDetections->setValue(nbDetections);
}

void ApriltagSettings::saveSettings() {
	QSettings settings;
	if ( d_ui->family->currentIndex() == -1 ) {
		settings.remove("at/family");
	} else {
		settings.setValue("at/family",d_ui->family->currentData());
	}
	settings.setValue("at/threshold",d_ui->threshold->value());
	settings.setValue("at/cluster-size",d_ui->minCluster->value());
	settings.setValue("at/nb-detections",d_ui->nbDetections->value());
}

void ApriltagSettings::loadSettings() {
	QSettings settings;
	d_ui->family->setCurrentIndex(d_ui->family->findData(settings.value("at/family",int(fort::tags::Family::Undefined))));
	d_ui->threshold->setValue(settings.value("at/threshold",40).toInt());
	d_ui->minCluster->setValue(settings.value("at/cluster-size",25).toInt());
	d_ui->nbDetections->setValue(settings.value("at/nb-detections",6).toInt());
}
