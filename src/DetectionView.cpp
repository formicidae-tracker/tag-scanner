#include "DetectionView.hpp"

#include <QGraphicsPixmapItem>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsPathItem>

#include <fort/myrmidon/Types.hpp>

DetectionView::DetectionView(QWidget * parent)
	: d_scene(new QGraphicsScene(this)) {

	setScene(d_scene);

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


DetectionView::~DetectionView() {
}

void DetectionView::displayImage(const QImage & image) {
	auto pixmap = QPixmap::fromImage(image);
	d_image->setPixmap(pixmap);
	fitInView(d_image->boundingRect(),Qt::KeepAspectRatio);
}

void DetectionView::displayNoDetection() {
	setText("No Tag Detected");
	d_tagOutline->setVisible(false);
}

void DetectionView::displayDetection(const DetectionDisplay & detection) {
	auto tagStr = fort::myrmidon::FormatTagID(detection.TagID);
	setText(tr("Detected: %1").arg(tagStr.c_str()));

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
