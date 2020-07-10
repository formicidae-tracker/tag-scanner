#pragma once

#include <QObject>

#include <opencv2/videoio.hpp>

class QTimer;

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

	virtual bool autofocusEnabled() const = 0;
	virtual qreal gain() const = 0;
	virtual qreal exposure() const = 0;
	virtual qreal focus() const = 0;

	void emitAllSignals();

public slots:
	virtual void setAutofocus(bool autofocus) = 0;
	virtual void setGain(qreal gain) = 0;
	virtual void setExposure(qreal exposure) = 0;
	virtual void setFocus(qreal focus) = 0;

	virtual void start() = 0;
	virtual void stop() = 0;

signals:
	void autofocusChanged(bool autofocus);
	void gainChanged(qreal gain);
	void exposureChanged(qreal exposure);
	void focusChanged(qreal exposure);
	void playing(bool playing);

	void newFrame(cv::Mat);
};


class StubCamera : public Camera {
	Q_OBJECT
public:
	StubCamera(const std::string & filename,
	           QObject * parent);
	virtual ~StubCamera();

	bool autofocusEnabled() const override;
	qreal gain() const override;
	qreal exposure() const override;
	qreal focus() const override;

public slots:
	void setAutofocus(bool autofocus)  override;
	void setGain(qreal gain)  override;
	void setExposure(qreal exposure)  override;
	void setFocus(qreal focus) override;

	void start() override;
	void stop() override;
private :
	cv::Mat d_mat;
	QTimer * d_timer;
};


class CVCamera : public Camera {
	Q_OBJECT
public:
	static std::map<int,std::string> Enumerate();

	CVCamera(int index,
	         QObject * parent);
	virtual ~CVCamera();

	bool autofocusEnabled() const override;
	qreal gain() const override;
	qreal exposure() const override;
	qreal focus() const override;


public slots:
	void setAutofocus(bool autofocus) override;
	void setGain(qreal gain) override;
	void setExposure(qreal exposure) override;
	void setFocus(qreal focus) override;

	void start() override;
	void stop() override;
};
