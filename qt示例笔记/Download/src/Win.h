#ifndef WIN_H
#define WIN_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class Win; }
QT_END_NAMESPACE





class Win : public QDialog
{
	Q_OBJECT

public:
	Win(QWidget *parent = nullptr);
	~Win();

private:
	Ui::Win *ui;
};
#endif // WIN_H
