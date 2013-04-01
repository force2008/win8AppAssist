(function () {
    "use strict";
    importScripts('//Microsoft.WinJS.1.0/js/base.js');
    importScripts('/js/logging.js','/js/util.js');

    var init = function () {
        //if (Application.isCurrent)
        //    return;
        if (Util.getData('isAppCurrent').isCurrent)
            return;
        var task = Windows.UI.WebUI.WebUIBackgroundTaskInstance.current;
        var num = task.triggerDetails.content;
        var notifications = Windows.UI.Notifications;
        var notificationManager = notifications.ToastNotificationManager;
        var template = notifications.ToastTemplateType.toastText01;
        var toastXml = notificationManager.getTemplateContent(notifications.ToastTemplateType[template]);

        var textNodes = toastXml.getElementsByTagName("text");
        textNodes[0].appendChild(toastXml.createTextNode("I am push background" + num));
        var toast = new notifications.ToastNotification(toastXml);

        notificationManager.createToastNotifier().show(toast);
        close();
    }
    init();
}())