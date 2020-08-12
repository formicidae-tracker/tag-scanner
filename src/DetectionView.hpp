#include <QWidget>
#include <QAbstractVideoSurface>

#include <memory>

class QGraphicsScene;
class QGraphicsView;
class QGraphicsPixmapItem;
class ApriltagDetector;
class ApriltagSettings;

class Detection;
typedef std::shared_ptr<Detection> DetectionPtr;

class DetectionView : public QAbstractVideoSurface {
	Q_OBJECT
	Q_PROPERTY(bool detectionActive
	           READ isDetectionActive
	           NOTIFY detectionActiveChanged);

public:
	explicit DetectionView(QObject * parent = nullptr);
	virtual ~DetectionView();

	void setApriltagSettings(ApriltagSettings * settings);
	void setView(QGraphicsView * view);

	bool present(const QVideoFrame & frame) override;

	QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType type) const override;


	ApriltagDetector * detector() const;

	bool isDetectionActive() const;

	bool start(const QVideoSurfaceFormat & format) override;

	void stop() override;

public slots:
	void setDetectionActive(bool value);


signals:
	void newTag(quint32 tagID);

	void detectionActiveChanged(bool active);

private:

	QGraphicsScene   * d_scene;
	QGraphicsView    * d_view;
	ApriltagDetector * d_detector;
	ApriltagSettings * d_settings;
	QGraphicsPixmapItem * d_image;
	bool               d_detectionActive;
	DetectionPtr       d_lastDetection;
	size_t             d_nbDetections;
};
