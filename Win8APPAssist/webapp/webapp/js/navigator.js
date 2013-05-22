(function () {
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

                nav.onbeforenavigate = this._beforeNavigate.bind(this);
                nav.onnavigated = this._navigated.bind(this);
                window.onresize = this._resized.bind(this);

                document.body.onkeyup = this._keyupHandler.bind(this);
                document.body.onkeypress = this._keypressHandler.bind(this);
                document.body.onmspointerup = this._mspointerupHandler.bind(this);

                Application.navigator = this;
                this._cachePath = [];
                this._cache = {};
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
                // 进入页面前执行的逻辑
                _beforeNavigate: function (args) {
                    // 如果当前页面的beforeUnload返回false，阻止离开当前页
                    if (this.pageControl && this.pageControl.beforeUnload && this.pageControl.beforeUnload() == false) {
                        args.preventDefault();
                        return;
                    }
                    
                    // 如果导航的目的地址等于当前地址，刷新当前页
                    if (this.location && this.location.href === args.detail.location) {
                        this.pageControl.referer = this.location.href;
                        this.pageControl && this.pageControl.refresh && this.pageControl.refresh();
                        args.preventDefault();
                    }
                },
                // Responds to navigation by adding new pages to the DOM.
                _navigated: function (args) {
                    var that = this;
                    var referer;
                    var newElement;
                    var parentedComplete;
                    var parented = new WinJS.Promise(function (c) {
                        parentedComplete = c;
                    });
                    var options = {};
                    var state = args.detail.state;
                    var result;

                    this._lastNavigationPromise.cancel();
                    this._lastNavigationPromise = WinJS.Promise.timeout().then(function () {
                        var oldLocation = that.location;
                        referer = oldLocation && oldLocation.href;
                        var locationObject = Util.parseURL(args.detail.location);
                        Util.mix(options, Util.query2object(locationObject.search));
                        Util.mix(options, Util.query2object(locationObject.hash));
                        that.location = locationObject;

                        // 卸载原模块
                        var oldElement = that.pageElement;
                        if (oldElement.winControl && oldElement.winControl.unload) {
                            oldElement.winControl.unload();
                        }

                        if (oldLocation && oldLocation.pathname == that.location.pathname) {
                            // 目的地址的pathname与原地址的pathname相等时
                            return 0;
                        } else if (newElement = that._cache[that.location.pathname]) {
                            // 模块在缓存中
                            return 1;
                        } else {
                            newElement = that._createPageElement();
                            return WinJS.UI.Pages.render(that.location.pathname, newElement, Util.mix(state, options), parented);
                        }

                    }).then(function parentElement(x) {
                        result = x;
                        if (result) {
                            // 如果页面自定义了退出动画，使用页面退出动画，否则使用默认退出动画
                            var exitPageAsync = that.pageControl && that.pageControl.exitPageAsync && that.pageControl.exitPageAsync.bind(that.pageControl) || that._exitPageAsync && that._exitPageAsync.bind(that);
                            return exitPageAsync&&exitPageAsync(that._getAnimationElements());
                        }
                    }).then(function () {
                        if (result) {
                            //节点个数在达到2000以上时移除节点耗时很大，在移除前把节点隐藏，可以大大缩减切换时间
                            var node = that._element.lastChild;
                            node.style.display = 'none';
                            setTimeout(function () { node.style.display = '' }, 10);
                            Util.Element.empty(that._element);
                            // 容器节点上包含页面名称类名，用于控制背景样式
                            that._element.className = 'index p-' + that.name;
                            that._element.appendChild(newElement);
                            // 如果页面自定义了进入动画，使用页面进入动画，否则使用默认进入动画
                            var enterPageAsync = that.pageControl && that.pageControl.enterPageAsync && that.pageControl.enterPageAsync.bind(that.pageControl) || that._enterPageAsync && that._enterPageAsync.bind(that);
                            enterPageAsync(that._getAnimationElements());
                        }
                        that.pageControl.referer = referer;
                        if (typeof result === 'number') {
                            that.pageControl.reset && that.pageControl.reset(options);
                        } else {
                            that._resetAnchor();
                            if (that._cachePath.indexOf(that.location.pathname)!=-1) {
                                that._cache[that.location.pathname] = that.pageElement;
                            }
                        }
                        that.navigated(state);
                        // 本项目中that._element.appendChild(newElement); 与 parentedComplete();不能异步执行
                        parentedComplete();
                    });
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
                // This function updates application controls once a navigation has completed.
                /// <param name="options" type="Object">可选参数，已实现参数列表如下：</param>
                ///     <param name="replace" type="true">是否替代历史</param>
                navigated: function (options) {
                    // 处理历史
                    var history = nav.history;
                    var backStack = history.backStack;
                    var back = backStack[backStack.length - 1];
                    if (back) {
                        var replace = options && options.replace;
                        // 如果replace为true，用目标地址替换原地址
                        if (replace) {
                            backStack.pop();
                        } else {
                            var backObject = Util.parseURL(back.location);
                            var currentObject = Util.parseURL(history.current.location);
                            // 如果目的地址的pathname, searh和原地址的pathname, search相等，用目标地址替换原地址
                            if (backObject.pathname == currentObject.pathname && backObject.search == currentObject.search) {
                                backStack.pop();
                            }
                        }
                    }


                    // 设置回退按钮的状态
                    if (this._backButton && this._backButton.hasAttribute('aria-label')) {
                        this._backButton.onclick = this._back.bind(this);

                        if (nav.canGoBack) {
                            this._backButton.removeAttribute("disabled");
                        } else {
                            this._backButton.setAttribute("disabled", "disabled");
                        }
                    }
                },
                /// <summary>后退</summary>
                _back: function () {
                    var canGoBack = this.pageControl && this.pageControl.canGoBack;
                    canGoBack = canGoBack || function () {
                        return this._backButton && this._backButton.hasAttribute('aria-label') && !this._backButton.getAttribute('disabled');
                    }.bind(this);
                    if (canGoBack()) {
                        nav.back();
                    }
                },
                /// <summary>页面退出动画</summary>
                /// <param name="element" type="HTMLElement|Array">Single element or collection of elements representing the incoming page.</param>
                _exitPageAsync: function (element) {
                    return WinJS.UI.Animation.exitPage(element);
                },
                /// <summary>页面进入动画</summary>
                /// <param name="element" type="HTMLElement|Array">Single element or collection of elements representing the outgoing page.</param>
                _enterPageAsync: function (element) {
                    return WinJS.UI.Animation.enterPage(element);
                },
                _backButton: {
                    get: function () {
                        return this.pageElement.querySelector("header[role=banner] .win-backbutton");
                    }
                },
            }
        )
    });
})();
