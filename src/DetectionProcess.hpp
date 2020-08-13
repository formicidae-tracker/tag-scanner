#include <QWidget>
#include <QAbstractVideoSurface>

#include <memory>

class QGraphicsScene;
class QGraphicsView;
class QGraphicsPixmapItem;
class QGraphicsSimpleTextItem;
class QGraphicsRectItem;
class QGraphicsPathItem;
class ApriltagDetector;
class ApriltagSettings;

class Detection;
typedef std::shared_ptr<Detection> DetectionPtr;

class DetectionProcess : public QAbstractVideoSurface {
	Q_OBJECT
	Q_PROPERTY(bool detectionActive
	           READ isDetectionActive
	           NOTIFY detectionActiveChanged);

public:
	explicit DetectionProcess(QObject * parent = nullptr);
	virtual ~DetectionProcess();

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
	void setText(const QString & text);

	QGraphicsScene   * d_scene;
	QGraphicsView    * d_view;
	ApriltagDetector * d_detector;
	ApriltagSettings * d_settings;

	QGraphicsPixmapItem     * d_image;
	QGraphicsSimpleTextItem * d_displayText;
	QGraphicsRectItem       * d_textBackground;
	QGraphicsPathItem       * d_tagOutline;

	bool               d_detectionActive;
	DetectionPtr       d_lastDetection;
	size_t             d_nbDetections;
};
