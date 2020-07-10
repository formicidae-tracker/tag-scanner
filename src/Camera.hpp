#pragma once

#include <QObject>

#include <opencv2/core.hpp>

Q_DECLARE_METATYPE(cv::Mat)

class Camera : public QObject{
	Q_OBJECT
	Q_PROPERTY(bool autofocus
	           WRITE setAutofocus
	           READ autofocusEnabled
	           NOTIFY autofocusChanged);
	Q_PROPERTY(qreal exposure
	           WRITE setExposure
	           READ exposure
	           NOTIFY exposureChanged);
	Q_PROPERTY(qreal gain
	           WRITE setGain
	           READ gain
	           NOTIFY gainChanged);
	Q_PROPERTY(qreal focus
	           WRITE setFocus
	           READ focus
	           NOTIFY focusChanged);
public:
	Camera(QObject * parent = nullptr);
	virtual ~Camera();

	bool autofocusEnabled() const;
	qreal gain() const;
	qreal exposure() const;
	qreal focus() const;


public slots:
	void setAutofocus(bool autofocus);
	void setGain(qreal gain);
	void setExposure(qreal exposure);
	void setFocus(qreal focus);

	void start();
	void stop();

signals:
	void autofocusChanged(bool autofocus);
	void gainChanged(qreal gain);
	void exposureChanged(qreal exposure);
	void focusChanged(qreal exposure);
	void playing(bool playing);

	void newFrame(cv::Mat);
};
