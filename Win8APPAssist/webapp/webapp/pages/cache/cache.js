(function () {
    "use strict";
    var ListCache = Nej.Util.ListCache;
    var CacheTest = WinJS.Class.derive(ListCache, function (_options) { ListCache.apply(this,arguments)}, {
        _doLoadList: function (_event) {
            var _options = _event.detail;
            var _id     = _options.id;
            var _key    = _options.key;
            var _rkey   = _options.rkey;
            var _onload = _options.onload;
            setTimeout( function () {
                var _list = [{ id: Util.randNumberString(5), title: "The Smoothest", text: "Maybe the smoothest ice cream ever." },
    { id: Util.randNumberString(5), title: "What a great flavor!", text: "Although the texture was a bit lacking, this one has the best flavor I have tasted!" },
    { id: Util.randNumberString(5), title: "Didn't like the 'choco bits'", text: "The little bits of chocolate just weren't working for me." },
    { id: Util.randNumberString(5), title: "Loved the peanut butter", text: "The peanut butter! I loved this ice cream. Did I mention I loved this ice cream! However, The little bits of chocolate just weren't working for me. I will try it again another time to give my final opinion. I will write back when I have finished the next carton." },
    { id: Util.randNumberString(5), title: "Wish there was more sugar", text: "This wasn't sweet enough for me. I will have to try your other flavors, but maybe this is too healthy for me. Who knows?" },
    { id: Util.randNumberString(5), title: "Texture was perfect", text: "This was the smoothest ice cream I have ever had" },
    { id: Util.randNumberString(5), title: "Kept wishing there was more", text: "When I got to the end of each carton I kept wishing there was more ice cream. It was delicious! The best part of this delicious snack was the strawberries!" },
    { id: Util.randNumberString(5), title: "aaaThe Smoothest", text: "Maybe the smoothest ice cream ever." },
                { id: Util.randNumberString(5), title: "bbbThe Smoothest", text: "Maybe the smoothest ice cream ever." },
                { id: Util.randNumberString(5), title: "ccccThe Smoothest", text: "Maybe the smoothest ice cream ever." }]
                _onload(_list);
            }, 500);
            //_j._$request('http://123.163.com:3000/xhr/getLog',{
            //       type:'json',
            //       method:'POST',
            //       data:{id:_id,key:_key},
            //       timeout:1000,
            //       onload:_onload._$bind(this),
            //       onerror:function(_error){}
            //  }
            //);
        },
        _doLoadItem: function (_event) {
            var _options = _event.detail;
            var _id = _options.id;
            var _key = _options.key;
            var _rkey = _options.rkey;
            var _onload = _options.onload;
            setTimeout(function () {
                _onload({ id: _id, title: "force2008", text: "Maybe the smoothest ice cream ever." });
            }, 500);
        },
        _doAddItem: function (_event) {
            var _options = _event.detail;
            var _id = _options.id;
            var _key = _options.key;
            var _rkey = _options.rkey;
            var _onload = _options.onload;
            setTimeout(function () {
                _onload({ id: Util.randNumberString(5), title: "force2009", text: "Maybe the smoothest ice cream ever." });
            }, 500);
        },
        _doUpdateItem: function (_event) {
            var _options = _event.detail;
            var _onload = _options.onload;
            var _data = _options.data;
            setTimeout(function () {
                _onload(_data);
            }, 500);
        },
        _doPullRefresh: function (_event) {
            var _options = _event.detail;
            var _id = _options.id;
            var _key = _options.key;
            var _rkey = _options.rkey;
            var _onload = _options.onload;
            setTimeout(function () {
                var _list = [{ id: Util.randNumberString(5), title: Util.randNumberString(5)+"The Smoothest", text: "Maybe the smoothest ice cream ever." },
                            { id: Util.randNumberString(5), title: Util.randNumberString(5)+"What a great flavor!", text: "Although the texture was a bit lacking, this one has the best flavor I have tasted!" },
                            { id: Util.randNumberString(5), title: Util.randNumberString(5)+"Didn't like the 'choco bits'", text: "The little bits of chocolate just weren't working for me." },
                            { id: Util.randNumberString(5), title: Util.randNumberString(5)+"Loved the peanut butter", text: "The peanut butter! I loved this ice cream. Did I mention I loved this ice cream! However, The little bits of chocolate just weren't working for me. I will try it again another time to give my final opinion. I will write back when I have finished the next carton." },
                            { id: Util.randNumberString(5), title: Util.randNumberString(5)+"Wish there was more sugar", text: "This wasn't sweet enough for me. I will have to try your other flavors, but maybe this is too healthy for me. Who knows?" },
                            { id: Util.randNumberString(5), title: Util.randNumberString(5)+"Texture was perfect", text: "This was the smoothest ice cream I have ever had" },
                            { id: Util.randNumberString(5), title: Util.randNumberString(5)+"Kept wishing there was more", text: "When I got to the end of each carton I kept wishing there was more ice cream. It was delicious! The best part of this delicious snack was the strawberries!" },
                            { id: Util.randNumberString(5), title: Util.randNumberString(5)+"The Smoothest", text: "Maybe the smoothest ice cream ever." }]
                                        _onload(_list);
                                    }, 500);
        },
        
        _doDeleteItems: function (_event) {
            var _options = _event.detail;
            var _id = _options.id;
            var _key = _options.key;
            var _rkey = _options.rkey;
            var _onload = _options.onload;
            _onload(_options,true)
        }
    }, {
    })

    WinJS.Namespace.define('Nej.Test', {
        CacheTest: CacheTest
    })
})();
