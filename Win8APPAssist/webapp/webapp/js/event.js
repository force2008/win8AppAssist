/// <summary>UI控件基类</summary>
(function(){
var Event = WinJS.Class.define(
     function EventConstructor (options) {
        /// <summary>缓存事件，和dom事件</summary>
        this.events = {};
        this.events_dom = {};
    },
    {
        /// <summary>重置控件</summary>
        _reset: function (_options) {
            this.batEvent(_options);
        },
        /// <summary>销毁控件</summary>
        _destroy: function () {
            this.clearEvent();
            this._doClearDomEvent();
        },
        /// <summary>初使化dom事件</summary>
        _doInitDomEvent: function (_list) {
            var _seed = +new Date;
            for (var i = 0, l = _list.length; i < l; i++) {
                if (_list[i].length < 3) return;
                this.events_dom['de-' + (_seed++)] = _args;
                try {
                    _args[0].addEventListener(_args[1], _agrs[2]);
                } catch (e) { }
            }
        },
        /// <summary>清空dom事件</summary>
        _doClearDomEvent: function () {
            for (x in this.events_dom) {
                this.events_dom[x][0].removeEventListener(this.events_dom[x][1], this.events_dom[x][2])
                delete this.events_dom[x]
            }
        },
        /// <summary>实例回收控件</summary>
        recycle: function () {
            this.constructor.recycle(this);
        },
        /// <summary>是否已添加事件</summary>
        /// <param name="_type" type="String">事件key</param>
        hasEvent: function (_type) {
            var _event = this.events[
                     _type.toLowerCase()];
            return !!_event && _event !== Util.f;
        },
        /// <summary>添加事件</summary>
        /// <param name="_type" type="String">事件key</param>
        /// <param name="_event" type="Function">回调函数</param>
        addEvent: function (_type, _event) {
            this.setEvent.apply(this, arguments);
            return this;
        },
        /// <summary>设置事件</summary>
        /// <param name="_type" type="String">事件key</param>
        /// <param name="_event" type="Function">回调函数</param>
        setEvent: function (_type, _event) {
            if (!!_type && Util.isTypeOf(_event, 'Function'))
                this.events[_type.toLowerCase()] = _event;
            return this;
        },
        /// <summary>批量添加事件</summary>
        /// <param name="_event" type="Map">事件map对象</param>
        batEvent: function (_events) {
            for (var event in _events)
                this.setEvent(event, _events[event])
        },
        /// <summary>清空事件</summary>
        /// <param name="_type" type="String">事件key</param>
        clearEvent: function (_type) {
            var _type = (_type || '').toLowerCase();
            if (!!_type) {
                delete this.events[_type];
            } else {
                for (var event in this.events)
                    this.clearEvent(event)
            }
            return this;
        },
        /// <summary>追加事件</summary>
        /// <param name="_type" type="String">事件key</param>
        /// <param name="_event" type="Function">回调函数</param>
        pushEvent: function (_type, _event) {
            if (!_type && !Util.isTypeOf(_event, 'Function'))
                return
            _type = _type.toLowerCase();
            var _events = this.events[_type]
            if (!_events) {
                this.events[_type] = _event;
                return this;
            }
            if (!Util.isTypeOf(_events, 'Array')) {
                this.events[_type] = [_events];
            }
            this.events[_type].push(_event);
            return this;
        },
        /// <summary>分发事件</summary>
        /// <param name="_type" type="String">事件key</param>
        dispatchEvent: function (_type) {
            var _event = this.events[(_type || '').toLowerCase()];
            if (!_event) return this;
            var _args = Util.r.slice.call(arguments, 1);
            if (!Util.isTypeOf(_event,'Array'))
                return _event.apply(this, _args);
            for (var evt in _event) {
                try {
                    _event[evt].apply(this, _args)
                } catch (e) { }
            }
        },
    },
    {
        /// <summary>静态实例化对象方法</summary>
        allocate: function (_options) {
            _options = _options || {}
            var _instance = !!this.pool
                        && this.pool.shift()
                        || new this(_options);
            // reset instance, flag first
            _instance._xxxx = !0;
            _instance._reset(_options);
            return _instance;
        },
        /// <summary>静态回收对象实例</summary>
        /// <param name="_instance" type="Object">实例</param>
        recycle: function (_instance) {
            if (!_instance) return null;
            if (!Util.isTypeOf(_instance, 'Array')) {
                if (!_instance._xxxx)
                    return null;
                if (!(_instance instanceof this)) {
                    // use constructor recycle instance
                    var _class = _instance.constructor;
                    if (!!_class.recycle)
                        _class.recycle(_instance);
                    return null;
                }
                // delete singleton instance
                if (_instance == this.instance)
                    delete this.instance;
                // recycle instance
                if (!this.pool)
                    this.pool = [];
                // void re-recycle, flag first
                _instance._xxxx = !1;
                _instance._destroy();
                this.pool.push(_instance);
                return null;
            }
            for (var l = _instance.length - 1; l >= 0; l--) {
                this.recycle(_instance[l]);
                _instance.splice(l, 1);
            }
        },
        /// <summary>静态取实例,静态的getInstance生成，用静态的recycle回收才能正常调用到下一个的getInstance</summary>
        /// <param name="_options" type="Object">实例参数</param>
        /// <returns type="Object" >实例对象</returns>
        getInstance: function (_options) {
            _options = _options || {};
            if (!this.instance) {
                this.instance = new this(_options);
                this.instance._xxxx = !0;
                this.instance._reset(_options);
            }
            return this.instance;
        },
    });
/// <summary>UI控件基类</summary>
var Abstract = WinJS.Class.derive(Event,
    function AbstractConstructor(options) {
        Event.apply(this, arguments);
        //this.super(arguments);
        this._initXGui();
        this._initNode();
    },
    {
        /// <summary>重置控件</summary>
        _reset: function (_options) {
            Event.prototype._reset.call(this, _options);
            //this.super('_reset', _options);
            this._doInitClass(_options.clazz||'');
            this.appendTo(_options.parent);
        },
        /// <summary>销毁控件</summary>
        _destroy: function () {
            Event.prototype._destroy.apply(this);
            //this.super('_destroy');
            // clear parent
            this._doDelParentClass();
            delete this._parent;
            // clear body
            Util.Element.removeByEC(this._body);
            WinJS.Utilities.removeClass(this._body, this._class)
            delete this._class;
        },
        /// <summary>继承类实现_initXGui</summary>
        _initXGui: Util.f,
        /// <summary>节点方法</summary>
        _initNode : function(){
            if (!this._seed_html)
                this._initNodeTemplate();
            this._body = Util.JST.getNodeTemplate(this._seed_html);
            if (!this._body)
                this._body = Util.createElement('div', this._seed_css);
            WinJS.Utilities.addClass(this._body, this._seed_css||'');
        },
        /// <summary>继承类实现_initNodeTemplate</summary>
        _initNodeTemplate: Util.f,
        /// <summary>控件添加样式</summary>
        /// <param name="_clazz" type="String">样式名</param>
        _doInitClass : function(_clazz){
            this._class = _clazz || '';
            WinJS.Utilities.addClass(this._body, this._class)
        },
        /// <summary>父节点添加样式</summary>
        _doAddParentClass : function(){
            if (!this._seed_css) return;
            WinJS.Utilities.addClass(this._parent,
                              this._seed_css+'-parent');
        },
        /// <summary>删除父节点样式</summary>
        _doDelParentClass : function(){
            if (!this._seed_css) return;
            WinJS.Utilities.removeClass(this._parent,
                              this._seed_css+'-parent');
        },
        /// <summary>取控件html结构</summary>
        getBody : function(){
            return this._body;
        },
        /// <summary>添加到父节点</summary>
        /// <param name="_parent" type="String">父节点</param>
        appendTo: function (_parent) {
            if (!this._body) return;
            this._doDelParentClass();
            if (Util.isTypeOf(_parent,'Function')){
                this._parent = _parent(this._body);
            }else{
                this._parent = Util.Element.getElement(_parent);
                if (!!this._parent)
                    this._parent.appendChild(this._body);
            }
            this._doAddParentClass();
        },
        /// <summary>显示控件</summary>
        show : function(){
            if (!this._parent||!this._body||
                 this._body.parentNode==this._parent)
                return;
            this._parent.appendChild(this._body);
        },
        /// <summary>隐藏控件</summary>
        hide : function(){
            Util.Element.removeByEC(this._body);
        }
    },
    {
        /// <summary>从父类继承三个静态方法</summary>
        allocate: Event.allocate,
        recycle: Event.recycle,
        getInstance: Event.getInstance
    });
var Item = WinJS.Class.derive(Abstract,
    function ItemConstructor(options) {
        var _seed = +new Date;
        this._id = 'itm-' + (++_seed);
        Abstract.apply(this, arguments);
        //this.super(arguments)
    },
    {
        /// <summary>重置项</summary>
        _reset: function (_options) {
            Abstract.prototype._reset.apply(this, arguments);
            //this.super('_reset', _options);
            this._data = _options.data;
            this._index = _options.index;
            this._total = _options.total;
            this._range = _options.range;
            this._doRefresh(this._data);
        },
        /// <summary>销毁控件</summary>
        _destroy: function () {
            Abstract.prototype._destroy.apply(this, arguments);
            delete this._data;
            delete this._index;
            delete this._total;
            delete this._range;
        },
        /// <summary>刷新控件，由继承类实现</summary>
        _doRefresh: Util.f,
        /// <summary>取id</summary>
        getId: function () {
            return this._id
        },
        
    },
    {
        /// <summary>从父类继承三个静态方法</summary>
        allocate: Abstract.allocate,
        recycle: Abstract.recycle,
        getInstance: Abstract.getInstance
    });
WinJS.Namespace.define('Nej.Util', {
    Event: Event,
    Abstract:Abstract,
    Item: Item
})
}())