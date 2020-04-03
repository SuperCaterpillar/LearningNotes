#include "Win.h"
#include "ui_Win.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QByteArray>

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

}

void Win::on_btQuit_clicked()
{

	close();
}

