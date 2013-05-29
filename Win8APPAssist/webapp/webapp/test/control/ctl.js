(function () {
    "use strict"

    var controlClass = WinJS.Class.define(
            function Control_ctor(element, options) {
                this.element = element || document.createElement("div");
                this.element.winControl = this;

                // Set option defaults
                this._blink = false;

                // Set user-defined options,循环options变量高到this上
                WinJS.UI.setOptions(this, options);

                element.textContent = "Hello, World!"
            },
            {
                _blinking: 0,
                _blinkCount: 0,
                blink: {
                    get: function () {
                        return this._blink;
                    },

                    set: function (value) {
                        if (this._blinking) {
                            clearInterval(this._blinking);
                            this._blinking = 0;
                        }
                        this._blink = value;
                        if (this._blink) {
                            this._blinking = setInterval(this.doBlink.bind(this), 500);
                        }

                    }
                },

                doBlink: function () {
                    if (this.element.style.display === "none") {
                        this.element.style.display = "block";
                    } else {
                        this.element.style.display = "none";
                    }
                    this._blinkCount++;
                    this.dispatchEvent("blink", {
                        count: this._blinkCount
                    });
                },
            });
    
    WinJS.Namespace.define("Contoso.UI", {
        HelloWorld: controlClass
    });
    WinJS.Class.mix(Contoso.UI.HelloWorld,
        WinJS.Utilities.createEventProperties("blink"),
        WinJS.UI.DOMEventMixin);

}())