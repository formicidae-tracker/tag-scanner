#pragma once

#include <QWidget>

namespace Ui {
class ApriltagSettings;
}

class ApriltagSettings : public QWidget {
    Q_OBJECT
public:
    explicit ApriltagSettings(QWidget *parent = nullptr);
    virtual ~ApriltagSettings();

private:
    Ui::ApriltagSettings * d_ui;
};
