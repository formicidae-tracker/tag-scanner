#include <QVideoSink>
#include <QWidget>

#include <memory>
#include <qvideoframe.h>

namespace fort {
namespace myrmidon {
class TrackingSolver;
}
} // namespace fort

class DetectionView;
class ApriltagDetector;
class ApriltagSettings;
class QStandardItemModel;

class Detection;
typedef std::shared_ptr<Detection> DetectionPtr;

class DetectionProcess : public QObject {
	Q_OBJECT
	Q_PROPERTY(bool detectionActive READ isDetectionActive NOTIFY
	               detectionActiveChanged);

public:
	typedef std::unique_ptr<fort::myrmidon::TrackingSolver> TrackingSolverPtr;

	explicit DetectionProcess(QObject *parent = nullptr);
	virtual ~DetectionProcess();

	static std::string FormatAntID(quint32 antID);

	void setApriltagSettings(ApriltagSettings *settings);
	void setView(DetectionView *view);
	void setVideoSink(QVideoSink *sink);

	ApriltagDetector *detector() const;

	bool isDetectionActive() const;

	QStandardItemModel *model() const;

	bool hasTrackingSolver() const;

	void setTrackingSolver(TrackingSolverPtr solver);

public slots:
	void setDetectionActive(bool value);

	void pushNewFrame(const QVideoFrame &frame);

	void clearData();

signals:

	void detectionActiveChanged(bool active);

private:
	quint32 identifyAnt(quint32 tagID);
	quint32 countTag(quint32 tagID);

	void saveTag(quint32 tagID);

	ApriltagDetector *d_detector;
	ApriltagSettings *d_settings;
	DetectionView    *d_view;

	QStandardItemModel *d_model;

	TrackingSolverPtr d_solver;

	bool         d_detectionActive;
	DetectionPtr d_lastDetection;
	size_t       d_nbDetections;
};
