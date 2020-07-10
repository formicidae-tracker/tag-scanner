#include <QApplication>

#include "MainWindow.hpp"

int main(int argc, char ** argv) {

	QCoreApplication::setOrganizationName("FORmicidae Tracker");
	QCoreApplication::setOrganizationDomain("formicidae-tracker.github.io");
	QCoreApplication::setApplicationName("FORT Tag Scanner");
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QApplication fortTagScanner(argc,argv);

	MainWindow main;
	main.show();


	return fortTagScanner.exec();
}
