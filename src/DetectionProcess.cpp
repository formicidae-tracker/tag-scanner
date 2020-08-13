
#include "DetectionProcess.hpp"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVideoSurfaceFormat>
#include <QGraphicsPixmapItem>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsRectItem>
#include <QGraphicsPathItem>

#include "ApriltagDetector.hpp"
#include "ApriltagSettings.hpp"

DetectionProcess::DetectionProcess(QObject * parent)
	: QAbstractVideoSurface(parent)
	, d_scene(new QGraphicsScene(this))
	, d_view(nullptr)
	, d_detectionActive(false)
	, d_detector( new ApriltagDetector(this) )
	, d_settings(nullptr) {

	d_image = new QGraphicsPixmapItem();

	d_textBackground = new QGraphicsRectItem();
	d_textBackground->setVisible(false);
	d_textBackground->setBrush(QColor(255,255,255,127));
	d_textBackground->setPen(Qt::NoPen);

	d_displayText = new QGraphicsSimpleTextItem();
	d_displayText->setVisible(false);

	auto font = d_displayText->font();
	font.setPixelSize(60);
	d_displayText->setFont(font);

	d_tagOutline = new QGraphicsPathItem();
	d_tagOutline->setVisible(false);
	QPen outlinePen(QColor(0,255,255));
	outlinePen.setWidth(5);
	d_tagOutline->setPen(outlinePen);
	d_tagOutline->setBrush(QColor(0,255,255,60));

	d_scene->addItem(d_image);
	d_scene->addItem(d_tagOutline);
	d_scene->addItem(d_textBackground);
	d_scene->addItem(d_displayText);
}

DetectionProcess::~DetectionProcess() {
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

	auto pixmap = QPixmap::fromImage(image);
	d_image->setPixmap(pixmap);
	d_view->fitInView(d_image->boundingRect(),Qt::KeepAspectRatio);


	auto detection = d_detector->processImage(image);
	if ( !detection ) {
		setText(tr("No Tag Detected"));
		d_tagOutline->setVisible(false);
		frame.unmap();
		return true;
	}

	auto tagStr = fort::myrmidon::FormatTagID(detection->TagID);
	setText(tr("Detected: %1").arg(tagStr.c_str()));
	QPainterPath path;
	path.moveTo(ToQt(detection->Corners[3]));
	path.lineTo(ToQt(detection->Corners[0]));
	path.lineTo(ToQt(detection->Corners[1]));
	path.lineTo(ToQt(detection->Corners[2]));
	path.lineTo(ToQt(detection->Corners[3]));
	d_tagOutline->setPath(path);
	d_tagOutline->setVisible(true);

	if ( !d_lastDetection || d_lastDetection->TagID == detection->TagID ) {
		if ( ++d_nbDetections >= d_settings->numberDetections() ) {
			setDetectionActive(false);
			emit newTag(detection->TagID);
		}
	}

	//todo: display detection in scene

	return true;
}

void DetectionProcess::setView(QGraphicsView * view) {
	d_view = view;
	if ( d_view == nullptr ) {
		return;
	}
	d_view->setScene(d_scene);
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
	QRectF sceneBox(0,0,format.frameWidth(),format.frameHeight());

	d_scene->setSceneRect(sceneBox);
	d_view->fitInView(sceneBox,Qt::KeepAspectRatio);
	d_textBackground->setVisible(true);
	QRectF displayRect(0,0,format.frameWidth(),80);
	d_textBackground->setRect(displayRect);

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


void DetectionProcess::setText(const QString & text) {
	if ( text.isEmpty() ) {
		d_displayText->setVisible(false);
	}
	d_displayText->setText(text);
	auto textRect =  d_displayText->boundingRect();
	auto pos = d_textBackground->boundingRect().center()
		- QPointF(textRect.width(),textRect.height()) * 0.5;
	d_displayText->setPos(pos);
	d_displayText->setVisible(true);
}
