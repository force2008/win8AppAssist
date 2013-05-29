(function () {
    "use strict";

    WinJS.UI.Pages.define("/test/search/search.html", {
        // This function is called whenever a user navigates to this page. It
        // populates the page elements with the app's data.
        ready: function (element, options) {
            // TODO: Initialize the page here.
            document.getElementById('content').innerHTML = options.queryText;
        },
        
        
    });
})();
