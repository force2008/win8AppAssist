
(function () {
    "use strict";

    Function.prototype.aop = function (_before, _after) {
        var f = Util.f,
            _after = _after || f,
            _before = _before || f,
            _handler = this;
        return function () {
            var _event = { args: [].slice.call(arguments, 0) };
            _before(_event);
            if (!_event.stopped) {
                _event.value = _handler
                      .apply(this, _event.args);
                _after(_event);
            }
            return _event.value;
        };
    }
    
    /// <summary>解析{for x in b}字符串的前缀</summary>
    /// <param name="_part" type="Array">按空格拆分的值,['for','x','in','b']</param>
    /// <returns type="String" 解析后的前缀值/>
    var _doParsePrefixFor = function (_part) {
        if (_part[2] != 'in')
            throw 'bad for loop statement: ' + _part.join(' ');
        _stack.push(_part[1]);
        return 'var __HASH__' + _part[1] + ' = ' + _part[3] + ',' +
                    _part[1] + ',' + _part[1] + '_count=0;' +
               'if (!!__HASH__' + _part[1] + ')' +
                   'for(var ' + _part[1] + '_key in __HASH__' + _part[1] + '){' +
                        _part[1] + ' = __HASH__' + _part[1] + '[' + _part[1] + '_key];' +
                        'if (!' + _part[1] + '||typeof(' + _part[1] + ')=="function") continue;' +
                        _part[1] + '_count++;';
    };
    var _trim,
        _stack = [], // loop statement stack
        _rspc = /\s+/g,
        _seed = +new Date,
        _skey = (+new Date) + '-',
        _cache = {}; // template cache

    
    /*
     * 解析{forelse}字符串的前缀
     * @return {String} 解析后的前缀值
     */
    /// <summary>解析{forelse}字符串的前缀</summary>
    /// <returns type="String" 解析后的前缀值/>
    var _doParsePrefixForElse = function () {
        var _part = _stack[_stack.length - 1];
        return '}; if(!__HASH__' + _part + '||!' + _part + '_count){';
    };

    /// <summary>解析{/for}字符串的前缀</summary>
    /// <returns type="String" 解析后的前缀值/>
    var _doParsePrefixForEnd = function () {
        _stack.pop();
        return '};';
    };
    /// <summary>解析纯文本内容，不包含需要解析的内容</summary>
    /// <param name="_content" type="String">待解析内容</param>
    /// <param name="_out" type="String">内容输出</param>
    /// <returns type="String" />
    var _doParseText = (function () {
        var _map = { r: /\n|\\|\'/g, '\n': '\\n', '\\': '\\\\', '\'': '\\\'' };
        return function (_content, _out) {
            if (!_content) return;
            _out.push('__OUT.push(\'' + Util.encode(_map, _content) + '\');');
        };
    })();
    /// <summary>解析{list seq as x}或者{list 1..100 as x}字符串的前缀</summary>
    /// <param name="_part" type="String">按空格拆分的值,['list','seq','as','x']</param>
    /// <returns type="String" 解析后的前缀值/>
    var _doParsePrefixList = function (_part) {
        if (_part[2] != 'as')
            throw 'bad for list loop statement: ' + _part.join(' ');
        var _seq = _part[1].split('..');
        if (_seq.length > 1) {
            // {list 1..100 as x}
            return 'for(var ' + _part[3] + ',' + _part[3] + '_index=0,' +
                        _part[3] + '_beg=' + _seq[0] + ',' + _part[3] + '_end=' + _seq[1] + ',' +
                        _part[3] + '_length=parseInt(' + _part[3] + '_end-' + _part[3] + '_beg+1);' +
                        _part[3] + '_index<' + _part[3] + '_length;' + _part[3] + '_index++){' +
                        _part[3] + ' = ' + _part[3] + '_beg+' + _part[3] + '_index;';
        } else {
            // {list seq as x}
            return 'for(var __LIST__' + _part[3] + ' = ' + _part[1] + ',' +
                        _part[3] + ',' + _part[3] + '_index=0,' +
                        _part[3] + '_length=__LIST__' + _part[3] + '.length;' +
                        _part[3] + '_index<' + _part[3] + '_length;' + _part[3] + '_index++){' +
                        _part[3] + ' = __LIST__' + _part[3] + '[' + _part[3] + '_index];';
        }
    };

    /// <summary>解析{macro macroName(arg1,arg2,...argN)}字符串的前缀</summary>
    /// <param name="_part" type="String">按空格拆分的值,['macro','macroName(arg1,arg2,...argN)']</param>
    /// <returns type="String" 解析后的前缀值/>
    var _doParsePrefixMacro = function (_part) {
        if (!_part || !_part.length) return;
        _part.shift(); // remove macro key word
        var _name = _part[0].split('(')[0];
        return 'var ' + _name + ' = function' + _part.join('').replace(_name, '') + '{var __OUT=[];';
    };
    var _config = {
        blk: /^\{(cdata|minify|eval)/i,
        tag: 'forelse|for|list|if|elseif|else|var|macro|break|notrim|trim',
        // {pmin : min param number,
        //  pdft : param default value,
        //  pfix : statement prefix,
        //  sfix : statement suffix}
        def: {
            'if': { pfix: 'if(', sfix: '){', pmin: 1 },
            'else': { pfix: '}else{' },
            'elseif': { pfix: '}else if(', sfix: '){', pdft: 'true' },
            '/if': { pfix: '}' },
            'for': { pfix: _doParsePrefixFor, pmin: 3 },
            'forelse': { pfix: _doParsePrefixForElse },
            '/for': { pfix: _doParsePrefixForEnd },
            'list': { pfix: _doParsePrefixList, pmin: 3 },
            '/list': { pfix: '};' },
            'break': { pfix: 'break;' },
            'var': { pfix: 'var ', sfix: ';' },
            'macro': { pfix: _doParsePrefixMacro },
            '/macro': { pfix: 'return __OUT.join(\'\'); };' },
            'trim': { pfix: function () { _trim = !0; } },
            '/trim': { pfix: function () { _trim = null; } }
        },
        ext: {
            'rand': Util.randNumberString,
            'escape': Util.escape,
            'format': Util.format,
            'seed': function (_prefix) { return (_prefix || '') + _seed; },
            'default': function (_value, _default) { return _value || _default; }
        }
    };
    /// <summary>解析模板为执行函数</summary>
    /// <param name="_content" type="String">模板内容</param>
    /// <returns type="Function" value="模板执行函数"/>
    var _doParseTemplate = (function () {
        var _rtab = /\t/g,
            _rnln = /\n/g,
            _rlne = /\r\n?/g;
        return function (_content) {
            _content = _content.replace(_rlne, '\n').replace(_rtab, '    ');
            var _ftxt = ['if(!__CTX) return \'\';var __OUT=[];with(__CTX){'];
            // defiend used variables
            var _prvend = -1, _length = _content.length;
            var _stmtbeg, _stmtend, _statement,
                _blockrx, _blktmp, _blkend, _blkmrk, _blktxt;
            // search content
            while ((_prvend + 1) < _length) {
                // search statement begin
                _stmtbeg = _prvend;
                _stmtbeg = _content.indexOf("{", _stmtbeg + 1);
                while (_stmtbeg >= 0) {
                    _stmtend = _content.indexOf("}", _stmtbeg + 1);
                    _statement = _content.substring(_stmtbeg, _stmtend);
                    _blockrx = _statement.match(_config.blk);
                    // minify/eval/cdata implementation
                    if (!!_blockrx) {
                        _blktmp = _blockrx[1].length + 1;
                        _blkend = _content.indexOf('}', _stmtbeg + _blktmp);
                        if (_blkend >= 0) {
                            // gen block end marker
                            _blkmrk = _blkend - _stmtbeg - _blktmp <= 0
                                    ? ('{/' + _blockrx[1] + '}')
                                    : _statement.substr(_blktmp + 1);
                            _blktmp = _content.indexOf(_blkmrk, _blkend + 1);
                            // parse block content
                            if (_blktmp >= 0) {
                                _doParseSectionText(_content.substring(_prvend + 1, _stmtbeg), _ftxt);
                                // get block text and parse
                                _blktxt = _content.substring(_blkend + 1, _blktmp);
                                switch (_blockrx[1]) {
                                    case 'cdata': _doParseText(_blktxt, _ftxt); break;
                                    case 'minify': _doParseText(_blktxt.replace(_rnln, ' ').replace(_rspc, ' '), _ftxt); break;
                                    case 'eval': if (!!_blktxt) _ftxt.push('__OUT.push((function(){' + _blktxt + '})());'); break;
                                }
                                _stmtbeg = _prvend = _blktmp + _blkmrk.length - 1;
                            }
                        }
                    } else if (_content.charAt(_stmtbeg - 1) != '$' &&
                             _content.charAt(_stmtbeg - 1) != '\\' &&
                             _statement.substr(_statement.charAt(1) == '/' ? 2 : 1)
                                                         .search(_config.tag) == 0) {
                        // break when result is a statement
                        break;
                    }
                    _stmtbeg = _content.indexOf("{", _stmtbeg + 1);
                }
                if (_stmtbeg < 0) break;
                _stmtend = _content.indexOf("}", _stmtbeg + 1);
                if (_stmtend < 0) break;
                // parse content
                _doParseSectionText(_content.substring(_prvend + 1, _stmtbeg), _ftxt);
                _doParseStatement(_content.substring(_stmtbeg, _stmtend + 1), _ftxt);
                _prvend = _stmtend;
            }
            _doParseSectionText(_content.substring(_prvend + 1), _ftxt);
            _ftxt.push('};return __OUT.join(\'\');');
            return new Function('__CTX', '__MDF', _ftxt.join(''));
        };
    })()

    /// <summary> 解析语句，如{if customer != null && customer.balance > 1000}</summary>
    /// <param name="_content" type="String">待解析语句</param>
    /// <param name="_out" type="String">内容输出</param>
    /// <returns type="String" />
    var _doParseStatement = function (_content, _out) {
        var _part = _content.slice(1, -1).split(_rspc),
            _conf = _config.def[_part[0]];
        if (!_conf) { _doParseSectionText(_content, _out); return; }
        if (!!_conf.pmin && _conf.pmin >= _part.length)
            throw 'Statement needs more parameters:' + _content;
        // parse prefix
        _out.push((!!_conf.pfix &&
                   typeof (_conf.pfix) != 'string')
                   ? _conf.pfix(_part) : (_conf.pfix || ''));
        // parse params and suffix
        if (!!_conf.sfix) {
            if (_part.length <= 1) {
                if (!!_conf.pdft) _out.push(_conf.pdft);
            } else {
                for (var i = 1, l = _part.length; i < l; i++) {
                    if (i > 1) _out.push(' ');
                    _out.push(_part[i]);
                }
            }
            _out.push(_conf.sfix);
        }
    };
    /// <summary>解析内容，内容中可能包含换行</summary>
    /// <param name="_content" type="String">待解析语句</param>
    /// <param name="_out" type="Array">内容输出</param>
    /// <returns type="Void" />
    var _doParseSectionText = function (_content, _out) {
        if (!_content) return;
        var _lines = _content.split('\n');
        if (!_lines || !_lines.length) return;
        for (var i = 0, l = _lines.length, _line; i < l; i++) {
            _line = _lines[i];
            if (!!_trim) {
                _line = _line.trim();
                if (!_line) continue;
            }
            _doParseSectionTextLine(_line, _out);
            if (!!_trim && i < l - 1) _out.push('__OUT.push(\'\\n\');');
        }
    };

    /// <summary>解析内容，内容中可能包含${a}或者${%a%}取值语句</summary>
    /// <param name="_content" type="String">待解析语句</param>
    /// <param name="_out" type="Array">内容输出</param>
    /// <returns type="Void" />
    var _doParseSectionTextLine = (function () {
        var _raor = /\|\|/g,
            _rvor = /#@@#/g;
        return function (_content, _out) {
            // defined used variable
            var _prvmrkend = '}', _prvexpend = -1,
                _length = _content.length,
                _begin, _end, _begexp, _endexp, _exparr;
            while ((_prvexpend + _prvmrkend.length) < _length) {
                _begin = '${'; _end = '}';
                _begexp = _content.indexOf(_begin, _prvexpend + _prvmrkend.length);
                if (_begexp < 0) break;
                // parse ${% customer.firstName %} syntax
                if (_content.charAt(_begexp + 2) == '%') {
                    _begin = '${%'; _end = '%}';
                }
                _endexp = _content.indexOf(_end, _begexp + _begin.length);
                if (_endexp < 0) break;
                _doParseText(_content.substring(_prvexpend + _prvmrkend.length, _begexp), _out);
                // parse expression: 'firstName|default:"John Doe"|capitalize'.split('|')
                _exparr = _content.substring(_begexp + _begin.length, _endexp).replace(_raor, '#@@#').split('|');
                for (var i = 0, l = _exparr.length; i < l; _exparr[i] = _exparr[i].replace(_rvor, '||'), i++);
                _out.push('__OUT.push('); _doParseExpression(_exparr, _out); _out.push(');');
                _prvmrkend = _end; _prvexpend = _endexp;
            }
            _doParseText(_content.substring(_prvexpend + _prvmrkend.length), _out);
        };
    })();
    /// <summary>解析表达式，如['firstName','default:"John Doe"','capitalize']</summary>
    /// <param name="_exps" type="Array">表达式内容</param>
    /// <param name="_out" type="Array">内容输出</param>
    /// <returns type="Void" />
    var _doParseExpression = function (_exps, _out) {
        // foo|a:x|b:y1,y2|c:z1,z2 -> c(b(a(foo,x),y1,y2),z1,z2)
        if (!_exps || !_exps.length) return;
        if (_exps.length == 1) {
            _out.push(_exps.pop()); return;
        }
        var _exp = _exps.pop().split(':');
        _out.push('__MDF[\'' + _exp.shift() + '\'](');
        _doParseExpression(_exps, _out);
        if (_exp.length > 0)
            _out.push(',' + _exp.join(':'));
        _out.push(')');
    };
    /// <summary>JST模板封装</summary>
    WinJS.Namespace.define("Util.JST", {
        cache: {},
        tcache: {},
        _doAddTemplate: function (_node) {
            var _type = _node.name.toLowerCase();
            switch (_type) {
                case 'jst':
                    this.addHtmlTemplate(_node, !0);
                    return;
                case 'txt':
                    this.addTextTemplate(_node.id, _node.value || '');
                    return;
                case 'ntp':
                    this.addNodeTemplate(_node.value || '', _node.id);
                    return;

            }
        },
        parseTemplate: function (_element) {
            _element = Util.Element.getElement(_element);
            if (!!_element) {
                var _list = _element.tagName == 'TEXTAREA' ? [_element]
                          : _element.getElementsByTagName('textarea');
                for (var i = 0, l = _list.length; i < l; i++)
                    this._doAddTemplate(_list[i])
                _element.parentNode.removeChild(_element);
            }
        },
        /// <summary>添加JST模板，JST模板可以是节点的值</summary>
        /// <param name="name" type="_content">JST模板内容或者节点ID</param>
        /// <param name="name" type="_keep">是否保留节点</param>
        /// <returns type="String" value="JST模板在缓存中的序列号"/>
        addHtmlTemplate: function (_content, _keep) {
            if (!_content) return '';
            var _sn, _element = Util.Element.getElement(_content);
            if (!!_element) {
                _sn = _element.id;
                _content = _element.value || _element.innerText;
                if (!_keep) {
                    _element.parentNode.removeChild(_element)
                }
            }
            _sn = _sn || ('ck_' + util.randNumberString());
            this.tcache[_sn] = _content;
            return _sn;
        },
        /// <summary>根据模板的序列号合并模板数据</summary>
        /// <param name="_sn" type="String">模板序列号</param>
        /// <param name="_data" type="String">模板数据</param>
        /// <param name="_extend" type="String">扩展接口</param>
        /// <returns type="String" value ="合并数据后的内容"/>
        getHtmlTemplate: (function () {
            var _fcache = {};
            return function (_sn, _data, _extend) {
                try {
                    _data = _data || {};
                    if (!_fcache[_sn] && !this.tcache[_sn])
                        return '';
                    if (!_fcache[_sn]) {
                        _fcache[_sn] = _doParseTemplate(this.tcache[_sn]);
                        delete this.tcache[_sn];
                    }
                    _data.defined = function (_key) {
                        return _data[_key] != null;
                    };
                    if (!!_extend)
                        for (var p in _config.ext)
                            _extend[p] = _config.ext[p];
                    return _fcache[_sn](_data, _extend || _config.ext);
                } catch (ex) { return ex.message || ''; }
            };
        })(),
        /// <summary>将html模板成生并添加到_parent节点里</summary>
        /// <param name="_parent" type="Element">容器节点</param>
        /// <param name="_sn" type="String">模板id</param>
        /// <param name="_data" type="Object">模板数据</param>
        /// <param name="_extent" type="Object">扩展接口</param>
        renderHtmlTemplate: function (_parent, _sn, _data, _extend) {
            _parent = Util.Element.getElement(_parent);
            if (!_parent) return;
            _parent.innerHTML = this.getHtmlTemplate(
                                   _sn, _data, _extend);
        },
        /// <summary>text模版添加，静态的html字符串添加</summary>
        /// <param name="_key" type="String">key</param>
        /// <param name="_value" type="String">html字符器或普通字符器</param>
        addTextTemplate: function (_key, _value) {
            this.cache[_key] = _value;
        },
        /// <summary>获取text模板</summary>
        /// <param name="_key" type="String">key</param>
        getTextTemplate: function (_key) {
            return this.cache[_key];
        },
        /// <summary>取node template 结构</summary>
        /// <param name="_key" type="String">id</param>
        getNodeTemplate: function (_key) {
            if (!_key) return null;
            _key = _skey + _key;
            var _value = this.getTextTemplate(_key);
            if (!_value) return null;
            if (Util.isTypeOf(_value,'String')) {
                _value = Util.Element.html2node(_value);
                this.addTextTemplate(_key, _value);
            }
            return _value.cloneNode(!0);
        },
        /// <summary>添加节点模板</summary>
        /// <param name="_element" type="String">节点</param>
        /// <param name="_key" type="String">模板节点id</param>
        addNodeTemplate: function (_element, _key) {
            _key = _key || Util.randNumberString();
            _element = Util.Element.getElement(_element) || _element;
            this.addTextTemplate(_skey + _key, _element);
            return _key;
        },
        /// <summary>取ITEM模板列表</summary>
        /// <param name="_list" type="Array">数据列表</param>
        /// <param name="_item" type="Class">列表项构造函数</param>
        /// <param name="_options" type="Object">可选配置参数，已处理参数列表如下，其他参数参见item指定的构造函数的配置参数</param>
        ///                                             offset   [Number]  - 起始指针【包含】，默认0
        ///                                             limit    [Number]  - 分配数据长度或者数量，默认为列表长度
        getItemTemplate : (function(){
            var _doFilter = function(_value,_key){
                return _key=='offset'||_key=='limit';
            };
            return function(_list,_item,_options){
                var _arr = [];
                if (!_list||!_list.length||!_item)
                    return _arr;
                _options = _options||_o;
                var _len = _list.length,
                    _beg = parseInt(_options.offset)||0,
                    _end = Math.min(_len,_beg+(
                           parseInt(_options.limit)||_len)),
                    _opt = {total:_list.length,range:[_beg,_end]};
                Util.mix(_opt, _options, _doFilter);
                for(var i=_beg,_instance;i<_end;i++){
                    _opt.index = i;
                    _opt.data = _list[i];
                    _instance = _item.allocate(_opt);
                    var _id = _instance.getId();
                    _cache[_id] = _instance;
                    _instance.recycle = 
                    _instance.recycle.aop(
                        function(_event){
                            delete _cache[_id];
                            delete _instance.recycle;
                        });
                    _arr.push(_instance);
                }
                return _arr;
            };
        })(),
    })
})()