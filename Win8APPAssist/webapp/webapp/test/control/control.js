(function () {
    "use strict";

    WinJS.UI.Pages.define("/test/control/control.html", {
        // This function is called whenever a user navigates to this page. It
        // populates the page elements with the app's data.
        ready: function (element, options) {
            WinJS.UI.processAll().then(function(){
                document.getElementById("hello-world-with-events").addEventListener("blink",
                    function (event) {
                        console.log("blinked element this many times: " + event.count);
                    });
            }
            )
        }
    });
})();
