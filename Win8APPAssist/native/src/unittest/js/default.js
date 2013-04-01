// For an introduction to the Blank template, see the following documentation:
// http://go.microsoft.com/fwlink/?LinkId=232509
(function () {
    "use strict";

    var app = WinJS.Application;
    var activation = Windows.ApplicationModel.Activation;
    WinJS.strictProcessing();

    app.onactivated = function (args) {
        if (args.detail.kind === activation.ActivationKind.launch) {
            if (args.detail.previousExecutionState !== activation.ApplicationExecutionState.terminated) {
                // TODO: This application has been newly launched. Initialize
                // your application here.
            } else {
                // TODO: This application has been reactivated from suspension.
                // Restore application state here.
            }
            args.setPromise(WinJS.UI.processAll());
        }
        openButton.addEventListener("click", onOpen)
    };

    app.oncheckpoint = function (args) {
        // TODO: This application is about to be suspended. Save any state
        // that needs to persist across suspensions here. You might use the
        // WinJS.Application.sessionState object, which is automatically
        // saved and restored across suspension. If you need to complete an
        // asynchronous operation before your application is suspended, call
        // args.setPromise().
    };

    function onOpen(){
        var openPicker = new Windows.Storage.Pickers.FileOpenPicker();
        openPicker.viewMode = Windows.Storage.Pickers.PickerViewMode.thumbnail;
        openPicker.suggestedStartLocation = Windows.Storage.Pickers.FileOpenPicker;
        // Users expect to have a filtered view of their folders depending on the scenario.
        // For example, when choosing a documents folder, restrict the filetypes to documents for your application.
        openPicker.fileTypeFilter.replaceAll([".eml", ".jpg", ".jpeg"]);

        // Open the picker for the user to pick a file
        openPicker.pickSingleFileAsync().then(function (file) {
            if (file) {
                // Application now has read/write access to the picked file
                WinJS.log && WinJS.log("Picked photo: " + file.name, "sample", "status");
                file.openReadAsync().then(function (stream) {
                    var reader = new Windows.Storage.Streams.DataReader(stream);
                    reader.inputStreamOptions = Windows.Storage.Streams.InputStreamOptions.partial;
                    reader.loadAsync(stream.size).then(function (size) {
                        var buf = new Uint8Array(size);
                        var s = reader.readBytes(buf);
                        var native = new Native.MimeParser();
                        var str = "&YhFmL1kpYk0-/&T2BmLw-SB";
                        native.decodeMBoxName_Sync(str);
                        //native.parseHeader_Sync(buf);
                       // console.log(s);
                    });
                });
            } else {
                // The picker was dismissed with no selected file
                WinJS.log && WinJS.log("Operation cancelled.", "sample", "status");
            }
        });
    }

    //;
    //console.log(native.test());

    app.start();
})();
