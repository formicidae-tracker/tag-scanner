#include "ApriltagSettings.hpp"
#include "ui_ApriltagSettings.h"

ApriltagSettings::ApriltagSettings(QWidget *parent)
	: QWidget(parent)
	, d_ui(new Ui::ApriltagSettings) {
    d_ui->setupUi(this);
}

ApriltagSettings::~ApriltagSettings(){
    delete d_ui;
}
