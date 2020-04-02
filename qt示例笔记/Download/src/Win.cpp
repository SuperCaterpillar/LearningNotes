#include "Win.h"
#include "ui_Win.h"

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

