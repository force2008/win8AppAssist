
(function () {
    "use strict"
    var PopupMenu = WinJS.Class.define(
        /// <summary>初使化卡片对象popupMenu</summary>
        function PopupMenuConstructor(_options) {
            this.menu = new Windows.UI.Popups.PopupMenu();
            for (var k in _options) {
                if (Util.isTypeOf(_options[k], 'Function'))
                    this.menu.commands.append(new Windows.UI.Popups.UICommand(k, _options[k]))
            }
        },
        {
            /// <summary>卡片显示</summary>
            /// <param name="_rect" type="Object">形行位置</param>
            /// <param name="_placement" type="below|above|left|right">Description</param>
            show: function (_rect, _placement) {
                _placement = _placement || "above"
                this.menu.showForSelectionAsync(_rect, Windows.UI.Popups.Placement[_placement]);
            }
        },
        {})
    WinJS.Namespace.define('Util', {
        PopupMenu: PopupMenu
    })
}())
///appbar WinJS.UI.AppBar http://msdn.microsoft.com/en-us/library/windows/apps/br229670.aspx 
/// <summary>应用中appbar的command会有不同的状态，可以设置多个appbar，或是通过改变appbar的onbeforeshow回调事件把command隐藏和显示状态</summary>

///日期选择 WinJS.UI.DatePicker http://msdn.microsoft.com/en-us/library/windows/apps/br211681.aspx

///Mixin 是实现某类功能的对象 WinJS.Class.mix WinJS.UI.DOMEventMixin http://msdn.microsoft.com/zh-cn/library/windows/apps/hh967789.aspx
/// <summary>使类拥有事件分发功能，使控件拥有addEventListener,removeEventListener,dispatchEvent</summary>

///scroll滚动控件 WinJS.UI.FlipView  http://msdn.microsoft.com/library/windows/apps/Hh465425

///弹出窗口 WinJS.UI.Flyout http://msdn.microsoft.com/zh-cn/library/windows/apps/br211726.aspx

///添加菜单 WinJS.UI.Menu  http://msdn.microsoft.com/zh-cn/library/windows/apps/hh780659.aspx

///WinJS.UI.HtmlControl 控件 http://msdn.microsoft.com/en-us/library/windows/apps/hh700625.aspx

///WinJS.UI.ListLayout,WinJS.UI.GridLayout,WinJS.UI.ListView http://msdn.microsoft.com/en-us/library/windows/apps/br211837.aspx ,
///http://msdn.microsoft.com/en-us/library/windows/apps/hh465382.aspx

/// 星号等级控件 WinJS.UI.Rating http://msdn.microsoft.com/en-us/library/windows/apps/hh465489.aspx

///WinJS.UI.SemanticZoom http://msdn.microsoft.com/en-us/library/windows/apps/br229690.aspx

///setting pane 控件 WinJS.UI.SettingsFlyout http://msdn.microsoft.com/en-us/library/windows/apps/hh701253.aspx

///WinJS.UI.TabContainer 阻止该元素的里的tab响应 http://msdn.microsoft.com/en-us/library/windows/apps/hh701297.aspx

///WinJS.UI.ToggleSwitch A control that lets the user switch an option between two states: on (checked is set to true) and off (checked is set to false).
///http://msdn.microsoft.com/en-us/library/windows/apps/hh701411.aspx

/// WinJS.UI.Tooltip  Displays a tooltip that can contain images and formatting.
///http://msdn.microsoft.com/en-us/library/windows/apps/br229763.aspx

///WinJS.UI.ViewBox Scales a single child element to fill the available space without resizing it. This control reacts to changes in the size of the container as well as changes in size of the child element. For example, a media query may result in a change in aspect ratio.
///http://msdn.microsoft.com/en-us/library/windows/apps/br229771.aspx

///控件列表  http://msdn.microsoft.com/library/windows/apps/Hh465453

///WinJS.UI.TimePicker, Allows users to select time values.
///http://msdn.microsoft.com/en-us/library/windows/apps/br229736.aspx

///添加控件和内容 http://msdn.microsoft.com/zh-cn/library/windows/apps/hh465393.aspx
///appbar will not include in widget，as user not use frequently ，and ms maybe remove this function the later version

///WinJS.UI.IListDataSource http://msdn.microsoft.com/en-us/library/windows/apps/br211786.aspx

///create UI http://msdn.microsoft.com/en-us/library/windows/apps/br211362.aspx

/// WinJS.UI.Animation 节点动动参考 http://msdn.microsoft.com/en-us/library/windows/apps/br229780.aspx

/// Tiles, badges, and notifications  http://msdn.microsoft.com/en-us/library/windows/apps/hh779725.aspx
