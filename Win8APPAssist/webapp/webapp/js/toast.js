/// <summary>toast类型封装</summary>
/// reference NotificationExtentions project

(function () {
    "use strict";

    WinJS.Namespace.define("Toast", {
        Notifications : Windows.UI.Notifications,
        ToastContent: NotificationsExtensions.ToastContent,
        /// <summary>一句话toast</summary>
        toastText01:function(body,launchObj){
            var content = this.ToastContent.ToastContentFactory.createToastText01();
            content.textBodyWrap.text = body;
            content.launch = JSON.stringify(launchObj);
            this.showToast(content);
        },
        /// <summary>head短，body两行的toast</summary>
        toastText02: function (head, body, launchObj) {
            var content = this.ToastContent.ToastContentFactory.createToastText02();
            content.textHeading.text = head;
            content.textBodyWrap.text = body;
            content.launch = JSON.stringify(launchObj);
            this.showToast(content);
        },
        /// <summary>head长，占两行，body短</summary>
        toastText03: function (head, body, launchObj) {
            var content = this.ToastContent.ToastContentFactory.createToastText03();
            content.textHeadingWrap.text = head;
            content.textBody.text = body;
            content.launch = JSON.stringify(launchObj);
            this.showToast(content);
        },
        /// <summary>head短，两个短body</summary>
        toastText04: function (head, body1, body2, launchObj) {
            var content = this.ToastContent.ToastContentFactory.createToastText04();
            content.textHeading.text = head;
            content.textBody1.text = body1;
            content.textBody2.text = body2;
            content.launch = JSON.stringify(launchObj);
            this.showToast(content);
        },
        /// <summary>一句话，图片地址</summary>
        toastImageAndText01: function (body, imgUrl, launchObj) {
            var content = this.ToastContent.ToastContentFactory.createToastImageAndText01();
            content.textBodyWrap.text = body;
            content.launch = JSON.stringify(launchObj);
            this.setToastImage(content,imgUrl);
        },
        /// <summary>head一行，body两行，图片地址</summary>
        toastImageAndText02: function (head, body, imgUrl, launchObj) {
            var content = this.ToastContent.ToastContentFactory.createToastImageAndText02();
            content.textHeading.text = head;
            content.textBodyWrap.text = body;
            content.launch = JSON.stringify(launchObj);
            this.setToastImage(content,imgUrl);
        },
        /// <summary>长head占两行，短body,图片地址</summary>
        toastImageAndText03: function (head, body, imgUrl, launchObj) {
            var content = this.ToastContent.ToastContentFactory.createToastImageAndText03();
            content.textHeadingWrap.text = head;
            content.textBody.text = body;
            content.launch = JSON.stringify(launchObj);
            this.setToastImage(content,imgUrl);
        },
        /// <summary>head一行，body1一行，body2一行,图片地址</summary>
        toastImageAndText04: function (head, body1, body2, imgUrl, launchObj) {
            var content = this.ToastContent.ToastContentFactory.createToastImageAndText04();
            content.textHeading.text = head;
            content.textBody1.text = body1;
            content.textBody2.text = body2;
            content.launch = JSON.stringify(launchObj);
            this.setToastImage(content,imgUrl);
        },
        setToastImage:function(content,imgUrl){
            content.image.src = imgUrl;
            content.image.alt = content;
            this.showToast(content);
        },
        /// <summary>NotificationExtensions　生成的Content对象</summary>
        showToast:function(content){
            var toast = content.createNotification();
            var notificationManager = this.Notifications.ToastNotificationManager;
            notificationManager.createToastNotifier().show(toast);
        }
        
    });
}())


/// <summary>Tile metro应用桌面上的方块信息，动态的内容</summary>
/// <summary>Tile各模块内容和表现形式参照　http://msdn.microsoft.com/zh-cn/library/windows/apps/hh761491.aspx　</summary>
/// <summary>在设置wide　tile只同时设计square块是为了在tile是小块时也可以显示tile内容</summary>
/// <summary>循环播放设置 Windows.UI.Notifications.TileUpdateManager.createTileUpdaterForApplication().enableNotificationQueue(true);或flase</summary>
/// <summary>清空tile Windows.UI.Notifications.TileUpdateManager.createTileUpdaterForApplication().clear()</summary>
/// <summary>badge是指tile底部的信息，如个数，图标等</summary>
