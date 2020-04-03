#include "Win.h"

#include <QApplication>
#include <QTextCodec>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF8"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
	//QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF8"));

	QApplication::setApplicationDisplayName("Download");
	Win w;
	w.show();
	return a.exec();
}
