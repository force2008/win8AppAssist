(function () {
    "use strict";

    WinJS.UI.Pages.define("/pages/home/home.html", {
        // This function is called whenever a user navigates to this page. It
        // populates the page elements with the app's data.
        ready: function (element, options) {
            // TODO: Initialize the page here.
            var a = [{ name: 'a', id: '1' }, { name: 'b', id: '2' }, { name: 'c', id: '3' }];
            var b = [1, 2, 3, 4];
            var index = Util.indexOf(a, function (item) { return item.name == 'a' });
            console.log(index);
            var index = Util.indexOf(b, 2);
            console.log(index);
            var t = Util.escape('<div>hello world</div>');
            console.log(t);
            t = Util.unescape('<div>hello world</div>');
            console.log(t);
            var date = new Date();
            console.log(Util.format(date, 'yyyy / MM / DD'))
            document.getElementById('test').innerText = t;
            try {
                Util.requestLockScreenAccess();
                Util.registerBackgroundTask('js\\push.js', 'NeteaseMailPushTask2', new Windows.ApplicationModel.Background.PushNotificationTrigger, null);
            } catch (e) {
                if (e == -1)
                    console.log('request lock screen failed')
                else if (e == 0)
                    console.log('have already request')
            }

            Util.generateChannel().then(function (channel) {
                if (!channel.modified)//no subcribe
                {
                    var channel_uri = channel.uri;
                    var url = "http://localhost:8080/test/push/?url=" + encodeURIComponent(channel_uri);
                    var oReq = new XMLHttpRequest();
                    oReq.open("GET", url, true);

                    oReq.onreadystatechange = function () {
                        if (oReq.readyState == 4) {
                            console.log('subscribe success');
                        }
                    }

                    oReq.send();
                }
                else {//unsubcribe from server,then subcribe

                }
                channel.addEventListener('pushnotificationreceived', this.pushNotificationReceived.bind(this));
            }.bind(this), function (err) {
                console.log('subscribe push service error.');
            })




            var index = 1;
            setInterval(function () {
                if (index < 5)
                    this.toast('Image', index);
                else
                    this.toast('', index-4);
                index++;
                if (index > 8)
                    index = 1;
            }.bind(this),3000)
        },
        toast: function (type, index) {
            var launch = { "type": "toast" };
            if (type != 'Image') {
                switch (Number(index)) {
                    case 1:
                        Toast.toastText01("新闻背景：有一名中国男子在女儿出生后无法接受相貌奇丑的女儿，进而怀疑妻子的容貌,谁知果真如此，妻子承认整容，男子申请离婚。", launch)
                        break;
                    case 2:
                        Toast.toastText02("新闻背景", "有一名中国男子在女儿出生后无法接受相貌奇丑的女儿，进而怀疑妻子的容貌,谁知果真如此，妻子承认整容，男子申请离婚。", launch)
                        break;
                    case 3:
                        Toast.toastText03("有一名中国男子在女儿出生后无法接受相貌奇丑的女儿，进而怀疑妻子的容貌,谁知果真如此，妻子承认整容，男子申请离婚。", "新闻背景：", launch)
                        break;
                    case 4:
                        Toast.toastText03("新闻背景", "有一名中国男子在女儿出生后无法接受相貌奇丑的女儿，进而怀疑妻子的容貌,谁知果真如此，妻子承认整容，男子申请离婚。", "test", launch)
                        break;
                }
            }
            else {
                var imgUrl = "http://i2.sinaimg.cn/ty/1098/2012/0531/U687P6T1098D1F32497DT20130313091732.jpg";
                switch (index) {
                    case 1:
                        Toast.toastImageAndText01("新闻背景：有一名中国男子在女儿出生后无法接受相貌奇丑的女儿，进而怀疑妻子的容貌,谁知果真如此，妻子承认整容，男子申请离婚。", imgUrl, launch)
                        break;
                    case 2:
                        Toast.toastImageAndText02("新闻背景", "有一名中国男子在女儿出生后无法接受相貌奇丑的女儿，进而怀疑妻子的容貌,谁知果真如此，妻子承认整容，男子申请离婚。", imgUrl, launch)
                        break;
                    case 3:
                        Toast.toastImageAndText03("有一名中国男子在女儿出生后无法接受相貌奇丑的女儿，进而怀疑妻子的容貌,谁知果真如此，妻子承认整容，男子申请离婚。", "新闻背景：", imgUrl, launch)
                        break;
                    case 4:
                        Toast.toastImageAndText03("新闻背景", "有一名中国男子在女儿出生后无法接受相貌奇丑的女儿，进而怀疑妻子的容貌,谁知果真如此，妻子承认整容，男子申请离婚。", "test", imgUrl, launch)
                        break;
                }
            }
        },
        pushNotificationReceived: function (event) {
            if (!Util.getData('isAppCurrent').isCurrent)
                return;
            var num = event.rawNotification.content;
            var notifications = Windows.UI.Notifications;

            var notificationManager = notifications.ToastNotificationManager;

            var template = notifications.ToastTemplateType.toastText01;
            var toastXml = notificationManager.getTemplateContent(notifications.ToastTemplateType[template]);

            var textNodes = toastXml.getElementsByTagName("text");
            textNodes[0].appendChild(toastXml.createTextNode("I am push foreground" + num));

            var toast = new notifications.ToastNotification(toastXml);
            notificationManager.createToastNotifier().show(toast);
        }
    });
})();
