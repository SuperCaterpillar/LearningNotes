## 简介

&emsp;&emsp;在qt示例中，这个示例原本是展示如何使用`QNetworkAccessManager`获取谷歌搜索时弹出的下拉列表的内容的，但是由于某些原因，将本示例改为获取百度下拉列表内容。

&emsp;&emsp;在原示例中谷歌的返回结果是一个xml文档,格式如下：

```xml
<!--搜索qt时返回如下内容-->
<toplevel>
<CompleteSuggestion>
<suggestion data="qt"/>
</CompleteSuggestion>
<CompleteSuggestion>
<suggestion data="qttabbar"/>
</CompleteSuggestion>
<CompleteSuggestion>
<suggestion data="qt creator"/>
</CompleteSuggestion>
<CompleteSuggestion>
<suggestion data="qt教程"/>
</CompleteSuggestion>
<CompleteSuggestion>
<suggestion data="qtranslate"/>
</CompleteSuggestion>
<CompleteSuggestion>
<suggestion data="千图"/>
</CompleteSuggestion>
<CompleteSuggestion>
<suggestion data="全统广场"/>
</CompleteSuggestion>
<CompleteSuggestion>
<suggestion data="青团"/>
</CompleteSuggestion>
<CompleteSuggestion>
<suggestion data="晴天"/>
</CompleteSuggestion>
<CompleteSuggestion>
<suggestion data="情头"/>
</CompleteSuggestion>
</toplevel>
```

&emsp;&emsp;修改后，使用百度的返回结果为
```json
{
	"q":"qt",
	"p":false,
	"g":
		[
			{"type":"sug","sa":"s_1","q":"情头"},
			{"type":"sug","sa":"s_2","q":"qt是什么"},
			{"type":"sug","sa":"s_3","q":"qtv6青岛电视台直播"},
			{"type":"sug","sa":"s_4","q":"qty什么意思中文翻译"},
			{"type":"sug","sa":"s_5","q":"qt间期延长"},
			{"type":"sug","sa":"s_6","q":"qts是什么意思"},
			{"type":"sug","sa":"s_7","q":"qtc间期正常范围"},
			{"type":"sug","sa":"s_8","q":"qt语音"},
			{"type":"sug","sa":"s_9","q":"qt间期"},
			{"type":"sug","sa":"s_10","q":"丘钛"}
		],
	"slid":"13696160320886506820",
	"queryid":"0x18893f193d8544"
}
```



## 演示

## 代码解析

1. 获取百度提供的API， 这些值可以通过浏览器调试 `F12`来抓取

    ```c++
    //百度提供的下拉列表api 最后的%1用来传递参数
    const QString gsuggestUrl(QStringLiteral("https://www.baidu.com/sugrec?prod=pc&wd=%1"));
    //百度搜索关键字的api 最后的%1用来传递参数
    const QString gsearchUrl = QStringLiteral("https://www.baidu.com/s?wd=%1");
    ```

2. 获取百度传回的下拉列表数据

   ```c++
   void GSuggestCompletion::autoSuggest()
   {
       QString str = editor->text();
       QString url = gsuggestUrl.arg(str);
       networkManager.get(QNetworkRequest(url));
   }
   
   void GSuggestCompletion::handleNetworkData(QNetworkReply *networkReply)
   {
       if (networkReply->error() == QNetworkReply::NoError) {
           QVector<QString> choices;
   		QByteArray response(networkReply->readAll());
   		//将原版的xml解析改为json，以适应百度返回值
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
   ```

   