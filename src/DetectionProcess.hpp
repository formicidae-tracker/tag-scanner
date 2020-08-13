#include <QWidget>
#include <QAbstractVideoSurface>

#include <memory>

namespace fort {
namespace myrmidon {
class TrackingSolver;
}
}

class DetectionView;
class ApriltagDetector;
class ApriltagSettings;
class QStandardItemModel;

class Detection;
typedef std::shared_ptr<Detection> DetectionPtr;


class DetectionProcess : public QAbstractVideoSurface {
	Q_OBJECT
	Q_PROPERTY(bool detectionActive
	           READ isDetectionActive
	           NOTIFY detectionActiveChanged);

public:
	typedef std::shared_ptr<fort::myrmidon::TrackingSolver> TrackingSolverPtr;


	explicit DetectionProcess(QObject * parent = nullptr);
	virtual ~DetectionProcess();

	static std::string FormatAntID(quint32 antID);

	void setApriltagSettings(ApriltagSettings * settings);
	void setView(DetectionView * view);

	bool present(const QVideoFrame & frame) override;

	QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType type) const override;

	ApriltagDetector * detector() const;

	bool isDetectionActive() const;

	bool start(const QVideoSurfaceFormat & format) override;

	void stop() override;

	QStandardItemModel * model() const;

	bool hasTrackingSolver() const;

public slots:
	void setTrackingSolver(const TrackingSolverPtr & solver);

	void setDetectionActive(bool value);

	void clearData();

signals:

	void detectionActiveChanged(bool active);

private:
	quint32 identifyAnt(quint32 tagID);
	quint32 countTag(quint32 tagID);

	void saveTag(quint32 tagID);

	ApriltagDetector * d_detector;
	ApriltagSettings * d_settings;
	DetectionView    * d_view;

	QStandardItemModel * d_model;

	TrackingSolverPtr  d_solver;

	bool               d_detectionActive;
	DetectionPtr       d_lastDetection;
	size_t             d_nbDetections;
};
