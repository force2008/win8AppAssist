(function () { 
    "use strict";

    var Item = WinJS.Class.define( function ItemContructor(options){
        this._ItemTemplate = options.template;
        this.batEvent(options);
    }, {
        renderAsyc: function (_data) {
            this._ItemTemplate.render(_data).then(function (result) {
                //this.dispatchEvent('onitemeventprocess', { elm: result, data: data });
                //this.dispatchEvent('appendItemAnimation', result);
                this.addEvent(result,_data);
                //this._parent.appendChild(result);
                return WinJS.Promise.wrap(result);
            }.bind(this));
        },
        batEvent:function(_options){
            if (Util.isTypeOf(_options[_key], 'Function')) {
                this.addEventListener(_key, _options[_key]);
            }
        },
        addEvnet:Util.f,
    },{
    })
    
    WinJS.Class.mix(Item, WinJS.Utilities.eventMixin);
    WinJS.Namespace.define("Nej.Util", {
        Item: Item
    });
}())