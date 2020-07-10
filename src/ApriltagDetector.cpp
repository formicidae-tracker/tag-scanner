#include "ApriltagDetector.hpp"


ApriltagDetector::ApriltagDetector(QObject * parent)
	: QObject(parent)
	, d_family(fort::tags::Family::Undefined)
	, d_threshold(40)
	, d_minCluster(25) {
	connect(this,&ApriltagDetector::familyChanged,
	        this,&ApriltagDetector::updateDetector);

	connect(this,&ApriltagDetector::thresholdChanged,
	        this,&ApriltagDetector::updateDetector);

	connect(this,&ApriltagDetector::clusterMinSizeChanged,
	        this,&ApriltagDetector::updateDetector);

}

ApriltagDetector::~ApriltagDetector() {

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

void ApriltagDetector::processFrame(cv::Mat mat) {

}


void ApriltagDetector::updateDetector() {
}
