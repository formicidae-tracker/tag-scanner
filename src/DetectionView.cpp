#include "DetectionView.hpp"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVideoSurfaceFormat>
#include <QGraphicsPixmapItem>

#include "ApriltagDetector.hpp"
#include "ApriltagSettings.hpp"

DetectionView::DetectionView(QObject * parent)
	: QAbstractVideoSurface(parent)
	, d_scene(new QGraphicsScene(this))
	, d_view(nullptr)
	, d_detectionActive(false)
	, d_detector( new ApriltagDetector(this) )
	, d_settings(nullptr) {

	d_image = new QGraphicsPixmapItem();
	d_scene->addItem(d_image);
}

DetectionView::~DetectionView() {
}

bool DetectionView::isDetectionActive() const {
	return d_detectionActive;
}

void DetectionView::setApriltagSettings(ApriltagSettings * settings) {
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

bool DetectionView::present(const QVideoFrame & frameIn)  {
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

	auto pixmap = QPixmap::fromImage(image);
	d_image->setPixmap(pixmap);
	d_view->fitInView(d_image->boundingRect(),Qt::KeepAspectRatio);

	auto detection = d_detector->processImage(image);
	if ( !detection ) {
		frame.unmap();
		return true;
	}

	if ( !d_lastDetection || d_lastDetection->TagID == detection->TagID ) {
		if ( ++d_nbDetections >= d_settings->numberDetections() ) {
			setDetectionActive(false);
			emit newTag(detection->TagID);
		}
	}

	//todo: display detection in scene

	return true;
}

void DetectionView::setView(QGraphicsView * view) {
	d_view = view;
	if ( d_view == nullptr ) {
		return;
	}
	d_view->setScene(d_scene);
}


QList<QVideoFrame::PixelFormat> DetectionView::supportedPixelFormats(QAbstractVideoBuffer::HandleType type) const {
	return {QVideoFrame::Format_RGB24,QVideoFrame::Format_BGR24};
}


ApriltagDetector * DetectionView::detector() const {
	return d_detector;
}

bool DetectionView::start(const QVideoSurfaceFormat & format) {
	if ( d_view == nullptr ) {
		return false;
	}
	QRectF sceneBox(0,0,format.frameWidth(),format.frameHeight());

	d_scene->setSceneRect(sceneBox);
	d_view->fitInView(sceneBox,Qt::KeepAspectRatio);

	setDetectionActive(true);
	return QAbstractVideoSurface::start(format);
}

void DetectionView::stop() {
	QAbstractVideoSurface::stop();
	setDetectionActive(false);
}

void DetectionView::setDetectionActive(bool value) {
	if ( d_detectionActive == value ) {
		return;
	}
	d_lastDetection.reset();
	d_nbDetections = 0;

	d_detectionActive = value;
	emit detectionActiveChanged(d_detectionActive);
}
