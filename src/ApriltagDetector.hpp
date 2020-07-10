#pragma once

#include <QObject>

#include <opencv2/core.hpp>

#include <fort/myrmidon/Types.hpp>

#include <apriltag/apriltag.h>

extern "C" {
#include <apriltag/tag16h5.h>
#include <apriltag/tag25h9.h>
#include <apriltag/tag36h11.h>
#include <apriltag/tagCircle21h7.h>
#include <apriltag/tagCircle49h12.h>
#include <apriltag/tagCustom48h12.h>
#include <apriltag/tagStandard41h12.h>
#include <apriltag/tagStandard52h13.h>
#include <fort/tags/tag36ARTag.h>
#include <fort/tags/tag36h10.h>
}

#include <fort/tags/fort-tags.h>

struct Detection {
	typedef std::shared_ptr<Detection> Ptr;
	fort::myrmidon::TagID TagID;
	fort::myrmidon::Vector2dList Corners;
};

Q_DECLARE_METATYPE(Detection::Ptr)
Q_DECLARE_METATYPE(fort::tags::Family)

class ApriltagDetector : public QObject {
	Q_OBJECT
	Q_PROPERTY(fort::tags::Family family
	           READ family
	           WRITE setFamily
	           NOTIFY familyChanged);
	Q_PROPERTY(quint8 threshold
	           READ threshold
	           WRITE setThreshold
	           NOTIFY thresholdChanged);
	Q_PROPERTY(quint32 clusterMinSize
	           READ clusterMinSize
	           WRITE setClusterMinSize
	           NOTIFY clusterMinSizeChanged);
public:
	explicit ApriltagDetector(QObject * parent = nullptr);
	virtual ~ApriltagDetector();

	fort::tags::Family family() const;
	quint8 threshold() const;
	quint32 clusterMinSize() const;


public slots:
	void setFamily(fort::tags::Family family);
	void setThreshold(quint8 threshold);
	void setClusterMinSize(quint32 minSize);

	void processFrame(cv::Mat mat);
private slots:
	void updateDetector();

signals:
	void frameProcessed(cv::Mat mat,Detection::Ptr);

	void thresholdChanged(quint8 );
	void clusterMinSizeChanged(quint8 );
	void familyChanged(fort::tags::Family);

private:

	typedef std::function<void (apriltag_family_t *)> FamilyDestructor;

	fort::tags::Family     d_family;
	quint32               d_minCluster;
	quint8                d_threshold;
	apriltag_family_t   * d_atFamily;
	apriltag_detector_t * d_detector;
	FamilyDestructor      d_familyDestructor;
};
