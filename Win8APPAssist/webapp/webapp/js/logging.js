
(function () {
    "use strict";

    var _getBufferFromString = function(str) { 
        var memoryStream = new Windows.Storage.Streams.InMemoryRandomAccessStream(); 
        var dataWriter = new Windows.Storage.Streams.DataWriter(memoryStream); 
        dataWriter.writeString(str); 
        var buffer = dataWriter.detachBuffer(); 
        dataWriter.close(); 
        return buffer; 
    };

    var _rjust = function (string, width, fillchar) {
        if (fillchar === undefined || fillchar === null) {
            fillchar = ' ';
        }
        string = string.toString();
        var result = '';
        var fillLength = width - string.length;
        for (var i = 0; i < fillLength; ++i) {
            result += fillchar;
        }
        result += string;
        return result;
    };

    var logLevel = {
        verbose: 1,
        info: 2,
        warning: 3,
        error: 4,
    };

    var Logging = WinJS.Class.define(function (filename, minLogLevel, truncatedSize) {
        this._closed = false;
        this.stream = undefined;
        this.rawStream = undefined; // for close
        this.dir = Windows.Storage.ApplicationData.current.localFolder.path;
        this.filename = !!filename ? filename : 'log.txt';
        this.minLogLevel = !!minLogLevel ? minLogLevel : logLevel.verbose;
        this.truncatedSize = !!truncatedSize ? truncatedSize : 1024 * 1024;
        this.queue = [];
    }, {
        _initAsync: function () {
            var that = this;
            var logFile = undefined;
            // not use function in util
            return WinJS.Promise.wrap().then(function () {
                return Windows.Storage.StorageFolder.getFolderFromPathAsync(that.dir);
            }).then(function (folder) {
                return folder.createFileAsync(that.filename, Windows.Storage.CreationCollisionOption.openIfExists);
            }).then(function (file) {
                logFile = file;
            }).then(function () {
                return logFile.getBasicPropertiesAsync();
            }).then(function (properties) {
                var logFileProperties = properties;
                var logSize = properties.size;
                if (logSize > that.truncatedSize) {
                    //truncate
                    var offset = logSize - that.truncatedSize;
                    var newLogSize = that.truncatedSize;
                    var buffer = new Windows.Storage.Streams.Buffer(newLogSize);
                    var outputStream = undefined;
                    var inputStream = undefined;
                    var outputTransaction = undefined;
                    return WinJS.Promise.wrap().then(function () {
                        return logFile.openAsync(Windows.Storage.FileAccessMode.read);
                    }).then(function (stream) {
                        inputStream = stream.getInputStreamAt(offset);
                        return inputStream.readAsync(buffer, that.truncatedSize, Windows.Storage.Streams.InputStreamOptions.none);
                    }).then(function (buffer) {
                        inputStream.close();
                        return logFile.openTransactedWriteAsync();
                    }).then(function (transaction) {
                        outputTransaction = transaction;
                        return outputTransaction.stream.writeAsync(buffer);
                    }).then(function () {
                        outputTransaction.stream.size = newLogSize;
                        return outputTransaction.commitAsync();
                    }).then(function () {
                        outputTransaction.close();
                    });
                }
            }).then(function () {
                return logFile.openAsync(Windows.Storage.FileAccessMode.readWrite);
            }).then(function (stream) {
                that.rawStream = stream;
                that.stream = stream.getOutputStreamAt(stream.size);
                return that;
            });
        },

        getLogTypeString: function () {
            var result = [];
            if (this.minLogLevel <= logLevel.verbose) {
                result.push('log');
            }
            if (this.minLogLevel <= logLevel.info) {
                result.push('info');
            }
            if (this.minLogLevel <= logLevel.warning) {
                result.push('warn');
            }
            if (this.minLogLevel <= logLevel.error) {
                result.push('error');
            }
            return result.join(' ');
        },

        closeAsync: function () {
            var that = this;
            return new WinJS.Promise(function (c, e, p) {
                that._internal_log('log exit', '', 'info');
                that._flush_log(" "/*dummy-string-for-crash-on-empty-string*/, {
                    needClose: true,
                    onCloseComplete: c
                });
            });
        },
        // 为解决unload时，无法执行异步IO，无法关闭文件，导致的快速再次加载时的死循环。
        // bug详见 https://qa.mail.netease.com/issues/show/132886?nav=730
        // 立即关闭文件存在丢失log的风险，慎用
        // mark by: tangjie@crop.netease.com

        closeImmediately: function () {
            this._closed = true
            this._finality();
        },

        _finality: function () {
            if (!!this.stream) {
                this.stream.close();
                delete this.stream;
            }
            if (!!this.rawStream) {
                this.rawStream.close();
                delete this.rawStream;
            }
            delete this;
        },

        _step: function () {
            var that = this;
            if (!that.stream) {
                console.log("Logging should have been ready!");
                return WinJS.Promise.wrap();
            }
            if (that.queue.length === 0) {
                return WinJS.Promise.wrap();
            }
            var data = that.queue[0];
            var message = data.message;
            var option = data.option;
            return WinJS.Promise.wrap().then(function () {
                if (!that._closed)
                    return that.stream.writeAsync(_getBufferFromString(message));
            }).then(function () {
                if (!that._closed)
                    return that.stream.flushAsync();
            }).then(function () {
                if (!that._closed) {
                    if (option.needClose) {
                        that._finality();
                        option.onCloseComplete();
                    }
                    if (option.isFault) {
                        MSApp.terminateApp(message);
                    }
                }
            }).then(function () {
                if (!that._closed) {
                    that.queue.shift();
                    that._step();
                }
            });
        },

        _flush_log: function (message, option) {
            console.log(message);
            this.queue.push({
                message: message,
                option: option
            });
            if (this.queue.length === 1) {
                return this._step();
            }
            return WinJS.Promise.wrap();
        },

        _internal_log: function (message, tag, type) {
            var now = new Date();
            if (type === 'log') {
                type = 'verb';
            }
            var dateString = [
                _rjust(now.getFullYear(), 4, '0'),
                _rjust(now.getMonth() + 1, 2, '0'),
                _rjust(now.getDate(), 2, '0'),
            ].join('-') + ' ' + [
                _rjust(now.getHours(), 2, '0'),
                _rjust(now.getMinutes(), 2, '0'),
                _rjust(now.getSeconds(), 2, '0'),
            ].join(':') + '.' + _rjust(now.getMilliseconds(), 3, '0');
            var string = [
                '[',
                dateString,
                '|',
                _rjust(type, 4),
                '][',
                _rjust(tag, 50),
                ']',
                message
            ].join('');
            if (!message || message[message.length - 1] != '\n') {
                string += '\r\n';
            }
            return this._flush_log(string, {isFault: tag.isFault});
        },

        getLogger: function () {
            var that = this;
            return function (message, tag, type) {
                // TODO(lixianliang): return value?
                that._internal_log(message, tag, type).done();
            };
        },
    }, {
        initAsync: function (filename, minLogLevel) {
            var logging = new Logging(filename, minLogLevel);
            return logging._initAsync();
        }
    });

    var HandyLogger = WinJS.Class.define(function () {
        this.filename = '';
        this.triTupleRegex = /at ([^(]+) \(ms-appx:\/\/[^\/]+([^:]+):([^)]+)\)/;
        this.getTypeRegex = /\[object ([^\]]+)\]/;
        this.logHelper = new native.Util();
    }, {
        _log: function (type, message) {
            var tag = '';
            // for efficiency, we only get source triTuple if we are in debug mode or log level > verbose
            if (this.logHelper.isInDebugMode() || type !== 'verb') {
                // stack[0] is this function, stack[1] is verbose|info|warning|error function
                var stack = this._getStack().splice(2);
                var fileName = 'unknown';
                var funcName = 'unknown';
                var lineNo = 'unknown';
                var triTupleString = stack[0];
                var matchResult = triTupleString.match(this.triTupleRegex);
                if (!!matchResult && matchResult.length == 4) {
                    fileName = matchResult[2];
                    lineNo = matchResult[3];
                    funcName = matchResult[1].replace(' ', '-');
                }
                tag = [
                    funcName,
                    '(',
                    fileName,
                    ':',
                    lineNo,
                    ')'
                ].join('');
                if (type === 'error' || type === 'fault') {
                    message += '\nstack is:\n';
                    message += stack.join('\n');
                }
            }
            if (type === 'verb') {
                type = 'log';
            }
            if (type === 'fault') {
                type = 'error';
                var rawTag = tag;
                tag = {
                    isFault: true,
                    toString: function() {
                        return rawTag;
                    },
                };
            }
            return WinJS.log && WinJS.log(message, tag, type);
        },

        _getMessage: function (rawArgs) {
            var args = [];
            for (var i in rawArgs) {
                var arg = rawArgs[i];
                var argType = Object.prototype.toString.call(arg);
                var m = argType.match(this.getTypeRegex);
                if (!!m) {
                    argType = m[1];
                }
                // null, Array, Object, WinRT Object etc.
                if (argType in { 'Array': 1, 'Object': 1, 'Error': 1, 'Null': 1, 'Undefined': 1}
                    || /^Windows/.test(argType)) {
                    // 之所以要做这样的处理是由于JSON.stringify(undefined)返回为''，
                    // JSON.stringify([undefined, null])返回为'[null, null]', 
                    // JSON.stringify({foo:undefined})返回为'{}'
                    // 综合考虑此处返回'UNDEFINED'字符串来表示这里是一个undefined值

                    // 但是IE下此处有问题，这样处理之后[undefined, null]
                    // JSON.stringify之后结果仍然为[null, null]，原因是其undefined值根本没有
                    // 拿到你提供的replacer中做查询，这个问题请知悉。
                    var logData = JSON.stringify(arg, function (key, value) {
                        if (value === undefined) {
                            return 'UNDEFINED';
                        }
                        return value;
                    });
                    args.push(logData.replace('"UNDEFINED"', "undefined"));
                } else {
                    args.push(arg);
                }
            }
            return args.join(' ');
        },

        _getStack: function () {
            var stack = [];
            try {
                this.logHelper.exceptionForCallStack();
            } catch (e) {
                // remove error message, blank line and this function;
                stack = e.stack.split('\n').splice(3);
            }

            return stack;
        },

        verbose: function() {
            this._log('verb', this._getMessage(arguments));
        },

        info: function() {
            this._log('info', this._getMessage(arguments));
        },

        warning: function() {
            this._log('warn', this._getMessage(arguments));
        },

        error: function() {
            this._log('error', this._getMessage(arguments));
        },

        fault: function () {
            this._log('fault', this._getMessage(arguments));
        },
    });

    WinJS.Namespace.define("Util.Log", {
        Logging: Logging,
        logLevel: logLevel,
    });
    
    WinJS.Namespace.define("Util.Log", {
        /// <summary>日志管理对象</summary>
        LoggerManager: {
            file: undefined,
            logLevel: (new native.Util()).isInDebugMode() ? Util.Log.logLevel.verbose : Util.Log.logLevel.info,
            logging:undefined,
            /// <summary>初使化日志对像</summary>
            /// <summary>初始化log,程序已经初始化过日志，先把日志关闭再进行日志初使化</summary>
            initLoggerAsy: function (file, logLevel) {
                this.file = file;
                this.logLevel = logLevel;
                return WinJS.Promise.wrap().then(function () {
                    if (!!this.logging)
                        this.logging.closeImmediately();
                    return Util.Log.Logging.initAsync('file.txt', logLevel || this.logLevel);
                }.bind(this)).then(function (logging) {
                    this.logging = logging;
                    WinJS.Utilities.startLog({
                        type: logging.getLogTypeString(),
                        action: logging.getLogger(),
                    });
                    Util.Log.info('==============logger====start=======================');
                    return logging;
                }.bind(this))
            }
        }
    })
    
    var handyLogger = new HandyLogger();
    WinJS.Namespace.define("Util.Log", {
        verbose: handyLogger.verbose.bind(handyLogger),
        info: handyLogger.info.bind(handyLogger),
        warning: handyLogger.warning.bind(handyLogger),
        error: handyLogger.error.bind(handyLogger),
        fault: handyLogger.fault.bind(handyLogger)
    });

})();