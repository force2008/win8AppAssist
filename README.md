win8AppAssist
=============
#概述
本文档旨在为win 8 js app开发人员提供一套通用app应用开发解决方案。该方案包括由win8AppAssist（生成的已经包含应用依赖关系的win 8 app工程），应用启动入口分类，应用导航模块，nej的jst模块，动态列表模块，日志模块，nej item模块，绑定item模块，util工具类模块和相应的测试模块。本文档对各模块的使用进行说明。
#	开发流程
win8AppAssist
是一个辅助工具，用来辅助生成win 8 js app，通过该工具用户可以用来配置app应用所包含的依赖项目。
 
* 在 projectName输入app应用的名称，只能为英文和数据的组合，不能是中文。
* Plug in里勾选你的app包含哪些功能，native是必须包含的依赖项目，如果js的开发受限于语言，可以用native进行扩展
* SQLite3是本地关系数据库依赖工程,可以将数据存储在本地。
* NotivationExtensions是app的toast和tile的封装工程，通过这个依赖，可以方便toast的调用。
* JsonXmlConverter是提供了js到xml和xml到js对象之间的转换功能。
* Wbxml可以将二进制的数据和xml格式间进行转换。
* Path app项目路径
* 在ok后会在指定目录，把相关的依赖项目都添加进去，自动生成app工程，同时把相关的模块添加进新生成的工程里。


# 应用设置
在package.appxmanifest里进行app的相关配置，新生成的app工程里把起始页设成了index.html，修改logo以适用相应的app， 这个url里有相关的应用配置指导http://msdn.microsoft.com/en-us/library/windows/apps/hh465346.aspx

# 启动入口
启动入口的主要脚本是js/index.js,在index.js里做了正常启动，search启动，分享启动这三种启动流程，其他的启动流程可以参考, http://msdn.microsoft.com/zh-cn/library/windows/apps/windows.applicationmodel.activation.activationkind.aspx 并有相关的启动方式的demo提供。在启动入口也配置了设置板面的项设置，超级按钮->设置后的对应用本身的设置的配置，同时也对日志进行的初使化，在后面的模块中日志。
#	应用导航
可以在_cachePath对缓存的path进行配置，点击的url和当前的url一致，则刷新当前页面，当前页面模块配置reset方法对页面进行重置
#	Nej 的jst模板模块
(二) Nej 的jst模板模块，win 8提供了动态的模板功能，但模板里不能进行逻辑操作，不能进行format操作等，jst提供了循环，逻辑等操作，只要添加了jst.js，可以和在nej编程一样进行jst的编写工作，对于静态的，有逻辑的页面节点，可以用jst的方式把节点添加到页面上，jst的操作手册：
http://nej.netease.com/course/topic/template/
#	动态列表模块
动态列表模块是开发人员从Nej.Util.ListCache继承，把相应的和服务器交互的函数填写完成，在页面上实例化Nej.Util.Module，在实例化Nej.Util.Module时传入相应的参数和回调函数，就可以完成列表的加载，并在把从服务器加载过来的数据缓存在内存里，Nej.Util.Module的参数如下：
bindDefine	动态绑定对象属性如{title:’’,name:’’}和template的属性相对应
template	WinJS.Binding.Template项模板
parent	列表父节点
onbeforeload	列表加载前回调，可以此回调函数添加loading状态
onafterload	列表加后回调，去掉loading状态
onitemeventprocess	对每个项进行事件添加
onbeforepullrefresh	向前拖动加载数据前回调
onafterpullrefresh	向前拖动加载数据后回调
onbeforeitemsdelete	删除项前回调
onafteritemsdelete	删除项后回调
onbeforeitemadd	添加项前回调
onafteritemadd	添加项后回调
onafteritemload	取项数据回调
unshiftItemAnimation	头部添加列表项动画回调
appendItemAnimation	列表添加时是动画回调
removeItemAnimation	列表项移除时动画回调
onlistempty	列表为空回调
cache	CacheClass Nej.Util.ListCache继承而来的缓存类
这个模是一数据,Item的结合封装，可以线过这个封装，用把数据从Nej.Util.ListCache取到数据后，对个列表数据进行动态绑定，并添加相关的事件，也可以在各项上添加动画以达到更丰富的效果。
Util.Element.addEvent
提供了给节点添加事件的方法，主要针对MSPointer(touch、pen、mouse)类的事件，主要是用pointer摸拟swipe轻扫的手势。只要添加swipe(Right,Left,Down,UP)事件，可以用MSPointer完成swipe手势
SWIPETHRESHOLDX，SWIPETHRESHOLDY灵敏度值
addEvent添加swipe方法

# Util工具模块
所在脚本js/util.js
Util.Network网络管理类，在用registerCallback后，网络的变动都会回调到注册的函数，isCurrentConnected获取网络当前状态。
Util.Element提供了以下几种方法，方便开发人员操作节点
findAncestor	查找符合条件的第一个祖先节点
hasClassName	判断是否有指定的className
getRect	获取浮动元素的4个点的位置,确定浮动元素的位置
getData	获取节点data-key的属性值
scroll	滚动祖先元素的滚动条，使元素处于可视状态
empty	清空节点
removeByEC	暂时移除节点
createElement	生成节点
Util.Installation安装包版本信息，获取安装地址和获取版版号
Util模块提供的api
MD5	MD5加密
parseURL	解析url返回location标准定义的对象
query2object	将查询字符串转换成对象<
isTypeOf	类型判断
indexOf	数组indexOf的衍生
reverseEach	逆向循环数组
f	空函数
r	空数组
setData	往localSetting添加键值数据
getData	从localSetting获取键值数据
encode	编码字符串　如：encode({r:/\d/g,'9':'t'},'99999')
escape	编码html代码，'<' -> '&lt;'
unescape	反编码html代码，'&lt;' -> '<'
var2date	转化日期
format	格式化时间，yyyy|yy|MM|M|dd|d|HH|H|mm|ms|ss|m|s
getFileSize	计算文件大小
mix	将对象源对象的属性混入到目的对象
randString	生成伪随机字符串
randNumberString	生成伪随机数
createContainer	在Windows.Storage.ApplicationData.current.localSettings新建一个key对象，可以用来存放数据
requestLockScreenAccess	请求锁屏权限，锁屏权限和background task关联在一起
registerBackgroundTask	在前台注册后台任务
generateChannel	生成推送信道
getChannel	获取推送信道
modifyChannel	修改推送信道

# Util.PopupMenu卡片类封装

# Toast封装
使用方法详见toast.js


# 日志模块封闭
在应用起动时调用Util.Log.LoggerManager.initLoggerAsy，传入日志文件名称，和日志级别，日志级别在debug模式为verbose,否则为info级别，在初使化完LoggerManager后，可以用Util.log进行日志记录。
在重新初使化时，对日志是否初使化进行判断，如果日志已经打开，先进行关闭，再重新初使化日志流程。

# Nej item模块
这个模块是从nej迁移过来，通常情况下，我们用Nej.Util.Module这个模块可以完成相应的功能，如果是nej的开发者，偏好于nej开发，可用其开发列表项，可以从Nej.Util.ListCache继承实现相关的取数据接口，把取到的列表数据用从Nej.Util.Item继承而来的类实现之，可以在item项上设计各自的动画实现，在app里动画是很重要的一个环节，所在在每个节点都有可能添加动画。
SQLite3
只在在win8AppAssist把SQlite3引入，就会把SQlite3相关的脚本添加到工程里，js/data/sqlite_ext.js和sqlite3.js
初使化数据库文件，用runAsync运行sql语句，对应用提供的接口有
runAsync	Create table,insert,delete等sql语句
allAsync	Select返回多条数据
oneAsync	返回一条数据sql
eachAsync	取到数据时，调用回调函数，可以格式化数据记录
mapAsync	取到数据时，调用回调函数，并把回调后的数据以数组形式返回
itemDataSource	getCount
itemsFromIndex requestIndex, countBefore, countAfter从数据库里分页取列表
groupDataSource	getCount分组数
itemsFromIndex
分页取分组数据
将网络上的数据同步到本地后，就可以用本地的数据进行展示。

# 绑定item模块
从Nej.Util.BindItem继承，并实现相关的事件逻辑，可以在实例化module时传入BindItem的类名，事件回调和template名称，外部可以监测BindItem的逻辑操作行为。
# 项目引用添加
C++有很多成熟的dll项目，在win 8 app如果要调用现在的项目的api，可以先把项目以引用形式添加进来，把该项目的主要api找出来，编写头文件和cpp文件，简单类型的数据返回，和普通的C++ api开发类似。
相关的win 8 的cx语言参考：
http://msdn.microsoft.com/zh-cn/library/windows/apps/hh699871.aspx

