#pragma once

#include <QWidget>

class ApriltagDetector;

namespace Ui {
class ApriltagSettings;
}

class ApriltagSettings : public QWidget {
    Q_OBJECT
public:
    explicit ApriltagSettings(QWidget *parent = nullptr);
    virtual ~ApriltagSettings();


	void setup(ApriltagDetector *d);

private:
    Ui::ApriltagSettings * d_ui;
};
