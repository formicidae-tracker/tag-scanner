#include "ApriltagDetector.hpp"


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

#include <QImage>

ApriltagDetector::ApriltagDetector(QObject * parent)
	: QObject(parent)
	, d_family(fort::tags::Family::Undefined)
	, d_threshold(40)
	, d_minCluster(25)
	, d_detector(apriltag_detector_create())
	, d_atFamily(nullptr) {
	connect(this,&ApriltagDetector::familyChanged,
	        this,&ApriltagDetector::updateDetector);

	connect(this,&ApriltagDetector::thresholdChanged,
	        this,&ApriltagDetector::updateDetector);

	connect(this,&ApriltagDetector::clusterMinSizeChanged,
	        this,&ApriltagDetector::updateDetector);

	updateDetector();

}

ApriltagDetector::~ApriltagDetector() {
	apriltag_detector_destroy(d_detector);
	if ( d_atFamily != nullptr) {
		d_familyDestructor(d_atFamily);
	}
}

fort::tags::Family ApriltagDetector::family() const {
	return d_family;
}

quint8 ApriltagDetector::threshold() const {
	return d_threshold;
}

quint32 ApriltagDetector::clusterMinSize() const {
	return d_minCluster;
}

void ApriltagDetector::setFamily(fort::tags::Family family) {
	if ( family  == d_family ) {
		return;
	}
	d_family = family;
	emit familyChanged(family);
}

void ApriltagDetector::setThreshold(quint8 threshold) {
	if ( threshold == d_threshold ) {
		return;
	}
	d_threshold = threshold;
	emit thresholdChanged(threshold);
}

void ApriltagDetector::setClusterMinSize(quint32 minSize) {
	if ( minSize = d_minCluster) {
		return;
	}
	d_minCluster = minSize;
	emit clusterMinSizeChanged(minSize);
}

Detection::Ptr ApriltagDetector::processImage(const QImage & frame) {
	if ( d_family == fort::tags::Family::Undefined ) {
		return nullptr;
	}
	auto gray = frame.convertToFormat(QImage::Format_Grayscale8);

	image_u8_t img = {
	                  .width = gray.width(),
	                  .height = gray.height(),
	                  .stride = gray.bytesPerLine(),
	                  .buf = gray.bits()
	};

	auto detections = apriltag_detector_detect(d_detector,
	                                           &img);

	if ( zarray_size(detections) == 0 ) {
		return nullptr;
	}
	apriltag_detection_t* d;
	zarray_get(detections,0,&d);
	auto res = std::make_shared<Detection>(Detection{
	                                                 .TagID = fort::myrmidon::TagID(d->id),
	                                                 .Corners = {
	                                                             Eigen::Vector2d(d->p[0][0],d->p[0][1]),
	                                                             Eigen::Vector2d(d->p[1][0],d->p[1][1]),
	                                                             Eigen::Vector2d(d->p[2][0],d->p[2][1]),
	                                                             Eigen::Vector2d(d->p[3][0],d->p[3][1]),
	                                                             },
		});
	apriltag_detections_destroy(detections);
	return res;
}


void ApriltagDetector::updateDetector() {
	d_detector->qtp.min_white_black_diff = d_threshold;
	d_detector->qtp.min_cluster_pixels = d_minCluster;

	if ( d_atFamily != nullptr ) {
		apriltag_detector_remove_family(d_detector,d_atFamily);
		d_familyDestructor(d_atFamily);
		d_atFamily = nullptr;
	}
	if ( d_family == fort::tags::Family::Undefined ) {
		return;
	}

	static std::map<fort::tags::Family,std::pair<std::function<apriltag_family_t *()>,
	                                      FamilyDestructor> > families
		= {
		   {fort::tags::Family::Tag16h5,{tag16h5_create,tag16h5_destroy}},
		   {fort::tags::Family::Tag25h9,{tag25h9_create,tag25h9_destroy}},
		   {fort::tags::Family::Tag36ARTag,{tag36ARTag_create,tag36ARTag_destroy}},
		   {fort::tags::Family::Tag36h10,{tag36h10_create,tag36h10_destroy}},
		   {fort::tags::Family::Tag36h11,{tag36h11_create,tag36h11_destroy}},
		   {fort::tags::Family::Standard41h12,{tagStandard41h12_create,tagStandard41h12_destroy}},
		   {fort::tags::Family::Standard52h13,{tagStandard52h13_create,tagStandard52h13_destroy}},
		   {fort::tags::Family::Circle21h7,{tagCircle21h7_create,tagCircle21h7_destroy}},
		   {fort::tags::Family::Circle49h12,{tagCircle49h12_create,tagCircle49h12_destroy}},
		   {fort::tags::Family::Custom48h12,{tagCustom48h12_create,tagCustom48h12_destroy}},
	};

	auto fi = families.find(d_family);
	if ( fi == families.end() ) {
		return;
	}
	d_atFamily = fi->second.first();
	d_familyDestructor = fi->second.second;
	apriltag_detector_add_family(d_detector,d_atFamily);
}
