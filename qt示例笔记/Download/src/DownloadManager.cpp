#include "DownloadManager.h"
#include <QtNetwork>

DownloadManager::DownloadManager(QObject *parent) :
	QObject(parent),
	manager(new QNetworkAccessManager)
{
	connect(manager, &QNetworkAccessManager::finished,
			this, &DownloadManager::downloadFinished);
}

void DownloadManager::downloadFinished(QNetworkReply *reply)
{

}
