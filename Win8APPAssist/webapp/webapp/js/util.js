/// <summary>库工具</summary>
/// Util.Network    网络管理对象，用来监测断网，连网的事件回调
/// Util.Package    应用安装包信息，版本信息，安装路径等
/// Util.MD5        MD5加密
/// array2object,isTypeOf,indexOf,setData,getData,encode,escape,unescape,var2date,format,getFileSize,createContainer,
/// requestLockScreenAccess, registerBackgroundTask, generateChannel, getChannel, modifyChannel
(function () {
    "use strict";

    // Network State Manager
    WinJS.Namespace.define("Util.Network", {
        /// <summary>网络状态管理对象</summary>
        NetworkStateManager: {
            callback: [],
            isConnected: undefined,
            networkInfo: Windows.Networking.Connectivity.NetworkInformation,
            /// <summary>注册网络状态回调事件</summary>
            registerCallback: function (callback) {
                var idx = -1;
                this.callback.forEach(function (item, index) {
                    if (callback == item)
                        idx = index;
                })
                if (idx != -1)
                    this.callback[idx] = callback;
                else
                    this.callback.push(callback)
            },
            /// <summary>注销网络状态回调事件</summary>
            unreisterCallback: function () {
                this.networkInfo.removeEventListener("networkstatuschanged", this.onNetworkChanged.bind(this));
            },
            /// <summary>网络状态改变时回调</summary>
            onNetworkChanged: function () {
                var isConnected = !!this.networkInfo.getInternetConnectionProfile();
                if (this.isConnected !== isConnected) {
                    this.isConnected = isConnected;
                    this.callback.forEach(function (item) {
                        item(this.isConnected);
                    }.bind(this))
                }
                this.isConnected = isConnected;
            },
            /// <summary>获取当前网络状态</summary>
            isCurrentConnected: function () {
                this.isConnected = !!this.networkInfo.getInternetConnectionProfile();
                return this.isConnected;
            },
            setState: function () {
                this.networkInfo.addEventListener("networkstatuschanged", this.onNetworkChanged.bind(this));
                this.isConnected = !!this.networkInfo.getInternetConnectionProfile();
            }
        }
    });

    (function () {
        Util.Network.NetworkStateManager.setState();
    }())

    WinJS.Namespace.define("Util", {
        /// <summary>安装包信息</summary>
        Package: {
            CurrentPackage: Windows.ApplicationModel.Package.current,
            /// <summary>获取版本号</summary>
            getVersion: function () {
                var version = this.CurrentPackage.id.version;
                var versions = [version.build, version.major, version.minor, version.revision].join('.');
                return versions;
            },
            /// <summary>获取安装路径</summary>
            getInstalledLocation: function () {
                return this.CurrentPackage.installedLocation.path;
            }

        },
        /// <summary>MD5加密</summary>
        /// <param name="str" type="String">加密字符串</param>
        MD5: function (str) {
            var Algorithm = Windows.Security.Cryptography.Core.HashAlgorithmProvider.openAlgorithm(Windows.Security.Cryptography.Core.HashAlgorithmNames.md5);
            var buff = Windows.Security.Cryptography.CryptographicBuffer.convertStringToBinary(str, Windows.Security.Cryptography.BinaryStringEncoding.Utf8);
            var hashed = Algorithm.hashData(buff);
            var res = Windows.Security.Cryptography.CryptographicBuffer.encodeToHexString(hashed);
            return res;
        },
        /// <summary>解析location</summary>
        /// <param name="url" type="String">地址</param>
        /// <returns type="Object" />
        /// case 0: NM.Util.parseLocation("http://www.163.com/news?a=12&b=3#c=mm&d=nn") --> {
        ///     href: "http://www.163.com/news?a=12&b=3#c=mm&d=nn",
        ///     pathname:"http://www.163.com/news", searh:"a=12&b=3", hash: "c=mm&d=nn"}
        parseURL: function (url) {
            if (typeof url !== "string") return null;
            var result = {};
            result.href = url;
            var arr = url.match(/([^?#]*)(\?[^#]*)?(#.*)?/);
            result.pathname = arr[1];
            result.search = arr[2] ? arr[2].slice(1) : arr[2];
            result.hash = arr[3] ? arr[3].slice(1) : arr[3];
            return result;
        },
        /// <summary>将查询字符串转换成对象</summary>
        /// <param name="query" type="String">查询字符串</param>
        /// <returns type="Object"/>
        /// case: NM.Util.query2object("a=123&b=mm") --> {a:"123", b:"mm"}
        query2object: function (query) {
            if (typeof query != "string" || !query.length) return null;
            var units = query.split("&");
            var queryObject = {};
            units.forEach(function (unit) {
                var arr = unit.split("=");
                queryObject[arr[0]] = arr[1];
            });
            return queryObject;
        },
        /// <summary>将数组根据指定键值转换成对象</summary>
        /// <param name="arr" type="Array">数组</param>
        /// <param name="key" type="String">键值，可选参数，默认为"id"</param>
        /// <returns type="Object" />
        array2object: function (arr, key) {
            if (!arr || !arr.forEach || !arr.length) return null;
            var result = {};
            key = key || "id";
            arr.forEach(function (item) {
                if (item && item[key]) {
                    result[item[key]] = item;
                }
            });
            return result;
        },
        /// <summary>判断类型</summary>
        /// <param name="data" type="Object">具体数据</param>
        /// <param name="type" type="String">类型（Function | String | Date object）</param>
        isTypeOf: function (data, type) {
            if (data === null) return _type == 'null';
            if (data === undefined) return _type == 'undefined';
            return Object.prototype.toString.call(data) == '[object ' + type + ']';
        },
        /// <summary>返回指定项在数组的index</summary>
        /// <param name="list" type="Array">数组</param>
        /// <param name="item" type="item | Function">查询项</param>
        /// <returns type="Number" /> -1没有查到该项
        /// <summary>Utile.indexOf(list,function(item){return item.xx=xx})或Utile.indexOf(list,item)</summary>
        indexOf: function (list, item) {
            var filter = this.isTypeOf(item, 'Function') ? item
                        : function (value) { return value === item; };
            var index = -1;
            for (var i = 0, l = list.length; i < l; i++) {
                if (filter.call(null, list[i])) {
                    index = i;
                    break;
                }
            }
            return index;
        },
        setData: function (key, data) {
            this.createContainer('app-data');
            var composite = new Windows.Storage.ApplicationDataCompositeValue();
            if (!this.isTypeOf(data, 'Object')) {
                composite[key] = data;
            }
            else if (this.isTypeOf(data, 'Object')) {
                for (var property in data) {
                    if (!this.isTypeOf(data[property], 'Object') && data.hasOwnProperty(property))
                        composite[property] = data[property];
                }
            }
            this.localSettings.containers.lookup('app-data').values[key] = composite;
        },
        getData: function (key) {
            if (this.localSettings.containers.hasKey('app-data')) {
                return !!this.localSettings.containers.lookup('app-data').values[key] ?
                    this.localSettings.containers.lookup('app-data').values[key] : {};
            }
        },
        /// <summary>编码字符串　如：encode({r:/\d/g,'9':'t'},'99999')</summary>
        /// <param name="map" type="map">正则表达式和替换内容</param>
        /// <param name="content" type="String">源内容</param>
        /// <returns type="String" />
        encode: function (map, content) {
            if (!map || !content || !content.replace)
                return content || '';
            return content.replace(map.r, function ($1) {
                var result = map[!map.i ? $1.toLowerCase() : $1];
                return result != null ? result : $1;
            });
        },
        /// <summary>编码html代码，'<' -> '&lt;'</summary>
        escape: (function () {
            var map = {
                r: /\<|\>|\&|\r|\n|\s|\'|\"/g,
                '<': '&lt;', '>': '&gt;', '&': '&amp;', ' ': '&nbsp;', '"': '&quot;', "'": '&#39;', '\n': '<br/>', '\r': ''
            };
            return function (content) {
                return this.encode(map, content);
            };
        })(),
        /// <summary>反编码html代码，'&lt;' -> '<'</summary>
        unescape: (function () {
            var map = {
                r: /\&(?:lt|gt|amp|nbsp|#39|quot)\;|\<br\/\>/gi,
                '&lt;': '<', '&gt;': '>', '&amp;': '&', '&nbsp;': ' ', '&#39;': "'", '&quot;': '"', '<br/>': '\n'
            };
            return function (content) {
                var e = document.createElement('div');
                e.innerHTML = content;
                return e.childNodes.length === 0 ? "" : e.childNodes[0].nodeValue;
                //return this.encode(map, content);
            };
        })(),
        /// <summary>转化日期</summary>
        /// <returns type="Date" />
        var2date: function (time) {
            var date = time;
            if (this.isTypeOf(time, 'String'))
                date = new Date(Date.parse(time));
            if (!this.isTypeOf(time, Date))
                date = new Date(time);
            return date;
        },
        /// <summary>格式化时间，yyyy|yy|MM|M|dd|d|HH|H|mm|ms|ss|m|s</summary>
        format: (function () {
            var _map = { i: !0, r: /\byyyy|yy|MM|M|dd|d|HH|H|mm|ms|ss|m|s\b/g };
            var _fmtnmb = function (_number) {
                _number = parseInt(_number) || 0;
                return (_number < 10 ? '0' : '') + _number;
            };
            return function (_time, _format) {
                if (!_time || !_format)
                    return '';
                _time = this.var2date(_time);
                _map['yyyy'] = _time.getFullYear();
                _map['yy'] = ('' + _map['yyyy']).substr(2);
                _map['M'] = _time.getMonth() + 1;
                _map['MM'] = _fmtnmb(_map['M']);
                _map['d'] = _time.getDate();
                _map['dd'] = _fmtnmb(_map['d']);
                _map['H'] = _time.getHours();
                _map['HH'] = _fmtnmb(_map['H']);
                _map['m'] = _time.getMinutes();
                _map['mm'] = _fmtnmb(_map['m']);
                _map['s'] = _time.getSeconds();
                _map['ss'] = _fmtnmb(_map['s']);
                _map['ms'] = _time.getMilliseconds();
                return this.encode(_map, _format);
            }
        })(),
        /// <summary>计算文件大小</summary>
        getFileSize: function (_size, _rate) {
            _rate = _rate || 1;
            _size = parseInt(_size) * _rate;
            // 文件大小 初始Byte
            var fileSize = '0B';
            if (_size >= 1024 * 1024 * 1024) {
                fileSize = parseInt((_size / (1024 * 1024 * 1024)) * 100) / 100 + 'GB'
            }
            else
                if (_size >= 1024 * 1024) {
                    fileSize = (parseInt((_size / (1024 * 1024)) * 100) / 100).toFixed(1) + 'MB'
                }
                else
                    if (_size >= 1024) {
                        fileSize = (parseInt((_size / 1024) * 100) / 100).toFixed(0) + 'KB'
                    }
                    else {
                        fileSize = _size.toFixed(0) + 'B';
                    }
            return fileSize;
        },
        /// <summary>将对象源对象的属性混入到目的对象</summary>
        /// <param name="to" type="Object">目的对象</param>
        /// <param name="from" type="Object">源对象</param>
        /// <returns type="Object">目的对象</returns>
        mix: function (to, from) {
            if (to == null && from == null) return null;
            to = to || {};
            if (from) {
                for (var p in from) {
                    to[p] = from[p];
                }
            }
            return to;
        },
        localSettings: Windows.Storage.ApplicationData.current.localSettings,
        createContainer: function (key) {
            this.localSettings.createContainer(key, Windows.Storage.ApplicationDataCreateDisposition.always);
        },
        /// <summary>请求锁屏权限</summary>
        /// <summary>锁屏权限和background task关联在一起</summary>
        ///try cache 如果抛出request lockscreen access error申请没有成功，如果request lockscreen access exception，很大可能是已经申请过，第一次申请时用户的是拒绝还是允许的状态，后面无法做改变
        /// <returns type="void" />
        requestLockScreenAccess: function () {
            var Background = Windows.ApplicationModel.Background;

            try {
                Background.BackgroundExecutionManager.requestAccessAsync().then(function (result) {
                    switch (result) {
                        case Background.BackgroundAccessStatus.denied:
                            //logger.info("This app is not on the lock screen.");
                            break;
                        case Background.BackgroundAccessStatus.allowedMayUseActiveRealTimeConnectivity:
                            //logger.info("The app is added to the lock screen and can set up background tasks, but it cannot use the real-time connectivity (RTC) broker.");
                            break;

                        case Background.BackgroundAccessStatus.allowedWithAlwaysOnRealTimeConnectivity:
                            //logger.info("The app is added to the lock screen, can set up background tasks, and, if it has the capability, can use the real-time connectivity (RTC) broker.");
                            break;

                        case Background.BackgroundAccessStatus.unspecified:
                            //logger.info("The user has not yet taken any action. This is the default setting and the app is not on the lock screen.");
                            break;

                        default:
                            break;
                    }
                }, function (err) {
                    throw -1;
                });
            } catch (exp) {
                throw 0;
            }
        },
        /// <summary>注册background task</summary>
        /// <param name="taskEntryPoint" type="String">taskEntryPoint</param>
        /// <param name="name" type="String">background name</param>
        /// <param name="trigger" type="String">激活类型</param>
        /// <param name="condition" type="String">激活条亻</param>
        /// <summary>Util.registerBackgroundTask('js\\push.js', 'NeteaseMailPushTask', new Windows.ApplicationModel.Background.PushNotificationTrigger, null);</summary>
        /// @tutorial http://msdn.microsoft.com/en-us/library/windows/apps/hh977045.aspx
        registerBackgroundTask: function (taskEntryPoint, name, trigger, condition) {
            //
            // Check for existing registrations of this background task.
            //
            try {
                var taskRegistered = false;

                var background = Windows.ApplicationModel.Background;
                var iter = background.BackgroundTaskRegistration.allTasks.first();

                while (iter.hasCurrent) {
                    var task = iter.current.value;
                    if (task.name === name) {
                        taskRegistered = true;
                        break;
                    }
                    iter.moveNext();
                }
                //
                // If the task is already registered, return the registration object.
                //
                if (taskRegistered != false) {
                    return iter.current;
                }
                //
                // Register the background task.
                //
                var builder = new Windows.ApplicationModel.Background.BackgroundTaskBuilder();
                builder.name = name;
                builder.taskEntryPoint = taskEntryPoint;
                builder.setTrigger(trigger);

                if (condition != null) {
                    builder.AddCondition(condition);
                }
                var task = builder.register();
            }
            catch (error) {
                throw -1;
            }
            //task.addEventListener("progress", new BackgroundTaskSample.progressHandler(task).onProgress);
            //task.addEventListener("completed", new BackgroundTaskSample.completeHandler(task).onCompleted);

            return task;
        },
        /**
         * 申请channel
         * @return {void} 
         */
        generateChannel: function () {
            this.createContainer('channel');
            var pushNotifications = Windows.Networking.PushNotifications;
            var channelOperation = pushNotifications.PushNotificationChannelManager.createPushNotificationChannelForApplicationAsync();

            return channelOperation.then(function (channel) {
                var oldChannel = Util.getChannel();
                if (oldChannel && (oldChannel.uri == channel.uri))
                    channel.modified = false;
                else
                    channel.modified = true;
                Util.modifyChannel(channel);
                return channel;
            }, function (err) {

                return WinJS.Promise.wrapError();
            });
        },
        getChannel: function () {
            if (this.localSettings.containers.hasKey('channel'))
                return this.localSettings.containers.lookup('channel').values['channelInfo'];
            else
                return;
        },
        modifyChannel: function (channel) {
            var composite = new Windows.Storage.ApplicationDataCompositeValue();
            composite['uri'] = channel.uri;
            composite['expirationTime'] = channel.expirationTime;
            this.localSettings.containers.lookup('channel').values['channelInfo'] = composite;
        }
    })
})();
