var myData =[
    { title: "The Smoothest", text: "Maybe the smoothest ice cream ever." },
    { title: "What a great flavor!", text: "Although the texture was a bit lacking, this one has the best flavor I have tasted!" },
    { title: "Didn't like the 'choco bits'", text: "The little bits of chocolate just weren't working for me." },
    { title: "Loved the peanut butter", text: "The peanut butter! I loved this ice cream. Did I mention I loved this ice cream! However, The little bits of chocolate just weren't working for me. I will try it again another time to give my final opinion. I will write back when I have finished the next carton." },
    { title: "Wish there was more sugar", text: "This wasn't sweet enough for me. I will have to try your other flavors, but maybe this is too healthy for me. Who knows?" },
    { title: "Texture was perfect", text: "This was the smoothest ice cream I have ever had" },
    { title: "Kept wishing there was more", text: "When I got to the end of each carton I kept wishing there was more ice cream. It was delicious! The best part of this delicious snack was the strawberries!" },
    { title: "The Smoothest", text: "Maybe the smoothest ice cream ever." }
];
var bindDefine = { title: '', text: '' };
(function () {
    "use strict";
    
    var itmcount = 0;
    WinJS.UI.Pages.define("/pages/list/bindlist.html", {
        // This function is called whenever a user navigates to this page. It
        // populates the page elements with the app's data.
        ready: function (element, options) {
            WinJS.UI.processAll().then(function () {
                
            })
            var defineItem = WinJS.Binding.define(bindDefine);
            var itemControl = element.querySelector("#myItemTemplate").winControl;
            var renderElement = document.getElementById('container');
            var itemCache = [];
            var bindData = [];
            //listView.itemTemplate = this.itemRenderer;
            for (var i = 0; i < myData.length; i++) {
                var data = new defineItem(myData[i])
                bindData.push(data);

                var item = itemControl.render(data).then(function (result) {
                    renderElement.appendChild(result);
                    itemCache.push(result);
                });
                        //.done(function (result) {
                        //    
                //});
                
            }
            function loadMoreButtonHandler1() {
                // Loads additional content in the ListView
                //var iDS = listView1.itemDataSource;
                for (var i = 0; i < 3; i++) {
                    //iDS.insertAtEnd(new Obj({ title: i + "The Smoothest" + i, text: "Maybe the smoothest ice cream ever." }))
                    //iDS.insertAtEnd(iDS.getCount(), { title: i + "The Smoothest" + i, text: "Maybe the smoothest ice cream ever." })
                    var data = new defineItem({ title: i + "The Smoothest" + i, text: "Maybe the smoothest ice cream ever." });
                    bindData.push(data);
                    var item = itemControl.render(data).then(function (result) {
                        renderElement.appendChild(result);
                        itemCache.push(result);
                    });

                }
                //listView.loadMorePages();
            }
            document.getElementById('appendData').addEventListener("click", loadMoreButtonHandler1, false);
            document.getElementById('getcount').addEventListener("click", function () {
                document.getElementById('count').innerText = myData.length;
            }, false);
            document.getElementById('deleteItem').addEventListener("click", function () {
                var count = myData.length;
                if (count > 0) {
                    bindData.splice(0, 1);
                    renderElement.removeChild(itemCache[0]);
                    itemCache.splice(0, 1);
                }
                    
            }, false);
            document.getElementById('changeItem').addEventListener("click", function () {
                localStorage['x'] = JSON.stringify({a:'a',b:'b'});
                var count = myData.length;
                if (count > 0) {
                    var data = bindData[0];
                    data.title = 'xxxx';
                    
                }

            }, false);
            
        }
    });
})();
