#include "Win.h"
#include "ui_Win.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QByteArray>
#include <QProgressBar>
#include <QNetworkReply>
#include "DownloadManager.h"

Win::Win(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::Win)
{
	ui->setupUi(this);
}

Win::~Win()
{
	delete ui;
}

bool Win::isRunDownload()
{
	for(auto key : replyMap.keys())
	{
		if(key->isRunning())
			return true;
	}
	return false;
}

void Win::on_btUrlFile_clicked()
{
	QString urlFile = QFileDialog::getOpenFileName(this, "url file", "./", "*.url");
	QFile file(urlFile);
	if (file.open(QIODevice::ReadOnly))
	{
		QByteArray content = file.readAll();
		QString strContent = content.data();
		ui->leUrl->setText(strContent);
	}
}

void Win::on_btDownloadPath_clicked()
{
	QString savePath = QFileDialog::getExistingDirectory(this, "保存路径");
	ui->leDownloadPath->setText(savePath);
}

void Win::on_btDownload_clicked()
{
	if(isRunDownload())
	{
		QMessageBox::information(this,"提示", "还有未完成的任务，请稍等");
		return;
	}
	else
	{
		for(auto w : replyMap.values())
		{
			ui->pbLayout->removeWidget(w);
		}
		for (auto data = replyMap.begin(); data != replyMap.end(); data++)
		{
			data.key()->deleteLater();
			data.value()->deleteLater();
		}
		replyMap.clear();
	}
	QString strUrl = ui->leUrl->text();
	if(strUrl.isEmpty())
		strUrl = ui->leUrl->placeholderText();

	QString strPath = ui->leDownloadPath->text();
	if(strPath.isEmpty())
		strPath = ui->leDownloadPath->placeholderText();

	QStringList urlList = strUrl.split("\n");
	qDebug()<<strUrl<<urlList.isEmpty()<<urlList.count()<<urlList[0]<< endl;
	if(urlList.isEmpty())
		return;

	manager.setSavePath(strPath);
	for(QString url : urlList)
	{
		if(url.isEmpty())
			continue;
		ReplyData replyData = manager.doDownload(url);
		QProgressBar* proBar = new QProgressBar;
		proBar->setFormat(replyData.fileName + "已下载%p%");

		ui->pbLayout->addWidget(proBar);


		connect(replyData.reply, &QNetworkReply::downloadProgress,
				this, &Win::downloadProgress);

		replyMap.insert(replyData.reply, proBar);
	}
}

void Win::on_btQuit_clicked()
{
	close();
}

void Win::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
	QNetworkReply* reply = (QNetworkReply*)sender();
	replyMap[reply]->setRange(0, bytesTotal);
	replyMap[reply]->setValue(bytesReceived);
}

