#ifndef WIN_H
#define WIN_H

#include <QDialog>
#include <QMap>
#include "DownloadManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Win; }
QT_END_NAMESPACE


class QProgressBar;
class QNetworkReply;

class Win : public QDialog
{
	Q_OBJECT

public:
	Win(QWidget *parent = nullptr);
	~Win();
	bool isRunDownload();
public slots:
	void on_btUrlFile_clicked();
	void on_btDownloadPath_clicked();
	void on_btDownload_clicked();
	void on_btQuit_clicked();

	void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
	Ui::Win *ui;
	QMap<QNetworkReply*, QProgressBar*>  replyMap;
	DownloadManager manager;
};
#endif // WIN_H
