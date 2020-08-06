#pragma once

#include <QWidget>

class ApriltagDetector;

namespace Ui {
class ApriltagSettings;
}

class ApriltagSettings : public QWidget {
	Q_PROPERTY(int numberDetections
	           READ numberDetections
	           WRITE setNumberDetections
	           NOTIFY numberDetectionsChanged)
    Q_OBJECT
public:
    explicit ApriltagSettings(QWidget *parent = nullptr);
    virtual ~ApriltagSettings();


	void setup(ApriltagDetector *d);

	int numberDetections() const;

public slots:
	void setNumberDetections(int nbDetections);
signals:
	void numberDetectionsChanged(int value);

private:
    Ui::ApriltagSettings * d_ui;
};
