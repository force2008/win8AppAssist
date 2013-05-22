(function () {

    var _ckey = 'dat-' + (+new Date);
    /// <summary>缓存基类</summary>
    var BaseCache = WinJS.Class.define(function (_options) {
        this.__cache = this.constructor[_ckey];
        if (!this.__cache) {
            this.__cache = {};
            // request loading information
            this.__cache[_ckey + '-l'] = {};
            this.constructor[_ckey] = this.__cache;
        }
        this.batEvent(_options);
        this._reset(_options);
    }, {
        batEvent: function (_options) {
            for (var _key in _options)
                if (Util.isTypeOf(_options[_key], 'Function')) {
                    this.addEventListener(_key, _options[_key]);
                }
        },
        /// <summary>从缓存中取数据</summary>
        /// <param name="_key" type="String">缓存键值</param>
        /// <returns type="Variable" >缓存数据</returns>
        _getDataInCache: function (_key) {
            return this.__cache[_key];
        },
        /// <summary>数据存入缓存</summary>
        /// <param name="_key" type="String">缓存键值</param>
        /// <param name="_value" type="Variable">缓存数据</param>
        _setDataInCache: function (_key, _value) {
            this.__cache[_key] = _value;
        },
        /// <summary>带默认值取本地数据</summary>
        /// <param name="_key" type="String">键值</param>
        /// <param name="_default" type="String">默认值</param>
        /// <returns type="Variable" >默认数据</returns>
        _getDataInCacheWithDefault: function (_key, _default) {
            var _data = this.__getDataInCache(_key);
            if (_data == null) {
                _data = _default;
                this.__setDataInCache(_key, _data);
            }
            return _data;
        },
        /// <summary>删除缓存数据，不传键值则清除所有缓存</summary>
        /// <param name="_key" type="String">缓存键值</param>
        _delDataInCache: function (_key) {
            if (_key != null) {
                delete this.__cache[_key];
                return;
            }
            for (var _ckey in this.__cache) {
                if (_key == (_ckey + '-l')) return;
                this.__delDataInCache(_key);
            }
        },
        /// <summary>从本地存储中取数据</summary>
        /// <param name="_key" type="String">存储键值</param>
        /// <returns type="String" />
        _getDataInStorage: function (_key) {
            return localStorage[_key];
        },
        /// <summary>数据存入本地缓存</summary>
        /// <param name="_key" type="String">存储键值</param>
        /// <param name="_value" type="String">存储数据</param>
        _setDataInStorage: function (_key, _value) {
            if (Util.isTypeOf == 'String')
                localStorage[_key] = _value;
            else
                localStorage[_key] = JSON.stringify(_value);
        },
        /// <summary>取本地数据,检测内存和本地存储</summary>
        /// <param name="_key" type="String">键值</param>
        _getDataLocal: function (_key) {
            var _data = this._getDataInCache(_key);
            if (_data != null) return _data;
            _data = this._getDataInStorage(_key);
            if (_data != null)
                this._setDataInCache(_key, _data);
            return _data;
        },
        /// <summary>存本地数据</summary>
        /// <param name="_key" type="String">键值</param>
        /// <param name="_value" type="String">数据</param>
        _setDataLocal: function (_key, _value) {
            this._setDataInStorage(_key, _value);
            this._setDataInCache(_key, _value);
        },
        /// <summary>清除本地缓存，不传键值则清除所有缓存</summary>
        /// <param name="_key" type="String">缓存键值</param>
        _delDataLocal: function (_key) {
            if (_key != null) {
                delete this.__cache[_key];
                delete localStorage[_key];
                return;
            }
            for (var _ckey in this.__cache) {
                if (_key == (_ckey + '-l')) return;
                this.__delDataLocal(_key);
            }
        },
        /// <summary>清除缓存数据</summary>
        clearDataLocal: function () {
            this.__delDataLocal();
            return this;
        },
        /// <summary>请求回调</summary>
        /// <param name="_key" type="String">请求标识</param>
        _doCallbackRequest: function (_key) {
            var _data = this.__cache[_ckey + '-l'],
                _args = Util.r.slice.call(arguments, 1);
            for (var _k in _data[_key]) {
                try {
                    _data[_key][_k].apply(null, _args);
                } catch (ex) {
                    // ignore
                    console.error(ex.message);
                    console.error(ex.stack);
                }
            }
            delete _data[_key];
        },
        /// <summary>缓存请求</summary>
        /// <param name="_key" type="String">请求标识</param>
        /// <param name="_callback" type="Function">请求回调</param>
        /// <returns type="Boolean" >是否已存在相同请求</returns>
        _doQueueRequest: function (_key, _callback) {
            _callback = _callback || _f;
            var _list = this.__cache[_ckey + '-l'][_key];
            if (!_list) {
                _list = [_callback];
                this.__cache[_ckey + '-l'][_key] = _list;
                return !1;
            }
            _list.push(_callback);
            return !0;
        },
        /// <summary>检测列表中是否已存在指定片段数据</summary>
        /// <param name="_list" type="Array">列表</param>
        /// <param name="_offset" type="Number">偏移量</param>
        /// <param name="_limit" type="Number">数量，0表示全列表，默认为0</param>
        /// <returns type="Boolean" >是否已经存在</returns>
        _hasFragment: function (_list, _offset, _limit) {
            if (!_list) return !1;
            _offset = parseInt(_offset) || 0;
            _limit = parseInt(_limit) || 0;
            if (!_limit) {
                if (!_list.loaded)
                    return !1;
                _limit = _list.length;
            }
            // length is list total number
            if (!!_list.loaded)
                _limit = Math.min(_limit,
                         _list.length - _offset);
            for (var i = 0; i < _limit; i++)
                if (!_list[_offset + i])
                    return !1;
            return !0;
        }
    }, {});

    WinJS.Class.mix(BaseCache, WinJS.Utilities.eventMixin);
    var BaseListCache = WinJS.Class.derive(BaseCache, function (_options) { BaseCache.apply(this, arguments) }, {
        _reset: function (_options) {
            //BaseCache.prototype._reset.apply(this._options);
            this.__key = _options.key || 'id';
            this.__data = _options.data || {};

            this._doSwapCache(_options.id);
        },
        _destroy: function () {
            delete this.__key;
            delete this.__data;
            delete this.__lspl;
        },
        /// <summary>切换缓存</summary>
        /// <param name="_id" type="String">缓存标识</param>
        _doSwapCache: function (_id) {
            var _cache;
            if (!!_id) {
                _cache = this.__cache[_id];
                if (!_cache) {
                    _cache = {};
                    this.__cache[_id] = _cache;
                }
            }
            _cache = _cache || this.__cache;
            _cache.hash = _cache.hash || {};
            // hash    [Object] - item map by id
            // list    [Array]  - default list
            // x-list  [Array]  - list with key 'x'
            this.__lspl = _cache;
        },
        /// <summary>缓存列表项</summary>
        /// <param name="_item" type="Object">列表项</param>
        /// <param name="_lkey" type="String">列表标识</param>
        /// <returns type="Object" ></returns>
        _doSaveItemToCache: function (_item, _lkey) {
            _item = this._doFormatItem(
                          _item, _lkey) || _item;
            if (!_item) return null;
            var _key = _item[this.__key];
            if (!!_key)
                this._getHash()[_key] = _item;
            return _item;
        },
        /// <summary>格式化数据项，子类实现具体业务逻辑</summary>
        _doFormatItem: Util.f,
        /// <summary>删除列表项</summary>
        /// <param name="_id" type="String">列表项标识</param>
        _doRemoveItemInCache: function (_id) {
            var _item = this._getHash()[_id];
            delete this._getHash()[_id];
            return _item;
        },
        /// <summary>向前追加列表项至列表</summary>
        /// <param name="_key" type="String">列表标识</param>
        /// <param name="_item" type="Object">列表项</param>
        _doUnshiftToList: function (_key, _item) {
            if (!_item) return;
            if (!Util.isTypeOf(_item, 'Array')) {
                var _list = this.getListInCache(_key),
                    _item = this._doSaveItemToCache(_item, _key);
                if (!!_item) _list.unshift(_item);
                return;
            }
            Util.reverseEach(
                _item, this.
                _doUnshiftToList.bind(this, _key)
            );
        },
        /// <summary>设置列表总数</summary>
        setTotal: function (_key, _total) {
            var _list = this.getListInCache(_key);
            _list.length = Math.max(_list.length, _total);
            this.setLoaded(_key);
            return this;
        },
        /// <summary>取列表总长度</summary>
        /// <param name="_key" type="String">列表标识</param>
        getTotal: function (_key) {
            return this.getListInCache(_key).length;
        },
        /// <summary>设置未知长度列表的载入完成标志</summary>
        /// <param name="_key" type="String">列表标识</param>
        /// <param name="_loaded" type="Boolean">是否载入</param>
        setLoaded: function (_key, _loaded) {
            this.getListInCache(_key).loaded = _loaded != !1;
            return this;
        },
        /// <summary>设置未知长度列表的载入完成标志</summary>
        /// <param name="_key" type="String">列表标识</param>
        /// <param name="_loaded" type="Boolean">是否载入</param>
        getLoaded: function (_key) {
            return this.getListInCache(_key).loaded;
        },
        /// <summary>设置列表，清除原有列表</summary>
        /// <param name="_key" type="String">列表标识</param>
        /// <param name="_list" type="String">列表</param>
        setListInCache: function (_key, _list) {
            this.clearListInCache(_key);
            this.__getList({
                key: _key,
                offset: 0,
                limit: _list.length + 1
            }, {
                list: _list,
                total: _list.length
            });
        },
        getListInCache: (function () {
            var _doFormatKey = function (_key) {
                return (_key || '') + (!_key ? '' : '-') + 'list';
            };
            return function (_key) {
                var _key = _doFormatKey(_key),
                    _list = this.__lspl[_key];
                if (!_list) {
                    _list = [];
                    this.__lspl[_key] = _list;
                }
                return _list;
            };
        })(),
        /// <summary>取Hash映射表</summary>
        _getHash: function () {
            var _hash = this.__lspl.hash;
            if (!_hash) {
                _hash = {};
                this.__lspl.hash = _hash;
            }
            return _hash;
        },
        /// <summary>前向刷新列表</summary>
        /// <param name="_options" type="Object">可选配置参数,key列表标识，data发送到服务器数据信息</param>
        pullRefresh: (function () {
            var _doFormatKey = function (_options) {
                return 'r-' + _options.key;
            };
            return function (_options) {
                var _ropt = Util.mix({}, _options),
                    _rkey = _doFormatKey(_ropt);
                if (!this._doQueueRequest(_rkey,
                     this.dispatchEvent.bind(this, 'onpullrefresh'))) {
                    _ropt.rkey = _rkey;
                    _ropt.onload = this._pullRefresh.bind(this, _ropt);
                    this.dispatchEvent('dopullrefresh', _ropt);
                }
                return this;
            };
        })(),
        /// <summary>前向取列表回调</summary>
        /// <param name="_options" type="Object">请求信息</param>
        /// <param name="_list" type="Array">数据列表</param>
        _pullRefresh: function (_options, _list) {
            _options.offset = 0;
            _options.limit = _list.length;
            this._doUnshiftToList(_options.key, _list);
            this._doCallbackRequest(_options.rkey, _options);
        },
        /// <summary>取列表</summary>
        /// <param name="_options" type="Object">可选配置，key,data,offset,limit</param>
        /// <returns type="ListCache" />
        getList: (function () {
            var _doFormatKey = function (_options) {
                return 'r-' +
                       _options.key + '-' +
                       _options.offset + '-' +
                       _options.limit;
            };
            return function (_options) {
                _options = _options || _o;
                var _ropt = {
                    key: _options.key || ''
                            , data: _options.data || null
                            , offset: parseInt(_options.offset) || 0
                            , limit: parseInt(_options.limit) || 0
                },
                    _list = this.getListInCache(_ropt.key);
                if (this._hasFragment(_list,
                          _ropt.offset, _ropt.limit)) {
                    this.dispatchEvent('onlistload', _ropt);
                    return this;
                }
                var _rkey = _doFormatKey(_ropt);
                if (!this._doQueueRequest(_rkey,
                     this.dispatchEvent.bind(this, 'onlistload'))) {
                    _ropt.rkey = _rkey;
                    _ropt.onload = this._getList.bind(this, _ropt);
                    this.dispatchEvent('doloadlist', _ropt);
                }
                return this;
            };
        })(),
        /// <summary>取列表回调</summary>
        /// <param name="_options" type="Obejct">请求信息</param>
        /// <param name="_resul" type="Object">列表数据</param>
        _getList: (function () {
            var _doClear = function (_item, _index, _list) {
                if (!!_item) {
                    return !0;
                }
                _list.splice(_index, 1);
            };
            var _getOffest = function (_chlist) {
                for (var i = 0, l = _chlist.length; i < l; i++) {
                    if (!_chlist[i]) {
                        return i;
                    }
                }
            };
            return function (_options, _result) {
                _options = _options || _o;
                // save list to cache
                var _key = _options.key,
                    _offset = _options.offset,
                    _chlist = this.getListInCache(_key);
                // list with total
                // {total:12,result:[]} 或者 {total:13,list:[]}
                var _list = _result;
                if (!Util.isTypeOf(_list, 'Array')) {
                    _list = _result.result || _result.list || [];
                    var _total = parseInt(_result.total) || 0;
                    if (_total > _list.length) {
                        this.setTotal(_key, _total);
                    }
                }
                // merge list
                for (var i = 0, l = _list.length; i < l; i++) {
                    _chlist[_offset + i] = this.
                             _doSaveItemToCache(_list[i], _key);
                }
                //_u._$forEach(_list,
                //    function(_item,_index){
                //        _chlist[_offset + _index] = this.
                //             _doSaveItemToCache(_item, _key);
                //    },this);
                // check list all loaded
                if (_list.length < _options.limit) {
                    this.setLoaded(_key);
                    _options.offset = _getOffest(_chlist) || _options.offset;
                    Util.reverseEach(_chlist, _doClear);
                    //for (var l = _chlist.length - 1, i = l; i >= 0; i--)
                    //    _doClear(_chlist[i], i, _chlist);
                }
                _options.offset = _getOffest(_chlist) || _options.offset;
                Util.reverseEach(_chlist, _doClear);
                _options.limit = (_list.length < _options.limit) ? _list.length : _options.limit;
                // do callback
                this._doCallbackRequest(_options.rkey, _options);
            };
        })(),
        /// <summary>清除缓存列表</summary>
        /// <param name="_key" type="String">列表标识</param>
        clearListInCache: (function () {
            var _doClear = function (_item, _index, _list) {
                _list.splice(_index, 1);
            };
            return function (_key) {
                var _list = this.getListInCache(_key);
                Util.reverseEach(_list, _doClear);

                this.setLoaded(_key, !1);
                return this;
            };
        })(),
        /// <summary>从缓存中取列表项</summary>
        getItemInCache: function (_id) {
            return this._getHash()[_id];
        },
        /// <summary>取列表项</summary>
        /// <param name="_options" type="Object">请求信息</param>

        getItem: (function () {
            var _doFormatKey = function (_options) {
                return 'r-' + _options.key + '-' + _options.id;
            };
            return function (_options) {
                _options = _options || _o;
                var _id = _options[this.__key],
                    _ropt = {
                        id: _id,
                        key: _options.key || '',
                        data: _options.data || {}
                    };
                _item = this.getItemInCache(_id);
                _ropt.data[this.__key] = _id;
                if (!!_item) {
                    this.dispatchEvent('onitemload', _ropt);
                    return this;
                }
                var _rkey = _doFormatKey(_ropt);
                if (!this._doQueueRequest(_rkey,
                     this.dispatchEvent.bind(this, 'onitemload'))) {
                    _ropt.rkey = _rkey;
                    _ropt.onload = this._getItem.bind(this, _ropt);
                    this.dispatchEvent('doloaditem', _ropt);
                }
                return this;
            };
        })(),
        /// <summary>取列表项回调</summary>
        /// <param name="_options" type="Object">请求信息</param>
        /// <param name="_item" type="Object">列表项对象</param>
        _getItem: function (_options, _item) {
            _options = _options || _o;
            this._doSaveItemToCache(_item, _options.key);
            this._doCallbackRequest(_options.rkey, _options);
        },
        /// <summary>添加列表项</summary>
        /// <param name="_options" type="Object">配置信息,key 列表标识，data 列表项数据，push是否追加到列表尾部</param>
        addItem: function (_options) {
            _options = Util.mix({}, _options);
            _options.onload = this._addItem.bind(this, _options);
            this.dispatchEvent('doadditem', _options);
        },
        /// <summary>添加列表项回调</summary>
        /// <param name="_options" type="Object">请求信息</param>
        /// <param name="_item" type="Object">列表项对象</param>
        _addItem: function (_options, _item) {
            var _key = _options.key;
            _item = this._doSaveItemToCache(_item, _key);
            if (!!_item) {
                var _flag = 0,
                    _list = this.getListInCache(_key);
                if (!_options.push) {
                    _flag = -1;
                    _list.unshift(_item);
                } else if (_list.loaded) {
                    _flag = 1;
                    _list.push(_item);
                } else {
                    // add total
                    _list.length++;
                }
            }
            var _event = {
                key: _key,
                flag: _flag,
                data: _item,
                action: 'add',
                ext: _options.ext
            };
            this.dispatchEvent('onitemadd', _event);
            return _event;
        },

        /// <summary>删除列表项</summary>
        /// <param name="_options" type="Object">配置信息 key 列表标识,id 列表项标识,data 列表项数据信息 ,ext  需要回传的数据信息</param>
        deleteItems: function (_options) {
            _options = Util.mix({}, _options);
            _options.onload = this._deleteItems.bind(this, _options);
            this.dispatchEvent('dodeleteitems', _options);
        },
        /// <summary>删除列表项回调</summary>
        /// <param name="_options" type="Object">请求信息</param>
        /// <param name="_isok" type="boolean">是否删除成功</param>
        _deleteItems: function (_options, _isok) {
            var _item,
                _key = _options.key;
            if (!!_isok) {
                for (var i = 0, l = _options.data.length; i < l; i++) {
                    _item = this._doRemoveItemInCache(_options.data[i]) || null;
                    var _list = this.getListInCache(_key),
                        _index = Util.indexOf(_list, _item);
                    if (_index >= 0) _list.splice(_index, 1);
                }
            }
            var _event = {
                key: _key,
                data: _options.data,
                action: 'delete',
                ext: _options.ext
            };
            this.dispatchEvent('onitemsdelete', _event);
            return _event;
        },
        /// <summary>更新列表项</summary>
        /// <param name="_options" type="Object">配置信息,key 列表标识,id 列表项标识,data 列表项数据信息 ,ext  需要回传的数据信息</param>
        updateItem: function (_options) {
            _options = Util.mix({}, _options);
            _options.onload = this._updateItem.bind(this, _options);
            this.dispatchEvent('doupdateitem', _options);
        },
        /// <summary>更新列表项回调</summary>
        /// <param name="_options" type="Object">请求信息</param>
        /// <param name="_item" type="Object">列表项对象</param>
        _updateItem: function (_options, _item) {
            var _key = _options.key;
            if (!!_item) {
                var _id = _item[this.__key],
                    _old = this._doRemoveItemInCache(_id),
                    _list = this.getListInCache(_key),
                    _index = Util.indexOf(_list, _old);
                if (_index >= 0) _list[_index] = _item;
                _item = this._doSaveItemToCache(_item, _key);
            }
            var _event = {
                key: _key,
                data: _item,
                action: 'update',
                ext: _options.ext
            };
            this.dispatchEvent('onitemupdate', _event);
            return _event;
        }
    }, {});
    var ListCache = WinJS.Class.derive(BaseListCache, function (_options) { BaseListCache.apply(this, arguments) }, {
        _reset: function (_options) {
            BaseListCache.prototype._reset.apply(this, arguments);
            this.batEvent(
                {
                    doloadlist: this._doLoadList.bind(this)
                    , doloaditem: this._doLoadItem.bind(this)
                    , doadditem: this._doAddItem.bind(this)
                    , dodeleteitems: this._doDeleteItems.bind(this)
                    , doupdateitem: this._doUpdateItem.bind(this)
                    , dopullrefresh: this._doPullRefresh.bind(this)
                }
            )
        },
        /// <summary>从服务器端载入列表，子类实现具体逻辑</summary>
        /// <param name="_options" type="Object"> 请求信息 key      列表标识,offset   偏移量,limit    数量,data     请求相关数据,onload   列表项载入回调</param>
        _doLoadList: Util.f,
        /// <summary>从服务器端载入列表项，子类实现具体逻辑</summary>
        /// <param name="_options" type="Object">key      列表标识,id       列表项标识,data     请求相关数据,onload   列表项载入回调</param>
        _doLoadItem: Util.f,
        /// <summary>从服务器上删除列表项，子类实现具体逻辑</summary>
        /// <param name="_options" type="Object">key      列表标识,id       列表项标识,data     请求相关数据,onload   列表项载入回调</param>
        _doAddItem: Util.f,
        /// <summary>从服务器上删除列表项，子类实现具体逻辑</summary>
        /// <param name="_options" type="Object">请求信息 key      列表标识,id       列表项标识,data     请求相关数据,onload   列表项载入回调</param>
        _doDeleteItems: Util.f,
        /// <summary>更新列表项至服务器，子类实现具体逻辑</summary>
        /// <param name="_options" type="Object">请求信息 key      列表标识,id       列表项标识,data     请求相关数据,onload   列表项载入回调</param>
        _doUpdateItem: Util.f,
        /// <summary>从服务器端前向刷新列表，子类实现具体逻辑</summary>
        /// <param name="_options" type="Object">请求信息 key      列表标识,id       列表项标识,data     请求相关数据,onload   列表项载入回调</param>
        _doPullRefresh: Util.f,
    }, {})
    /// <summary>数据缓存模板控件</summary>
    /// <param name="_options" type="Object">配置项
    /// bindDefine            动态绑定数据模板
    /// template              item模板和bindDefine的字段对印
    /// parent                列表的父节点
    /// onbeforeload          列表加载前回调，可以在回调添加加载状态
    /// onitemeventprocess    item节点生成后处理一些相关的事件
    /// onafterload           列表加载完回调
    /// onbeforepullrefresh   向前取数据加载前回调
    /// onafterpullrefresh    向前取数据加载后回调
    /// onbeforeitemsdelete    删除项成功回调
    /// onafteritemsdelete    删除项成功回调
    /// onbeforeitemadd       开始添加项回调
    /// onafteritemadd        添加项回调
    /// onafteritemload       取到具体项回调
    /// onlistempty           列表为空回调
    /// unshiftItemAnimation  头部添加列表项动画回调
    /// appendItemAnimation   列表添加时是动画回调
    /// removeItemAnimation   列表项移除时动画回调
    /// CacheClass Nej.Util.ListCache继承而来的缓存类，实现服务器端数据的各种操作，详见Nej.Util.ListCache需要的功能实现
    ///</param>

    var Module = WinJS.Class.define(function (_options) {
        this.bindDefine = _options.bindDefine || {};
        this._defineItem = WinJS.Binding.define(_options.bindDefine);
        this._itemControl = _options.template.winControl;
        this._parent = _options.parent;
        this.batEvent(_options)
        this._itemCache = [];
        this.bindDataCache = [];
        //this.appendItemAnimation = _options.appendItemAnimation || false;
        //this.removeItemAnimation = _options.removeItemAnimation || false;
        this._cache = new _options.CacheClass({
            id: _options.id,
            onlistload: this._onListLoad.bind(this),
            onitemload: this._onItemLoad.bind(this),
            onitemadd: this._onItemAdd.bind(this),
            onitemsdelete: this._onItemsDelete.bind(this),
            onitemupdate: this._onItemUpdate.bind(this),
            onpullrefresh: this._onPullRefresh.bind(this),
        })
    },
    {
        /// <summary>事件添加</summary>
        batEvent: function (_options) {
            for (var _key in _options)
                if (Util.isTypeOf(_options[_key], 'Function')) {
                    this.addEventListener(_key, _options[_key]);
                }
        },
        /// <summary>获取缓存</summary>
        /// <returns type="Object" />缓存实例</returns>
        getCache: function () {
            return this._cache;
        },
        /// <summary>取列表</summary>
        /// <param name="_options" type="Object">取列表参数，详见listcache getlist方法</param>
        getList: function (_options) {
            this.dispatchEvent('onbeforeload');
            this._cache.getList(_options);
        },

        
        getItem: function (_data) {
            this.dispatchEvent('onbeforeitemget');
            this._cache.getItem(_data);
        },
        
        /// <summary>取列表成功回调</summary>
        _onListLoad: function (event) {
            var _ropt = event.detail;
            var _list = this._cache.getListInCache(_ropt.key);
            if (_list.length == 0) {
                this.dispatchEvent('onlistempty');
            }
            for (var i = _ropt.offset, l = _ropt.limit + _ropt.offset; i < l; i++) {
                this._appendItem(_list[i]);
            }
            this.dispatchEvent('onafterload');
        },
        /// <summary>添加项</summary>
        _appendItem: function (_item) {
            var data = new this._defineItem(_item)
            this.bindDataCache.push(data);
            var item = this._itemControl.render(data).then(function (result) {
                this.dispatchEvent('onitemeventprocess', { elm: result, data: data });
                this.dispatchEvent('appendItemAnimation', result);
                this._parent.appendChild(result);
                this._itemCache.push(result);
            }.bind(this));
        },
        /// <summary>在头部插入项</summary>
        _unshiftItem: function (_item) {
            var data = new this._defineItem(_item)
            this.bindDataCache.unshift(data);
            var item = this._itemControl.render(data).then(function (result) {
                this.dispatchEvent('onitemeventprocess', { elm: result, data: data });
                this.dispatchEvent('unshiftItemAnimation', result);
                this._parent.insertBefore(result, this._parent.firstChild);
                this._itemCache.unshift(result);
            }.bind(this));
        },
        /// <summary>取具体项数据回调</summary>
        _onItemLoad: function (event) {
            var _ropt = event.detail;
            var _item = this._cache.getItemInCache(_ropt.id);
            //this.unshiftItem(_item);
            this.dispatchEvent('onafteritemload', { data: _item });
        },
        /// <summary>添加项数据 key data</summary>
        addItem: function (_data) {
            this.dispatchEvent('onbeforeitemadd');
            this._cache.addItem(_data);
        },
        /// <summary>添加项成功回调</summary>
        _onItemAdd: function (_event) {
            var _ropt = _event.detail;
            var _item = this._cache.getItemInCache(_ropt.data.id);
            this._unshiftItem(_item);
            this.dispatchEvent('onafteritemadd', _item);
        },
        /// <summary>批量删除项</summary>
        /// <param name="_options" type="Object">删除信息 key, data(id数据)</param>
        deleteItems: function (_options) {
            this.dispatchEvent('onbeforeitemsdelete');
            this._cache.deleteItems(_options);
        },
        /// <summary>删除项回调</summary>
        _onItemsDelete: function (_event) {
            var _ropt = _event.detail;
            var _ids = _ropt.data;
            for (var i = 0, l = _ids.length; i < l; i++) {
                var _index = Util.indexOf(this.bindDataCache, function (_item) {
                    return _item.id == _ids[i];
                })
                if (_index != -1) {
                    this.dispatchEvent('appendItemAnimation', this._itemCache[_index]);
                    this._itemCache[_index].parentNode.removeChild(this._itemCache[_index]);
                    this.bindDataCache.splice(_index, 1);
                    this._itemCache.splice(_index, 1);
                }
            }
            this.dispatchEvent('onafteritemsdelete');
        },
        /// <summary>更新项数据，key,data</summary>
        updateItem: function (_data) {
            this.dispatchEvent('onbeforeitemupdate');
            this._cache.updateItem(_data)
        },
        /// <summary>项更新回调</summary>
        _onItemUpdate: function (_event) {
            var _ropt = _event.detail;
            var _data = this._cache.getItemInCache(_ropt.data.id);
            var _index = Util.indexOf(this.bindDataCache, function (_item) {
                return _item.id == _ropt.data.id
            })
            if (_index != -1) {
                for (var _key in this.bindDefine) {
                    this.bindDataCache[_index][_key] = _data[_key];
                }
            }
            this.dispatchEvent('onafteritemupdate');
        },
        /// <summary>向前取数据</summary>
        pullRefresh: function (_options) {
            this.dispatchEvent('onbeforepullrefresh');
            this._cache.pullRefresh(_options);
        },
        /// <summary>向前取数据回调</summary>
        _onPullRefresh: function (_event) {
            var _ropt = _event.detail;
            var _list = this._cache.getListInCache(_ropt.key);
            //debugger;
            for (var i = _ropt.offset, l = _ropt.limit + _ropt.offset; i < l; i++) {
                this._unshiftItem(_list[i]);
            }
            this.dispatchEvent('onafterpullrefresh')
        }
    }, {
    })
    WinJS.Class.mix(Module, WinJS.Utilities.eventMixin);

    WinJS.Namespace.define("Nej.Util", {
        BaseCache: BaseCache,
        BaseListCache: BaseListCache,
        ListCache: ListCache,
        Module: Module
    });
}())