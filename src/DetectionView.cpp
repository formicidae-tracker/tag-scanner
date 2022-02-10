#include "DetectionView.hpp"
#include "DetectionProcess.hpp"

#include <QGraphicsPixmapItem>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsPathItem>

#include <fort/myrmidon/types/Typedefs.hpp>

DetectionView::DetectionView(QWidget * parent)
	: d_scene(new QGraphicsScene(this)) {

	setScene(d_scene);

	d_image = new QGraphicsPixmapItem();

	d_textBackground = new QGraphicsRectItem();
	d_textBackground->setVisible(false);
	d_textBackground->setPen(Qt::NoPen);

	d_displayText = new QGraphicsSimpleTextItem();
	d_displayText->setVisible(false);

	auto font = d_displayText->font();
	font.setPixelSize(60);
	d_displayText->setFont(font);

	d_tagOutline = new QGraphicsPathItem();
	d_tagOutline->setVisible(false);

	d_scene->addItem(d_image);
	d_scene->addItem(d_tagOutline);
	d_scene->addItem(d_textBackground);
	d_scene->addItem(d_displayText);

	setColor(0,0);

}


DetectionView::~DetectionView() {
}

void DetectionView::displayImage(const QImage & image) {
	auto pixmap = QPixmap::fromImage(image);
	d_image->setPixmap(pixmap);
	fitInView(d_image->boundingRect(),Qt::KeepAspectRatio);
}

void DetectionView::displayNoDetection() {
	setText("No Tag Detected");
	setColor(0,0);
	d_tagOutline->setVisible(false);
}

void DetectionView::displayDetection(const DetectionDisplay & detection) {
	setColor(detection.AntID,detection.Count);
	auto tagStr = fort::myrmidon::FormatTagID(detection.TagID);
	if ( detection.AntID == 0 ) {
		setText(tr("TagID: %1 Count: %2")
		        .arg(tagStr.c_str())
		        .arg(detection.Count));
	} else {
		setText(tr("TagID: %1 AntID: %2 Count: %3")
		        .arg(tagStr.c_str())
		        .arg(DetectionProcess::FormatAntID(detection.AntID).c_str())
		        .arg(detection.Count));
	}

	QPainterPath path;
	path.moveTo(detection.Corners.back());
	for ( const auto & c : detection.Corners ) {
		path.lineTo(c);
	}
	d_tagOutline->setPath(path);
	d_tagOutline->setVisible(true);

}

void DetectionView::prepare(const QSize & size) {
	QRectF sceneBox(0,0,size.width(),size.height());

	d_scene->setSceneRect(sceneBox);
	fitInView(sceneBox,Qt::KeepAspectRatio);
	d_textBackground->setVisible(true);
	QRectF displayRect(0,0,size.width(),80);
	d_textBackground->setRect(displayRect);
	setColor(0,0);
}

void DetectionView::setText(const QString & text) {
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


void DetectionView::setColor(quint32 antID,quint32 count) {
	QPen outlinePen;
	outlinePen.setWidth(5);

	if ( count > 1 ) {
		outlinePen.setColor(QColor(255,0,0));
		d_tagOutline->setPen(outlinePen);
		d_tagOutline->setBrush(QColor(255,0,0,60));
		d_textBackground->setBrush(QColor(255,0,0,127));
		return;
	}
	d_textBackground->setBrush(QColor(255,255,255,127));

	if ( antID != 0 ) {
		outlinePen.setColor(QColor(0,255,0));
		d_tagOutline->setPen(outlinePen);
		d_tagOutline->setBrush(QColor(0,255,0,60));
	} else {
		outlinePen.setColor(QColor(0,255,255));
		d_tagOutline->setPen(outlinePen);
		d_tagOutline->setBrush(QColor(0,255,255,60));
	}



}
