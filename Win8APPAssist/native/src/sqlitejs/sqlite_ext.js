(function () {
    // extend the sqlite database to support table update.
    // we use a table named "DBConfig" to record the version
    // of current database. then we must do update task before
    // the database init.
    WinJS.Namespace.define("SQLiteExt", {
        hasRecordsAsync: function (db, table) {
            var sql = "select count(*) from " + table;
            return db.oneAsync(sql).then(function (r) {
                return r["count(*)"] !== 0;
            });
        },

        isTableExistAsync: function (db, table) {
            var sql = "select count(*) from sqlite_master where name=\'" + table + "\'";
            return db.oneAsync(sql).then(function (r) {
                return r["count(*)"] !== 0;
            });
        },
        getVersionAsync: function (db) {
            var promise = SQLiteExt.isTableExistAsync(db, "DBConfig").then(function (r) {
                if (r) {
                    var sql = "select value from DBConfig where name='version'";
                    return db.oneAsync(sql)
                }
            }).then(function (r) {
                var version = "0";
                if (r)
                    version = r.value;
                return version;
            })
            return promise;
        },

        initDBConfigTableAsync: function (db, defaultConfig) {
            var sql = "CREATE TABLE IF NOT EXISTS DBConfig (name TEXT PRIMARY KEY, value TEXT)";
            return db.runAsync(sql).then(function () {
                if (defaultConfig && defaultConfig.length > 0) {
                    var p = []
                    defaultConfig.forEach(function (config, _) {
                        p.push(SQLiteExt.setDBConfigAsync(db, config.name, config.value))
                    })
                    return WinJS.Promise.join(p);
                }
            }).then(function () {
                return "0";
            });
        },
        setDBConfigAsync: function (db, name, value) {
            var sql = "INSERT OR REPLACE INTO DBConfig (name, value) VALUES (?, ?)";
            return db.runAsync(sql, [name, value]);
        }
    });
})()