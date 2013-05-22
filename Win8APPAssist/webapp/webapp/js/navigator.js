﻿(function () {
    "use strict";

    var appView = Windows.UI.ViewManagement.ApplicationView;
    var nav = WinJS.Navigation;
    var util = Util.Element;
    WinJS.Namespace.define("Application", {
        PageControlNavigator: WinJS.Class.define(
            // Define the constructor function for the PageControlNavigator.
            function PageControlNavigator(element, options) {
                this._element = element || document.createElement("div");
                this._element.appendChild(this._createPageElement());

                this.home = options.home;
                this._lastViewstate = appView.value;

                nav.onnavigated = this._navigated.bind(this);
                window.onresize = this._resized.bind(this);

                document.body.onkeyup = this._keyupHandler.bind(this);
                document.body.onkeypress = this._keypressHandler.bind(this);
                document.body.onmspointerup = this._mspointerupHandler.bind(this);

                Application.navigator = this;
            }, {
                home: "",
                /// <field domElement="true" />
                _element: null,
                _lastNavigationPromise: WinJS.Promise.as(),
                _lastViewstate: 0,

                // This is the currently loaded Page object.
                pageControl: {
                    get: function () { return this.pageElement && this.pageElement.winControl; }
                },

                // This is the root element of the current page.
                pageElement: {
                    get: function () { return this._element.firstElementChild; }
                },

                // Creates a container for a new page to be loaded into.
                _createPageElement: function () {
                    var element = document.createElement("div");
                    element.style.width = "100%";
                    element.style.height = "100%";
                    return element;
                },

                // Retrieves a list of animation elements for the current page.
                // If the page does not define a list, animate the entire page.
                _getAnimationElements: function () {
                    if (this.pageControl && this.pageControl.getAnimationElements) {
                        return this.pageControl.getAnimationElements();
                    }
                    return this.pageElement;
                },

                // Navigates back whenever the backspace key is pressed and
                // not captured by an input field.
                _keypressHandler: function (args) {
                    if (args.key === "Backspace") {
                        nav.back();
                    }
                },

                // Navigates back or forward when alt + left or alt + right
                // key combinations are pressed.
                _keyupHandler: function (args) {
                    if ((args.key === "Left" && args.altKey) || (args.key === "BrowserBack")) {
                        nav.back();
                    } else if ((args.key === "Right" && args.altKey) || (args.key === "BrowserForward")) {
                        nav.forward();
                    }
                },

                // This function responds to clicks to enable navigation using
                // back and forward mouse buttons.
                _mspointerupHandler: function (args) {
                    if (args.button === 3) {
                        nav.back();
                    } else if (args.button === 4) {
                        nav.forward();
                    }
                },

                // Responds to navigation by adding new pages to the DOM.
                _navigated: function (args) {
                    var that = this;
                    var newElement = this._createPageElement();
                    var parentedComplete;
                    var parented = new WinJS.Promise(function (c) { parentedComplete = c; });
                    var options = {};
                    this._lastNavigationPromise.cancel();

                    this._lastNavigationPromise = WinJS.Promise.timeout().then(function () {
                        var locationObject = Util.parseURL(args.detail.location);
                        Util.mix(options, Util.query2object(locationObject.search));
                        Util.mix(options, Util.query2object(locationObject.hash));
                        return WinJS.UI.Pages.render(locationObject.pathname, newElement, Util.mix(args.detail.state, options), parented);
                    }).then(function parentElement(control) {
                        var oldElement = this.pageElement;
                        if (oldElement.winControl && oldElement.winControl.unload) {
                            oldElement.winControl.unload();
                        }
                        this._element.appendChild(newElement);
                        this._element.removeChild(oldElement);
                        that._resetAnchor();
                        oldElement.innerText = "";
                        this._updateBackButton();
                        parentedComplete();
                        WinJS.UI.Animation.enterPage(this._getAnimationElements()).done();
                    }.bind(this));

                    args.detail.setPromise(this._lastNavigationPromise);
                },
                /// <summary>重置节点下的链接行为</summary>
                /// <summary>修改链接的行为，不以shceme(http,https,...)起始的链接,链接到系统内部的页面(/pages下面对应的页面)</summary>
                /// <param name="element" type="HTMLElement">根节点</param>
                _resetAnchor: (function () {
                    var regx = /ms-appx:\/\//;
                    var eventHandler = function (event) {
                        var a = util.findAncestor(event.target, function (node) {
                            return node.tagName == "A";
                        });
                        if (!a || !a.href || /^javascript:/.test(a.href) || a.href == "#" || a.target || a.getAttribute('navable') === 'false')
                            return;

                        if (regx.test(a.href)) {
                            event.preventDefault();
                            nav.navigate(a.pathname + a.search + a.hash);
                        }
                    };
                    return function () {
                        this.pageElement.addEventListener("click", eventHandler, false);
                    };
                })(),
                // Responds to resize events and call the updateLayout function
                // on the currently loaded page.
                _resized: function (args) {
                    if (this.pageControl && this.pageControl.updateLayout) {
                        this.pageControl.updateLayout.call(this.pageControl, this.pageElement, appView.value, this._lastViewstate);
                    }
                    this._lastViewstate = appView.value;
                },

                // Updates the back button state. Called after navigation has
                // completed.
                _updateBackButton: function () {
                    var backButton = this.pageElement.querySelector("header[role=banner] .win-backbutton");
                    if (backButton) {
                        backButton.onclick = function () { nav.back(); };

                        if (nav.canGoBack) {
                            backButton.removeAttribute("disabled");
                        } else {
                            backButton.setAttribute("disabled", "disabled");
                        }
                    }
                },
            }
        )
    });
})();
