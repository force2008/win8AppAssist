var myData = new WinJS.Binding.List([
    { title: "The Smoothest", text: "Maybe the smoothest ice cream ever." },
    { title: "What a great flavor!", text: "Although the texture was a bit lacking, this one has the best flavor I have tasted!" },
    { title: "Didn't like the 'choco bits'", text: "The little bits of chocolate just weren't working for me." },
    { title: "Loved the peanut butter", text: "The peanut butter! I loved this ice cream. Did I mention I loved this ice cream! However, The little bits of chocolate just weren't working for me. I will try it again another time to give my final opinion. I will write back when I have finished the next carton." },
    { title: "Wish there was more sugar", text: "This wasn't sweet enough for me. I will have to try your other flavors, but maybe this is too healthy for me. Who knows?" },
    { title: "Texture was perfect", text: "This was the smoothest ice cream I have ever had" },
    { title: "Kept wishing there was more", text: "When I got to the end of each carton I kept wishing there was more ice cream. It was delicious! The best part of this delicious snack was the strawberries!" },
    { title: "The Smoothest", text: "Maybe the smoothest ice cream ever." }
]);
var myData1 = new WinJS.Binding.List([

{ title: "Banana", description: "Banana Frozen Yogurt", picture: "images/banana.jpg" },
{ title: "Orange", description: "Orange Sherbet", picture: "images/orange.jpg" },
{ title: "Vanilla", description: "Vanilla Ice Cream", picture: "images/vanilla.jpg" },
{ title: "Mint", description: "Mint Gelato", picture: "images/mint.jpg" },
{ title: "Strawberry", description: "Strawberry Sorbet", picture: "images/strawberry.jpg" }
]);
(function () {
    "use strict";
    
    var itmcount = 0;
    WinJS.UI.Pages.define("/test/list/listview.html", {
        // This function is called whenever a user navigates to this page. It
        // populates the page elements with the app's data.
        ready: function (element, options) {
            WinJS.UI.processAll().then(function () {
                
            })
            var listView = element.querySelector("#listView").winControl;
            //listView.itemTemplate = this.itemRenderer;
            listView.forceLayout();
            function loadMoreButtonHandler1() {
                // Loads additional content in the ListView
                //var iDS = listView1.itemDataSource;
                for (var i = 0; i < 3; i++) {
                    //iDS.insertAtEnd(new Obj({ title: i + "The Smoothest" + i, text: "Maybe the smoothest ice cream ever." }))
                    //iDS.insertAtEnd(iDS.getCount(), { title: i + "The Smoothest" + i, text: "Maybe the smoothest ice cream ever." })
                    myData.push({ title: i + "The Smoothest" + i, text: "Maybe the smoothest ice cream ever." })
                }
                listView.loadMorePages();
            }
            document.getElementById('appendData').addEventListener("click", loadMoreButtonHandler1, false);
            document.getElementById('getcount').addEventListener("click", function () {
                var count = listView.itemDataSource.getCount();
                document.getElementById('count').innerText = count._value;
            }, false);
            document.getElementById('deleteItem').addEventListener("click", function () {
                var count = listView.itemDataSource.getCount()._value;
                if (count > 0) {
                    myData.splice(0, 1);
                }
                    
            }, false);
            document.getElementById('changeItem').addEventListener("click", function () {
                var count = listView.itemDataSource.getCount()._value;
                if (count > 0) {
                    var data = myData.getAt(0);
                    data.title = 'xxxx';
                    myData.setAt(0,  data);
                }

            }, false);
            
        }
    });
})();
