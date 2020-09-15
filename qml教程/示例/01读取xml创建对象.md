## 使用qml读取xml文件

### 要读取的xml文件

```xml
//文件名 config.xml
<?xml version="1.0" ?>
<title>
	<btn name="" width="50" height="50" img="" color="red" />
	<btn name="" width="50" height="50" img="" color="blue" />
	<btn name="" width="50" height="50" img="" color="yellow" />
	<btn name="" width="50" height="25" img="" color="black" />
</title>
```

* 在写xml文件时要注意文件编码,否则在读取的时候可能出现无法解析的情况,这种情况不太好定位,可以使用qtcreator创建一个scxml文件,然后修改一下.

![image-20200915182619675](/home/jc/data/myDoc/LearningNotes/qml教程/示例/img/image-20200915182619675.png)

### 使用到的qml对象

* [`XmlListModel`](https://doc.qt.io/qt-5/qml-qtquick-xmllistmodel-xmllistmodel.html)

  XmlListModel用于从XML数据创建只读模型。它可用作视图元素（例如ListView，PathView，GridView）和与模型数据交互的其他元素（例如Repeater）的数据源。

* [`XmlRole`](https://doc.qt.io/qt-5/qml-qtquick-xmllistmodel-xmlrole.html)

  根据xml文件定义XmlListModel的解析规则

#### 读取示例

```js
//<btn name="" width="50" height="50" img="" color="red" />
XmlListModel{
        id: model
        source: "./config.xml"  //读取xml行对应本文件的路径
        query: "/title/btn"  	//Xpath路径  从根路径开始最前面一定要以'/'开始
        XmlRole{
            name: "name"		//使用model对象访问时的名称
            query: "@name/string()"//@表示是btn元素的属性, 如果不是元素则去掉@符号,
        }
        XmlRole{
            name: "width"
            query: "@width/number()"
        }
        XmlRole{
            name: "height"
            query: "@height/number()"
        }
        XmlRole{
            name: "img"
            query: "@name/string()"
        }
        XmlRole{
            name: "color"
            query: "@color/string()"
        }
```

#### 获取读取到的数据

在获取数据时一定要保证数据加载完毕,判断方法是在`XmlListModel`中添加信号处理函数

```js
 onStatusChanged: {
    	 //加载完毕后XmlListModel的Status属性会发生改变,如果值为XmlListModel.Ready,就表示数据已经加载完毕,可以获取了
            if(status == XmlListModel.Ready && count > 0)
                {
                     //row是一个Row对象 readOver是一个自定义信号,下文可以看到
                     row.readOver()
                    console.log(model.get(0).img)//获取model对象中第0条数据, img就是在XmlRole中定义的名称
                }
                
        }

```

### 创建对象

qml动态穿件对象有两种方法,具体大家可以参考qt文档在本例中使用第二种方法

1. [Qt.createQmlObject()](https://doc.qt.io/qt-5/qml-qtqml-qt.html#createQmlObject-method) 
2. [Qt.createComponent()](https://doc.qt.io/qt-5/qml-qtqml-qt.html#createComponent-method) 

#### step1 构造被创建对象

现在我们新建一个叫做TBtn.qml的文件,文件内容如下,我们将会动态创建这个对象

```js
import QtQuick 2.0

Rectangle {
    id:btn
    width: 32
    height: 32
    color: "red"
}
```

#### step2 构建布局

构建一个布局,将动态创建好的对象加入其中,我们现在使用简单的`Row`布局

```js
Row {
        id:row
        signal readOver //读取xml加载完毕可以读取时发出这个信号
        spacing: 2
        onReadOver: {
            for(var i = 0; i < model.count; i++)
                MyScript.createSpriteObjects(i);//js穿件对象函数
        }
   	}
```

#### step3 使用js方法穿件对象

```js
var component;
var tbtn;
function createSpriteObjects(i) {
    component = Qt.createComponent("TBtn.qml");
    if (component.status === Component.Ready)
        finishCreation(i);
    else
        component.statusChanged.connect(finishCreation);

}

function finishCreation(i) {
    if (component.status === Component.Ready) {
        var index = model.get(i) //获取对去到的数据
        tbtn = component.createObject(row, {width:index.width, height:index.height, color:index.color});//为创建的对象设置属性
        if (tbtn === null) {
            // Error Handling
            console.log("Error creating object");
        }
    } else if (component.status === Component.Error) {
        // Error Handling
        console.log("Error loading component:", component.errorString());
    }
}
```

### 效果演示

![image-20200915201011076](/home/jc/data/myDoc/LearningNotes/qml教程/示例/img/image-20200915201011076.png)

