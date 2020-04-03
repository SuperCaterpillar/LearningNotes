#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H
#include <QObject>
#include <QPointer>
#include <QVector>
#include <QUrl>

class QNetworkAccessManager;
class QNetworkReply;

class DownloadManager : public QObject
{
	Q_OBJECT
public:
	explicit DownloadManager(QObject *parent = nullptr);
	void downloadFinished(QNetworkReply *reply);
	void doDownload(const QUrl &url);
	bool isHttpRedirect(QNetworkReply *reply);
signals:

private:
	QString saveFileName(QUrl& url);
	bool saveToDisk(const QString &filename, QIODevice *data);
private:
	QPointer<QNetworkAccessManager> manager;
	QVector<QNetworkReply *> currentDownloads;
};

#endif // DOWNLOADMANAGER_H
