(function () {
    "use strict";
    function showFlyout(flyout, anchor, placement) {
        flyout.winControl.show(anchor, placement);
    }
    WinJS.UI.Pages.define("/pages/jst/jst.html", {
        // This function is called whenever a user navigates to this page. It
        // populates the page elements with the app's data.
        ready: function (element, options) {
            WinJS.UI.processAll();
            Util.JST.parseTemplate('jsttpl');
            var btn = Util.getElement('addSenior');
            btn.addEventListener( 'click',this.addSenior.bind(this), false);
            var btn2 = Util.getElement('addHtmlTemplate');
            btn2.addEventListener('click', this.addHtmlTemplate.bind(this), false);

            var btn3 = Util.getElement('addItemTemplate');
            btn3.addEventListener('click', this.addItemTemplate.bind(this), false);

            var btn4 = Util.getElement('recycle');
            btn4.addEventListener('click', this.recycleInstance.bind(this), false);

            var btn5 = Util.getElement('getInstance');
            btn5.addEventListener('click', this.getInstance.bind(this), false);
            var btn6 = Util.getElement('popmenu');
            btn6.addEventListener('click', this.showPopmenu.bind(this), false);
            
            document.getElementById("respondButton").addEventListener("click", this.showMenu.bind(this), false);
            document.getElementById("alwaysSaveMenuItem").addEventListener("click", this.alwaysSave.bind(this), false);
            document.getElementById("replyMenuItem").addEventListener("click", this.reply.bind(this), false);
            document.getElementById("replyAllMenuItem").addEventListener("click", this.replyAll.bind(this), false);
            document.getElementById("forwardMenuItem").addEventListener("click", this.forward.bind(this), false);
        },
        showMenu:function(){
            showFlyout(respondFlyout, respondButton, "bottom");
        },
        hideFlyout:function (flyout) {
            flyout.winControl.hide();
        },
        alwaysSave: function () {
            var alwaysSaveState = document.getElementById("alwaysSaveMenuItem").winControl.selected;
        },
        reply:function () {
            this.hideFlyout(respondFlyout);
        },
        replyAll:function () {
            this.hideFlyout(respondFlyout);
        },
        forward:function () {
            this.hideFlyout(respondFlyout);
        },
        showPopmenu: function (event) {
            if (!this.m)
                this.m = new Menu({
                    'savedraft': this.saveClick.bind(this),
                    'delete': this.deleteClick.bind(this)
                })
            
                this.m.show(Util.Element.getRect(event.srcElement, 5), 'below');

            //var menu = new Windows.UI.Popups.PopupMenu();
            //menu.commands.append(new Windows.UI.Popups.UICommand('savedraft', this.saveClick.bind(this)));
            //menu.commands.append(new Windows.UI.Popups.UICommand('delete', this.deleteClick.bind(this)));
            //menu.showForSelectionAsync(Util.Element.getRect(event.srcElement, 5), Windows.UI.Popups.Placement.below);

        },
        saveClick: function (event) {},
        deleteClick: function (event) {},
        getInstance: function (_event) {
            this.friendItems = FriendItem.getInstance({
                data: { name: '魏文庆', gender: 1, ava: '/images/0.jpg' },
                ondelete: this.onDeleteItem.bind(this),
                parent: 'friends'
            });
        },
        recycleInstance: function (_event) {
            FriendItem.recycle(this.friendItems);
        },
        addSenior: function (event) {
            var _econtent = Util.getElement('seniors');
            var _html = Util.JST.getTextTemplate('seniorTextTemplate');
            var _node = Util.html2node(_html);
            _econtent.appendChild(_node);

        },
        addHtmlTemplate: function (event) {
            var _data = {workers: [{name:'魏文庆', gender:1}, 
                            {name:'严跃杰', gender:1}, 
                            {name:'张晓容'}]};
            var _html = Util.JST.getHtmlTemplate('workersJstTemplate', _data);
            var _node = Util.html2node(_html);
            var _econtent = Util.getElement('htmlTemplate');
            _econtent.appendChild(_node);
        },
        addItemTemplate: function (_event) {
            // 添加名片列表逻辑
            var _friends = [{ name: '魏文庆', gender: 1, ava: '/images/0.jpg' },
                                     { name: '严跃杰', gender: 1, ava: '/images/1.jpg' },
                                     { name: '张晓容', ava: '/images/2.jpg' }];
            this.friendItems = Util.JST.getItemTemplate(_friends, FriendItem, {
                ondelete: this.onDeleteItem.bind(this),
                parent: 'friends'
            });

        },
        onDeleteItem: function () {
            
        }
    });
    
    var FriendItem = WinJS.Class.derive(Item, function () {
        Item.apply(this,arguments);
    }, {
        _initNode: function () {
            Item.prototype._initNode.apply(this, arguments);
            //this.super('_initNode')
            var _espans = this._body.getElementsByTagName('span');
            this._ecb = this._body.getElementsByTagName('input')[0];
            this._eimg = this._body.getElementsByTagName('img')[0];
            this._enumber = _espans[0];
            this._ename = _espans[1];
            this._egender = _espans[2];
            this._edelete = this._body.getElementsByTagName('a')[0];
            this._edelete.addEventListener('click', this._onClickDelete.bind(this));
            this._ecb.addEventListener('click', this._onClickCheck.bind(this));
        },
        _initXGui : function () {
            // 设置节点模板的id，将结构和逻辑关联起来
            this._seed_html = 'friendNodeTemplate';
        },
        //_reset : function () {
        //    // 设置节点模板的id，将结构和逻辑关联起来
        //    this.super('_reset')
        //},
        _doRefresh : function (_data) {
            this._eimg.src = _data.ava;
            this._enumber.innerText = this._index + 1;
            this._ename.innerText = _data.name;
            this._egender.innerText = _data.gender ? '男' : '女';
        },
        /**
     * 点击删除的响应函数
     * @param {Object} _event   事件对象
     */
        _onClickDelete : function (_event) {
            // todo
            this.dispatchEvent('ondelete', 1);
            this._destroy()
        },
        _onClickCheck : function (_event) {
            this._ecb.checked ? WinJS.Utilities.addClass(this._body, 'checked') : WinJS.Utilities.removeClass(this._body, 'checked');;
        },
        _destroy : function () {
            Item.prototype._destroy.apply(this, arguments);
            this._eimg.src = '';
            this._ecb.checked = false;
            WinJS.Utilities.removeClass(this._body, 'checked');
        }
    }, {
        allocate: Item.allocate,
        recycle: Item.recycle,
        getInstance: Item.getInstance
    })
})();
