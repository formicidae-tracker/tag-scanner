#include <QGraphicsView>


struct DetectionDisplay {
	quint32 TagID,AntID,Count;

	QList<QPointF> Corners;
};


class DetectionView : public QGraphicsView {
	Q_OBJECT
public:
	explicit DetectionView(QWidget * parent = nullptr);
	virtual ~DetectionView();

	void displayImage(const QImage & image);

	void displayNoDetection();

	void displayDetection(const DetectionDisplay & detection);

	void prepare(const QSize & size);

private:
	void setText(const QString & text);

	QGraphicsScene * d_scene;

	QGraphicsPixmapItem     * d_image;
	QGraphicsSimpleTextItem * d_displayText;
	QGraphicsRectItem       * d_textBackground;
	QGraphicsPathItem       * d_tagOutline;
};
