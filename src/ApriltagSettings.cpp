#include "ApriltagSettings.hpp"
#include "ui_ApriltagSettings.h"

#include "ApriltagDetector.hpp"

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
