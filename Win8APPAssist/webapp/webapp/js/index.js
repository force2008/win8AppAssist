// For an introduction to the Navigation template, see the following documentation:
// http://go.microsoft.com/fwlink/?LinkId=232506
(function () {
    "use strict";

    WinJS.Binding.optimizeBindingReferences = true;

    var app = WinJS.Application;
    var activation = Windows.ApplicationModel.Activation;
    var nav = WinJS.Navigation;
    var networkState = Util.Network.NetworkStateManager;
    var logLevel = (new native.Util()).isInDebugMode() ? Util.Log.logLevel.verbose : Util.Log.logLevel.info;
    var log = Util.Log;
    app.addEventListener("activated", function (args) {
        /// <summary>http://msdn.microsoft.com/zh-cn/library/windows/apps/windows.applicationmodel.activation.activationkind.aspx</summary>
        if (args.detail.kind === activation.ActivationKind.launch) {
            initLaunch(args);
        }
        else if (args.detail.kind === activation.ActivationKind.search) {
            initSearchLaunch(args);
           
        }
        else if (args.detail.kind === activation.ActivationKind.shareTarget) {
            /// <summary>http://msdn.microsoft.com/zh-cn/library/windows/apps/hh465255.aspx</summary>
            /// <summary>共享内容各项操作</summary>
            var shareData = Application.shareOperation.data;
            if (data.contains(Windows.ApplicationModel.DataTransfer.StandardDataFormats.html)) { }
            else if (data.contains(Windows.ApplicationModel.DataTransfer.StandardDataFormats.text)) { }
            else if (data.contains(Windows.ApplicationModel.DataTransfer.StandardDataFormats.uri)) { }
            else if (data.contains(Windows.ApplicationModel.DataTransfer.StandardDataFormats.storageItems)) { }
        }
        else if (args.detail.kind === activation.ActivationKind.file) { }
        else if (args.detail.kind === activation.ActivationKind.protocol) { }
        else if (args.detail.kind === activation.ActivationKind.fileOpenPicker) { }
        else if (args.detail.kind === activation.ActivationKind.fileSavePicker) { }
        else if (args.detail.kind === activation.ActivationKind.cachedFileUpdater) { }
        else if (args.detail.kind === activation.ActivationKind.contactPicker) { }
        else if (args.detail.kind === activation.ActivationKind.device) { }
        else if (args.detail.kind === activation.ActivationKind.printTaskSettings) { }
        else if (args.detail.kind === activation.ActivationKind.cameraSettings) { }
        else if (args.detail.kind === activation.ActivationKind.cameraSettings) { }
    });
    initSearch();
    initCommondRequest();
    function initCommondRequest() {
        // 设置功能初始化
        Windows.UI.ApplicationSettings.SettingsPane.getForCurrentView().oncommandsrequested = onCommandRequest;
    }

    function onCommandRequest(event) {
        var user = NM.Data.getCurAccount();
        var applicationCommands = event.request.applicationCommands;
        applicationCommands.clear();

        var accounts = NM.Data.getAccounts();
        if (accounts.length != 0) {
            var multiAccountsCommand = new Windows.UI.ApplicationSettings.SettingsCommand("accounts", '帐号', function () {
                //WinJS.Navigation.navigate("/pages/multid/multid.html");
                console.log('帐号');
            });
            applicationCommands.append(multiAccountsCommand);
        }
        var feedbackCommand = new Windows.UI.ApplicationSettings.SettingsCommand("feedback", '反馈', function () {
            console.log('帐号');
        });
        applicationCommands.append(feedbackCommand);
        var declareCommand = new Windows.UI.ApplicationSettings.SettingsCommand("declare", '申明', function () {
            var uri = new Windows.Foundation.Uri("http://gb.corp.163.com/gb/legal.html");
            var options = new Windows.System.LauncherOptions();
            options.treatAsUntrusted = true;
            Windows.System.Launcher.launchUriAsync(uri, options).done(function (success) {
                if (success) { } else { }
            });
        });
        applicationCommands.append(declareCommand);
    }
    function initSearch() {
        var searchPane = Windows.ApplicationModel.Search.SearchPane.getForCurrentView();
        var xhrPromise;
        searchPane.addEventListener("querysubmitted", onQuerySubmitted, false);
        searchPane.addEventListener('visibilitychanged', function (evt) {
            if (evt.visible && evt.target.queryText == "") {
                searchPane.placeholderText = '搜索内容';
            }
        });

    }
    function onQuerySubmitted(event) {
        console.log(event.queryText.trim());
        nav.navigate("/pages/search/search.html?queryText=" + event.queryText.trim());
    }
    function initSearchLaunch(args) {
        args.setPromise(WinJS.UI.processAll().then(function () {
            // 初始化log,程序已经初始化过日志，不可再进行初使化，所以要进行是否进行初始化的逻辑处理
            return Util.Log.LoggerManager.initLoggerAsy('log_search.txt', logLevel);
        }).then(function (logging) {
            log.info('nav location:' + nav.location);
            var searchquery = args.detail.queryText.trim();
            log.info('searchquery:' + searchquery);
            nav.navigate("/pages/search/search.html?queryText=" + searchquery);
            //nav.navigate("/pages/search/search.html?queryText=" + args.queryText.trim());

            //if (nav.location) {
            //    nav.history.current.initialPlaceholder = true;
            //    return nav.navigate(nav.location, nav.state);
            //} else {
            //    return nav.navigate(Application.navigator.home);
            //}
        }))
    }
    function initLaunch(args) {
        //networkState.registerCallback(function (isConnect) {
        //    console.log("1:" + isConnect)
        //})
        //networkState.registerCallback(function (isConnect) {
        //    console.log("2:" + isConnect)
        //})
        //if (app.sessionState.history) {
        //    nav.history = app.sessionState.history;
        //}
        var needInitLog = false;
        //已经启动了的应用，再次激活时，不能再进行log初始化了，原先的log没有关闭呢
        args.setPromise(WinJS.UI.processAll().then(function () {
            // 初始化log,程序已经初始化过日志，不可再进行初使化，所以要进行是否进行初始化的逻辑处理
            return Util.Log.LoggerManager.initLoggerAsy('log.txt', logLevel);
        }).then(function (logging) {
            log.info('nav location:' + nav.location);
            log.info(Util.Package.getVersion());
            console.log(Util.Package.getVersion());
            console.log(Util.Package.getInstalledLocation());
            if (args.detail.arguments) {
                var params = JSON.parse(args.detail.arguments);
                if (params && params.type) {
                    switch (params.type) {
                        case 'toast':
                            return nav.navigate('/pages/toast/toast.html?type=' + params.type);
                    }
                }
            } else {
                return nav.navigate(Application.navigator.home);
            }
        }))
    }

    WinJS.Namespace.define("Application", {})
    document.addEventListener("visibilitychange", function (evt) {
        var state = document["visibilityState"];
        if (state == "visible") {
            Application.isCurrent = true;
            Util.setData('isAppCurrent', { isCurrent: true });
        }
        else if (state == "hidden") {
            Util.setData('isAppCurrent', { isCurrent: false });
        }
    }, false);
    app.oncheckpoint = function (args) {
        // TODO: This application is about to be suspended. Save any state
        // that needs to persist across suspensions here. If you need to 
        // complete an asynchronous operation before your application is 
        // suspended, call args.setPromise().
        app.sessionState.history = nav.history;
    };

    app.start();
})();
