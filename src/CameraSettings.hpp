#pragma once

#include <QWidget>

class Camera;

namespace Ui {
class CameraSettings;
}

class CameraSettings : public QWidget {
    Q_OBJECT
	Q_PROPERTY(bool autofocus
	           READ autofocusEnabled
	           WRITE setAutofocus
	           NOTIFY autofocusChanged);
	Q_PROPERTY(qreal gain
	           READ gain
	           WRITE setGain
	           NOTIFY gainChanged);
	Q_PROPERTY(qreal focus
	           READ focus
	           WRITE setFocus
	           NOTIFY focusChanged);
	Q_PROPERTY(qreal exposure
	           READ exposure
	           WRITE setExposure
	           NOTIFY exposureChanged);

public:
    explicit CameraSettings(QWidget *parent = nullptr);
    ~CameraSettings();

	bool autofocusEnabled() const;
	qreal gain() const;
	qreal exposure() const;
	qreal focus() const;

	void setCamera(Camera * camera);

public slots:
	void setExposure(qreal exposure);
	void setGain(qreal gain);
	void setAutofocus(bool autofocus);
	void setFocus(qreal focus);

signals:
	void autofocusChanged(bool autofocus);
	void gainChanged(qreal gain);
	void exposureChanged(qreal exposure);
	void focusChanged(qreal focus);

private:
    Ui::CameraSettings * d_ui;
	Camera             * d_camera;
};
