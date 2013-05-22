(function () {
    "use strict";

    function initializeProperties(target, members) {
        var keys = Object.keys(members);
        var properties;
        var i, len;
        for (i = 0, len = keys.length; i < len; i++) {
            var key = keys[i];
            var enumerable = key.charCodeAt(0) !== /*_*/95;
            var member = members[key];
            if (member && typeof member === 'object') {
                if (member.value !== undefined || typeof member.get === 'function' || typeof member.set === 'function') {
                    if (member.enumerable === undefined) {
                        member.enumerable = enumerable;
                    }
                    properties = properties || {};
                    properties[key] = member;
                    continue;
                }
            }
            if (!enumerable) {
                properties = properties || {};
                properties[key] = { value: member, enumerable: enumerable, configurable: true, writable: true }
                continue;
            }
            target[key] = member;
        }
        if (properties) {
            Object.defineProperties(target, properties);
        }
    }
    WinJS.Namespace.define('Global', {
        ///http://msdn.microsoft.com/en-us/library/windows/apps/windows.ui.viewmanagement.inputpane(v=win.10).aspx
        /// <summary>虚拟键盘输入对象，hiding,showing,OccludedRect等信息</summary>
        inputPane: Windows.UI.ViewManagement.InputPane.getForCurrentView(),
        ///http://msdn.microsoft.com/en-us/library/windows/apps/windows.ui.viewmanagement.applicationview.aspx
        appView: Windows.UI.ViewManagement.ApplicationView,
        ///http://msdn.microsoft.com/en-us/library/windows/apps/br242295.aspx
        /// <summary>Provides classes and enumerations used for managing the settings for an app's view</summary>
        /// <summary>ApplicationViewState(fullScreenLandscape,Filled ,Snapped ,FullScreenPortrait )</summary>
        ViewManagement: Windows.UI.ViewManagement,

    })
    function derive(baseClass, constructor, instanceMembers, staticMembers) {
        /// <signature helpKeyword="WinJS.Class.derive">
        /// <summary locid="WinJS.Class.derive">
        /// Creates a sub-class based on the supplied baseClass parameter, using prototypal inheritance.
        /// </summary>
        /// <param name="baseClass" type="Function" locid="WinJS.Class.derive_p:baseClass">
        /// The class to inherit from.
        /// </param>
        /// <param name="constructor" type="Function" locid="WinJS.Class.derive_p:constructor">
        /// A constructor function that is used to instantiate this class.
        /// </param>
        /// <param name="instanceMembers" type="Object" locid="WinJS.Class.derive_p:instanceMembers">
        /// The set of instance fields, properties, and methods to be made available on the class.
        /// </param>
        /// <param name="staticMembers" type="Object" locid="WinJS.Class.derive_p:staticMembers">
        /// The set of static fields, properties, and methods to be made available on the class.
        /// </param>
        /// <returns type="Function" locid="WinJS.Class.derive_returnValue">
        /// The newly-defined class.
        /// </returns>
        /// </signature>
        if (baseClass) {
            constructor = constructor || function () { };
            var basePrototype = baseClass.prototype;
            constructor.prototype = Object.create(basePrototype);
            //constructor.prototype.supro = Object.create(basePrototype);
            console.log('1111baseClass:' + baseClass);
            constructor.prototype.super = (function () {
                return function () {
                    console.log('2222baseClass:' + baseClass);
                    var method = arguments[0];//Array.prototype.slice(arguments, 1);
                    if (Object.prototype.toString.call(method) == '[object Arguments]') {
                        var args = [];
                        for (var i = 0, l = arguments.lenth; i < l; i++)
                            args.push(arguments[i]);
                        constructor.supro.constructor.apply(args);
                    }
                    else {
                        var args = Array.prototype.slice(arguments, 1);
                        if (!!constructor.supro[method])
                            return constructor.supro[method](this, args)
                    }
                }
            }());

            constructor.supro = Object.create(basePrototype);
            if (!!baseClass.supro)
                constructor.suproConstructor = baseClass.supro.constructor;
            WinJS.Utilities.markSupportedForProcessing(constructor);
            Object.defineProperty(constructor.prototype, "constructor", { value: constructor, writable: true, configurable: true, enumerable: true });
            if (instanceMembers) {
                initializeProperties(constructor.prototype, instanceMembers);
                //constructor.prototype.supro = {};
                //initializeProperties(constructor.prototype.supro, Object.create(basePrototype));
            }
            if (staticMembers) {
                initializeProperties(constructor, staticMembers);
            }
            return constructor;
        } else {
            return define(constructor, instanceMembers, staticMembers);
        }
    }
    WinJS.Class.derive = derive;
})()