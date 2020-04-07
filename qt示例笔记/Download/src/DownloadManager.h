#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H
#include <QObject>
#include <QPointer>
#include <QVector>
#include <QUrl>

class QNetworkAccessManager;
class QNetworkReply;
class QIODevice;

struct ReplyData
{
	QString fileName;
	QNetworkReply* reply;
};

class DownloadManager : public QObject
{
	Q_OBJECT
public:
	explicit DownloadManager(QObject *parent = nullptr);

	ReplyData doDownload(const QUrl &url);

	void setSavePath(const QString& path);

public slots:
	void downloadFinished(QNetworkReply *reply);

signals:

private:
	QString saveFileName(const QUrl& url);
	bool isHttpRedirect(QNetworkReply *reply);
	bool saveToDisk(const QString &filename, QIODevice *data);
private:
	QPointer<QNetworkAccessManager> manager;
	QString m_path;
};

#endif // DOWNLOADMANAGER_H
