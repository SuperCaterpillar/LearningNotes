#include "Win.h"

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QApplication::setApplicationDisplayName("Download");
	Win w;
	w.show();
	return a.exec();
}
