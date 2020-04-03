#include "DownloadManager.h"
#include <QtNetwork>
#include <QMessageBox>

DownloadManager::DownloadManager(QObject *parent) :
	QObject(parent),
	manager(new QNetworkAccessManager)
{
	connect(manager, &QNetworkAccessManager::finished,
			this, &DownloadManager::downloadFinished);
}

bool DownloadManager::isHttpRedirect(QNetworkReply *reply)
{
	int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
	return statusCode == 301 || statusCode == 302 || statusCode == 303
			|| statusCode == 305 || statusCode == 307 || statusCode == 308;
}

bool DownloadManager::saveToDisk(const QString &filename, QIODevice *data)
{
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly)) {
		fprintf(stderr, "Could not open %s for writing: %s\n",
				qPrintable(filename),
				qPrintable(file.errorString()));
		return false;
	}

	file.write(data->readAll());
	file.close();

	return true;
}

QString DownloadManager::saveFileName(QUrl &url)
{
	QString path = url.path();
	QString basename = QFileInfo(path).fileName();
	if (QFile::exists(basename)) {
		// already exists, don't overwrite
		int i = 0;
		basename += '.';
		while (QFile::exists(basename + QString::number(i)))
			++i;

		basename += QString::number(i);
	}
	return basename;
}

void DownloadManager::downloadFinished(QNetworkReply *reply)
{
	QUrl url = reply->url();
	if(reply->error())
		return ;

}

void DownloadManager::doDownload(const QUrl &url)
{
	QNetworkRequest request(url);
	QNetworkReply *reply = manager->get(request);

	currentDownloads.append(reply);
}


