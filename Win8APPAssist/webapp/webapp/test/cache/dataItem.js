(function () {
    "use strict";

    var DataItem = WinJS.Class.derive(Nej.Util.Item, function DataItemConstructor(_options) {
        Nej.Util.Item.apply(this, arguments);
    }, {
        addEvent: function (_elm,_data) {
            _elm.addEventListener("contextmenu", this._toggleSelected.bind(this, _data, _elm), false);
            Util.Element.addEvent(_elm, 'swipeDown', this._toggleSelected.bind(this, _data, _elm), false);
            _elm.addEventListener("MSPointerUp", this.onPointerUp, false);
            _elm.addEventListener("MSPointerDown", this.onPointerDown, false);
        },
        _toggleSelected: function (_data, _elm, _event) {
            console.log(this._data.title);
            if (WinJS.Utilities.hasClass(_elm, "select")) {
                WinJS.Utilities.removeClass(_elm, "select");
            }
            else {
                WinJS.Utilities.addClass(_elm, "select");
                
            }
            this.dispatchEvent('swipeDown', _data.id)
            
        },
        onPointerUp: function (evt) {
            WinJS.UI.Animation.pointerUp(evt.srcElement);
        },
        onPointerDown: function (evt) {
            WinJS.UI.Animation.pointerDown(evt.srcElement);
        }
    }, {})

    
    WinJS.Namespace.define("Test.Cache", {
        DataItem: DataItem
    });
}())