## 简介

&emsp;&emsp;在qt示例中，这个示例原本是展示如何使用`QNetworkAccessManager`获取谷歌搜索时弹出的下拉列表的内容的，但是由于某些原因，将本示例改为获取百度下拉列表内容。

&emsp;&emsp;在原示例中谷歌的返回结果是一个xml文档格式如下：

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



本文描述了如下几个问题

1. 使用`QNetworkAccessManager`获取搜索引擎提示列表
2. Qt解析json数据

