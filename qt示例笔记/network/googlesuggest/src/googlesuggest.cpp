/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

//! [1]
#include "googlesuggest.h"
#include <QNetworkProxy>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

//const QString gsuggestUrl(QStringLiteral("http://google.com/complete/search?output=toolbar&q=%1"));
const QString gsuggestUrl(QStringLiteral("https://www.baidu.com/sugrec?prod=pc&wd=%1"));

//! [1]

//! [2]
GSuggestCompletion::GSuggestCompletion(QLineEdit *parent): QObject(parent), editor(parent)
{
    popup = new QTreeWidget;
	//不显示窗口标题栏
	popup->setWindowFlags(Qt::Popup);
	//没有焦点，确保焦点在主界面上
	popup->setFocusPolicy(Qt::NoFocus);
	//将popup的焦点事件交由parent处理
	popup->setFocusProxy(parent);
	//实时检查鼠标位置
    popup->setMouseTracking(true);

    popup->setColumnCount(1);
	//各个item高度一致
    popup->setUniformRowHeights(true);
	//是否可以展开显示子item
	popup->setRootIsDecorated(false);
	//不可以编辑
    popup->setEditTriggers(QTreeWidget::NoEditTriggers);
    popup->setSelectionBehavior(QTreeWidget::SelectRows);
	popup->setFrameStyle(QFrame::Box | QFrame::Plain);
    popup->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	popup->header()->hide();

    popup->installEventFilter(this);

    connect(popup, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
            SLOT(doneCompletion()));

    timer.setSingleShot(true);
    timer.setInterval(500);
	//在输入编辑内容时，每隔0.5秒查找一次
    connect(&timer, SIGNAL(timeout()), SLOT(autoSuggest()));
    connect(editor, SIGNAL(textEdited(QString)), &timer, SLOT(start()));

//	//设置服务器代理，访问外网
//	QNetworkProxy *proxy =
//			new QNetworkProxy(QNetworkProxy::HttpProxy,"192.168.10.201",1081);
//	networkManager.setProxy(*proxy);

	//获取查找的数据
	connect(&networkManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(handleNetworkData(QNetworkReply*)));


}
//! [2]

//! [3]
GSuggestCompletion::~GSuggestCompletion()
{
    delete popup;
}
//! [3]

//! [4]
bool GSuggestCompletion::eventFilter(QObject *obj, QEvent *ev)
{
    if (obj != popup)
        return false;

    if (ev->type() == QEvent::MouseButtonPress) {
        popup->hide();
        editor->setFocus();
        return true;
    }

    if (ev->type() == QEvent::KeyPress) {
        bool consumed = false;
        int key = static_cast<QKeyEvent*>(ev)->key();
        switch (key) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
            doneCompletion();
            consumed = true;
            break;

        case Qt::Key_Escape:
            editor->setFocus();
            popup->hide();
            consumed = true;
            break;

        case Qt::Key_Up:
        case Qt::Key_Down:
        case Qt::Key_Home:
        case Qt::Key_End:
        case Qt::Key_PageUp:
        case Qt::Key_PageDown:
            break;

        default:
            editor->setFocus();
            editor->event(ev);
            popup->hide();
            break;
        }

        return consumed;
    }

    return false;
}
//! [4]

//! [5]
void GSuggestCompletion::showCompletion(const QVector<QString> &choices)
{
    if (choices.isEmpty())
        return;

    const QPalette &pal = editor->palette();
    QColor color = pal.color(QPalette::Disabled, QPalette::WindowText);

    popup->setUpdatesEnabled(false);
    popup->clear();
    for (const auto &choice : choices) {
		auto item  = new QTreeWidgetItem(popup);
        item->setText(0, choice);
        item->setForeground(0, color);
    }

    popup->setCurrentItem(popup->topLevelItem(0));
    popup->resizeColumnToContents(0);
    popup->setUpdatesEnabled(true);

    popup->move(editor->mapToGlobal(QPoint(0, editor->height())));
    popup->setFocus();
    popup->show();
}
//! [5]

//! [6]
void GSuggestCompletion::doneCompletion()
{
    timer.stop();
    popup->hide();
    editor->setFocus();
    QTreeWidgetItem *item = popup->currentItem();
    if (item) {
        editor->setText(item->text(0));
		//调用editer 的 returnPressed 信号
        QMetaObject::invokeMethod(editor, "returnPressed");
    }
}
//! [6]

//! [7]
void GSuggestCompletion::autoSuggest()
{
    QString str = editor->text();
    QString url = gsuggestUrl.arg(str);
    networkManager.get(QNetworkRequest(url));
}
//! [7]

//! [8]
void GSuggestCompletion::preventSuggest()
{
    timer.stop();
}
//! [8]

//! [9]
void GSuggestCompletion::handleNetworkData(QNetworkReply *networkReply)
{
    if (networkReply->error() == QNetworkReply::NoError) {
        QVector<QString> choices;
		QByteArray response(networkReply->readAll());

		QJsonDocument doc(QJsonDocument::fromJson(response));
		QJsonObject obj = doc.object();
		if(obj.contains("g"))
		{
			QJsonArray arr = obj.value("g").toArray();
			for (auto item : arr)
			{
				choices<< item.toObject().value("q").toString();
			}
		}
//        QXmlStreamReader xml(response);
//        while (!xml.atEnd()) {
//            xml.readNext();
//            if (xml.tokenType() == QXmlStreamReader::StartElement)
//                if (xml.name() == "suggestion") {
//                    QStringRef str = xml.attributes().value("data");
//                    choices << str.toString();
//                }
//        }
        showCompletion(choices);
    }

    networkReply->deleteLater();
}
//! [9]
