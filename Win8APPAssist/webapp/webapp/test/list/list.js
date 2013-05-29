(function () {
    "use strict";

    WinJS.UI.Pages.define("/test/list/list.html", {
        // This function is called whenever a user navigates to this page. It
        // populates the page elements with the app's data.
        ready: function (element, options) {
            var player = {
                name: '迈克尔-基德-吉尔克里斯特',
                color: { fore: '#fff', back: '#f0a89d' },
                university: 'Kentucky',
                avatar: 'http://www.sinaimg.cn/ty/nba/player/2008/5008.jpg'
            };
            WinJS.UI.processAll().then(function () {
                var element = document.querySelectorAll('[data-win-control="WinJS.Binding.Template"]');
                var template = element[0].winControl;
                this.playerDom = WinJS.Binding.as(player);
                template.render(this.playerDom, Util.Element.getElement('container'));
                var template = element[1].winControl;
                //template.render(player, Util.Element.getElement('container1'));
            }.bind(this));
            Util.Element.getElement('changeData').addEventListener('click',this.changeData.bind(this));
            Util.Element.getElement('changeData1').addEventListener('click', function () { player.name = 'aaa'; player.color.fore = '#000' }, 'false');
        },
        changeData: function () {
            this.playerDom.name = '魏文庆';
            this.playerDom.color.fore = '#000';
            delete this.playerDom;
        }
        
       
    });
})();
