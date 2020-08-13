
#include "DetectionProcess.hpp"

#include "DetectionView.hpp"

#include "ApriltagDetector.hpp"
#include "ApriltagSettings.hpp"

#include <QVideoSurfaceFormat>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QApplication>

#include <fort/myrmidon/Ant.hpp>
#include <fort/myrmidon/Experiment.hpp>

DetectionProcess::DetectionProcess(QObject * parent)
	: QAbstractVideoSurface(parent)
	, d_view(nullptr)
	, d_detectionActive(false)
	, d_detector( new ApriltagDetector(this) )
	, d_settings(nullptr)
	, d_model(new QStandardItemModel(this) ) {
	clearData();
}


void DetectionProcess::clearData() {
	d_model->clear();

	d_model->setHorizontalHeaderLabels({tr("Scan Time"),
	                                    tr("TagID"),
	                                    tr("AntID"),
	                                    tr("Comment")});
}



DetectionProcess::~DetectionProcess() {
}

QStandardItemModel * DetectionProcess::model() const {
	return d_model;
}

bool DetectionProcess::isDetectionActive() const {
	return d_detectionActive;
}

void DetectionProcess::setApriltagSettings(ApriltagSettings * settings) {
	settings->setup(d_detector);
	d_settings = settings;
}

std::ostream & operator<<(std::ostream & out,
                          const QRectF & rect) {
	return out << "QRectF{x:" << rect.x()
	           << ",y:" << rect.y()
	           << ",w:" << rect.width()
	           << ",h:" << rect.height()
	           << "}";
}

QPointF ToQt(const Eigen::Vector2d & p) {
	return QPointF(p.x(),p.y());
}


bool DetectionProcess::present(const QVideoFrame & frameIn)  {
	if ( d_detectionActive == false ) {
		//just drop the frame
		return true;
	}

	QVideoFrame frame(frameIn);
	frame.map(QAbstractVideoBuffer::ReadOnly);
	// now do the detection and the display, and stop detection if needed
	auto imageFormat = QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat());
	QImage image(frame.bits(),
	             frame.width(),
	             frame.height(),
	             frame.bytesPerLine(),
	             imageFormat);

	if ( d_view == nullptr ) {
		return true;
	}
	d_view->displayImage(image);

	auto detection = d_detector->processImage(image);
	if ( !detection ) {
		d_view->displayNoDetection();
		frame.unmap();
		d_lastDetection.reset();
		d_nbDetections = 0;
		return true;
	}


	if ( !d_lastDetection == false
	     && d_lastDetection->TagID == detection->TagID ) {
		if ( ++d_nbDetections >= d_settings->numberDetections() ) {
			setDetectionActive(false);
			QApplication::beep();
			saveTag(detection->TagID);
		}
	} else {
		d_lastDetection = detection;
		d_nbDetections = 1;
	}

	d_view->displayDetection(DetectionDisplay{
	                                          .TagID = detection->TagID,
	                                          .AntID = identifyAnt(detection->TagID),
	                                          .Count = countTag(detection->TagID),
	                                          .Corners = { ToQt(detection->Corners[0]),
	                                                       ToQt(detection->Corners[1]),
	                                                       ToQt(detection->Corners[2]),
	                                                       ToQt(detection->Corners[3])}});
	frame.unmap();
	return true;
}

void DetectionProcess::setView(DetectionView * view) {
	d_view = view;
}


QList<QVideoFrame::PixelFormat> DetectionProcess::supportedPixelFormats(QAbstractVideoBuffer::HandleType type) const {
	return {QVideoFrame::Format_RGB24,QVideoFrame::Format_BGR24};
}


ApriltagDetector * DetectionProcess::detector() const {
	return d_detector;
}

bool DetectionProcess::start(const QVideoSurfaceFormat & format) {
	if ( d_view == nullptr ) {
		return false;
	}
	if ( d_view ) {
		d_view->prepare(QSize(format.frameWidth(),format.frameHeight()));
	}

	setDetectionActive(true);
	return QAbstractVideoSurface::start(format);
}

void DetectionProcess::stop() {
	QAbstractVideoSurface::stop();
	setDetectionActive(false);
}

void DetectionProcess::setDetectionActive(bool value) {
	if ( d_detectionActive == value ) {
		return;
	}
	d_lastDetection.reset();
	d_nbDetections = 0;

	d_detectionActive = value;
	emit detectionActiveChanged(d_detectionActive);
}

std::string DetectionProcess::FormatAntID(quint32 antID) {
	std::ostringstream oss;
	oss << std::setw(3) << std::setfill('0') << antID;
	return oss.str();
}

void DetectionProcess::saveTag(quint32 tagID) {
	auto now = fort::myrmidon::Time::Now();
	std::ostringstream nowStr;

	nowStr << now.Round(fort::myrmidon::Duration::Second);
	auto tagStr = fort::myrmidon::FormatTagID(tagID);

	auto timeItem = new QStandardItem(nowStr.str().c_str());
	timeItem->setEditable(false);
	auto tagItem = new QStandardItem(tagStr.c_str());
	tagItem->setEditable(false);
	QStandardItem * antItem = new QStandardItem("");
	auto antID = identifyAnt(tagID);
	if ( antID != 0 ) {
		antItem->setText(FormatAntID(antID).c_str());
	}
	antItem->setEditable(false);
	auto commentItem = new QStandardItem("");
	d_model->insertRow(d_model->rowCount(),{timeItem,tagItem,antItem,commentItem});
}


quint32 DetectionProcess::identifyAnt(quint32 tagID) {
	if ( !d_solver ) {
		return 0;
	}
	return d_solver->IdentifyTag(tagID,fort::myrmidon::Time::Now());
}

quint32 DetectionProcess::countTag(quint32 tagID) {
	auto tagStr = fort::myrmidon::FormatTagID(tagID);
	return d_model->findItems(tagStr.c_str(),Qt::MatchExactly,1).size();
}

bool DetectionProcess::hasTrackingSolver() const {
	return !d_solver == false;
}

void DetectionProcess::setTrackingSolver(const TrackingSolverPtr & solver ) {
	d_solver = solver;
}
