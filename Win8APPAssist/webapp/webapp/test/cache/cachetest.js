(function () {
    "use strict";

    WinJS.UI.Pages.define("/test/cache/cache.html", {
        // This function is called whenever a user navigates to this page. It
        // populates the page elements with the app's data.
        ready: function (element, options) {
            // TODO: Initialize the page here.
            this.container = document.getElementById('container');
            this.loadingNode = this.createLoadingNode();
            this.module = new Nej.Util.Module({
                id: 'a',
                template: document.getElementById('myItemTemplate'),
                parent: this.container,
                bindDefine: { title: '', text: '', id: '' },
                CacheClass: Nej.Test.CacheTest,
                onafterload: this.onListLoadEnd.bind(this),
                onbeforeload: this.onListBeforeLoad.bind(this, true),
                onafteritemsdelete: this.cbItemsDelete.bind(this),
                onbeforepullrefresh: this.onListBeforeLoad.bind(this, false),
                onafterpullrefresh: this.onPullRefreshEnd.bind(this, false),
                onafteritemload: this.onAfterItemLoad.bind(this),
                onafteritemupdate: this.onAfterItemUpdate.bind(this),
                appendItemAnimation: this.appendItemAnimation.bind(this),
                item: {
                    klass: Test.Cache.DataItem,
                    options: {
                        swipeDown: this._toggleSelected.bind(this),
                        template: document.getElementById('myItemTemplate')
                    }
                }
            });
            this.cache = this.module.getCache();
            this.page = 0;
            this.pageCount = 10;
            this.module.getList({ key: 'abc', data: {}, offset: this.page * this.pageCount, limit: this.pageCount });
            //this.module.getItem({ id: '123', key: 'abc', data: {} });

            document.getElementById('delete').addEventListener('click', this.onItemsDelete.bind(this), false);
            document.getElementById('add').addEventListener('click', this.onItemAdd.bind(this), false);
            this.viewBtn = document.getElementById('viewitem');
            this.viewBtn.addEventListener('click', this.onItemView.bind(this), false);
            document.getElementById('edit').addEventListener('click', this.onItemEdit.bind(this), false);
            this.itempane = document.getElementById('itempane');
            this.animating = WinJS.Promise.wrap();
            this.itemPaneTemplate = document.getElementById('itemViewTemplate');
            this.itemEditTemplate = document.getElementById('itemEditTemplate');
            this.container.addEventListener('mousewheel', this.onMouseWheel.bind(this), false);
            this.selectId = [];
            this.container.parentNode.addEventListener('scroll', this.onContainerScroll.bind(this));

        },
        appendItemAnimation: function (_event) {
            var _elm = _event.detail;
            var addToSearchList = WinJS.UI.Animation.createAddToListAnimation(_elm, this.container.children);
            addToSearchList.execute();
        },
        createLoadingNode: function () {
            var node = document.createElement('div')
            WinJS.Utilities.setInnerHTMLUnsafe(node, '<progress class="win-ring"></progress>');
            node.className = 'myItem';
            return node;
        },
        onItemEdit: function () {
            if (this.selectId.length != 0) {
                this.editId = this.selectId[0];
                var _item = this.cache.getItemInCache(this.selectId[0]);
                var item = this.itemEditTemplate.winControl.render(_item).then(function (result) {
                    this.itempane.replaceChild(result, this.itempane.firstChild);
                    result.querySelector('[type="button"]').addEventListener('click', this.onSaveClick.bind(this));
                }.bind(this));
                if (!this.show) {
                    this.show = true;
                    this.animating = this.animating.then(function () {
                        this.itempane.style.opacity = "1";
                        this.itempane.style.zIndex = 1;
                        return WinJS.UI.Animation.showPanel(this.itempane);
                    }.bind(this))
                }
            }
        },
        onAfterItemUpdate: function () {
            this.show = false;
            this.animating = this.animating.then(function () {
                this.itempane.style.opacity = "0";
                this.itempane.style.zIndex = -1;
                return WinJS.UI.Animation.hidePanel(this.itempane);
            }.bind(this))
        },
        onSaveClick: function (_event) {
            var _title = this.itempane.querySelector('[data-win-bind="value: title"]').value;
            var _text = this.itempane.querySelector('[data-win-bind="value: text"]').value;
            this.module.updateItem({ data: { title: _title, text: _text, id: this.editId }, key: 'abc' });
        },
        onItemAdd: function (_event) {
            this.module.addItem({ key: 'abc', data: {} });
        },
        //eventProcess: function (_event) {
        //    var _elm = _event.detail.elm;
        //    var _data = _event.detail.data;
        //    _elm.addEventListener("contextmenu", this._toggleSelected.bind(this, _data, _elm), false);
        //    Util.Element.addEvent(_elm, 'swipeDown', this._toggleSelected.bind(this, _data, _elm), false);
        //    //_elm.addEventListener("swipeRight", this._toggleSelected.bind(this), false);
        //    Util.Element.addEvent(_elm, 'swipeRight', this._toggleSelected.bind(this, _data, _elm), false);
        //    //_elm.addEventListener("mousedown", this._mouseDown.bind(this), false);
        //    _elm.addEventListener("MSPointerUp", this.onPointerUp, false);
        //    _elm.addEventListener("MSPointerDown", this.onPointerDown, false);

        //},
        onPointerUp: function (evt) {
            WinJS.UI.Animation.pointerUp(evt.srcElement);
        },
        onPointerDown: function (evt) {
            WinJS.UI.Animation.pointerDown(evt.srcElement);
        },
        onMouseWheel: function (_event) {
            if (this.container.parentNode.scrollWidth <= this.container.parentNode.clientWidth && !this.loading) {
                if (_event.wheelDelta < 0 && !this.cache.getLoaded('abc')) {
                    this.loading = true
                    this.module.getList({ key: 'abc', data: {}, offset: this.page * this.pageCount, limit: this.pageCount, force: true });
                }
                else {
                    this.cache.pullRefresh({ key: 'abc', data: {}, offset: this.page * this.pageCount, limit: this.pageCount });
                }
            }
        },
        onContainerScroll: function (_event) {
            console.log('scroll:' + _event.target.scrollLeft + ',scrollWidth:' + _event.target.scrollWidth);
            var _container = _event.target;
            if ((_container.scrollWidth - (_container.scrollLeft + _container.clientWidth)) < 200 && !this.loading && !this.cache.getLoaded('abc')) {
                this.loading = true;
                console.log('loading start:' + this.loading);
                this.module.getList({ key: 'abc', data: {}, offset: this.page * this.pageCount, limit: this.pageCount, force: true });
            }
            else if (_container.scrollLeft == 0 && !this.loading) {
                this.loading = true;
                console.log('pull start:' + this.loading);
                this.module.pullRefresh({ key: 'abc', data: {} });
            }
        },
        onItemView: function () {
            if (this.selectId.length != 0) {
                this.module.getItem({ id: this.selectId[0] });
            }
        },
        onAfterItemLoad: function (_event) {
            var _item = _event.detail.data;
            var item = this.itemPaneTemplate.winControl.render(_item).then(function (result) {
                this.itempane.replaceChild(result, this.itempane.firstChild);
            }.bind(this));
            if (!this.show) {
                this.show = true;
                this.viewBtn.value = '隐藏查看项';
                this.animating = this.animating.then(function () {
                    this.itempane.style.opacity = "1";
                    this.itempane.style.zIndex = 1;
                    return WinJS.UI.Animation.showPanel(this.itempane);
                }.bind(this))
            } else {
                this.show = false;
                this.viewBtn.value = '查看选中第一项';
                this.animating = this.animating.then(function () {
                    this.itempane.style.opacity = "0";
                    this.itempane.style.zIndex = -1;
                    return WinJS.UI.Animation.hidePanel(this.itempane);
                }.bind(this))
            }
        },
        onListLoadEnd: function (_event) {
            this.page++;
            this.loading = false;
            this.loadingNode.parentNode.removeChild(this.loadingNode);
            console.log('listload end:' + this.loading);
        },
        onPullRefreshEnd: function (_event) {
            this.loading = false;
            console.log('pull end:' + this.loading);
            if (!!this.loadingNode.parentNode)
                this.loadingNode.parentNode.removeChild(this.loadingNode);
        },
        
        _toggleSelected: function (_event) {
            var _id = _event.detail;
            var _index = Util.indexOf(this.selectId, function (_item) { return _id == _item });
            if (_index == -1)
                this.selectId.push(_id);
            else
                this.selectId.splice(_index, 1);
        },
        onItemsDelete: function () {
            if (this.selectId.length != 0)
                this.module.deleteItems({ key: 'abc', data: this.selectId })
        },
        cbItemsDelete: function () {
            this.selectId = [];
        },
        onListBeforeLoad: function (_isAppend) {
            if (_isAppend)
                this.container.appendChild(this.loadingNode);
            else
                this.container.insertBefore(this.loadingNode, this.container.firstChild);
        },
    });
})();
