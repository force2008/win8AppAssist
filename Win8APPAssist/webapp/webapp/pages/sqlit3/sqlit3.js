(function () {
    "use strict";

    WinJS.UI.Pages.define("/pages/list/list.html", {
        // This function is called whenever a user navigates to this page. It
        // populates the page elements with the app's data.
        ready: function (element, options) {
            this.getNodes();
            this.addEvent();
        },
        getNodes: function () {
            this.btn = document.getElementById('initbtn');
            this.insertbtn = document.getElementById('insert');
            this.deletebtn = document.getElementById('delete');
            this.recordId = document.getElementById('recordid');
            this.result = document.getElementById('rst');
        },
        addEvent: function () {
            this.btn.addEventListener('click', this.onBtnClick.bind(this), false);
            this.insertbtn.addEventListener('click', this.onInsertBtnClick.bind(this), false);
            this.deletebtn.addEventListener('click', this.onDeleteBtnClick.bind(this), false);
        },
        onBtnClick: function () {
            this._initAsync(Windows.Storage.ApplicationData.current.localFolder.path);
        },
        onDeleteBtnClick: function () {
            if ((/^\d*$/g).test(this.recordId.value))
                this.database.runAsync('delete from test2 where id=?', [this.recordId.value])
                    .then(function (isok) {
                        this.info('数据删除成功');
                        this.showAllData();
                    }.bind(this))
        },
        onInsertBtnClick: function () {
            var num = Math.floor(Math.random() * 100);
            this.database.runAsync('INSERT INTO test2( a, b) VALUES( ' + num + ', \'hello' + num + '\')').
                then(function () {
                    //this.info('one record inserted. id is:' + num);
                    this.showAllData();
                }.bind(this));
        },
        showAllData: function () {
            var table = document.getElementById('tbl');
            WinJS.Utilities.empty(table);
            this.database.allAsync('select * from test2').
            then(function (rows) {
                var len = rows.length;
                for (var i = 0; i < len; i++) {
                    var tr = document.createElement('tr');
                    var td1 = document.createElement('td');
                    td1.innerText = rows[i].id;
                    var td2 = document.createElement('td');
                    td2.innerText = rows[i].a;
                    var td3 = document.createElement('td');
                    td3.innerText = rows[i].b;
                    tr.appendChild(td1);
                    tr.appendChild(td2);
                    tr.appendChild(td3);
                    table.appendChild(tr);
                }
            }.bind(this));
        },
        _initAsync: function (path) {
            var thisObj = this;
            var dbPath = path + "\\list.db";
            var promise = SQLite3JS.openAsync(dbPath).then(function (db) {
                thisObj.database = db;
                return thisObj.database.runAsync('CREATE TABLE IF NOT EXISTS test2(id INTEGER PRIMARY KEY,a INT, b TEXT)');
            }).then(function () {
                return thisObj.database.runAsync('INSERT INTO test2( a, b) VALUES( 6, \'hello1\')');
            }).then(function (rows) {// 加载数据到cacheAuto
                this.showAllData();
                this.info('APP - Load data Ok. ' + 'path is:' + dbPath);
            }.bind(this));
            return promise;
        },
        info: function (_info) {
            this.result.innerHTML += _info + '<br/>';
        },
        //  <summary>关闭存储</summary>
        _close: function (path) {
            if (this.database) this.database.close();
        },
    });
})();
