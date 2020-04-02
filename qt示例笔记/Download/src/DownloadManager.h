#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H
#include <QObject>
#include <QPointer>

class QNetworkAccessManager;
class QNetworkReply;

class DownloadManager : public QObject
{
	Q_OBJECT
public:
	explicit DownloadManager(QObject *parent = nullptr);
	void downloadFinished(QNetworkReply *reply);

signals:

private:
	QPointer<QNetworkAccessManager> manager;

};

#endif // DOWNLOADMANAGER_H
