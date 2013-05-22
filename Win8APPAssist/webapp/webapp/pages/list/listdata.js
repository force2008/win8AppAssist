(function () {
    "use strict";
    var templateElement;
    var templateItem =[];
    var Person1 = WinJS.Class.define(
            function () {
                this._initObservable();
                this.name = "Harry";
                this.color = "blue";
                this.timeout = null;
            }, {

                _nameArray:
                    new Array("Sally", "Jack", "Hal", "Heather", "Fred", "Paula", "Rick", "Megan", "Ann", "Sam"),
                _colorArray:
                    new Array("lime", "lavender", "yellow", "orange", "pink", "greenyellow", "white", "lightblue", "lightgreen", "lightyellow"),

                _newName: function () {
                    this.name = this._nameArray[this._randomizeValue()];
                    this.color = this._colorArray[this._randomizeValue()];
                    //this.setProperty("name", this._nameArray[this._randomizeValue()]);
                    //this.setProperty("color", this._colorArray[this._randomizeValue()]);
                },

                _randomizeValue: function () {
                    var value = Math.floor((Math.random() * 1000) % 8);
                    if (value < 0)
                        value = 0;
                    else if (value > 9)
                        value = 9;
                    return value;
                },

                start: function () {
                    var that = this;
                    if (this.timeout === null) {
                        this.timeout = setInterval(function () { that._newName(); }, 500);
                    }
                },

                stop: function () {
                    if (this.timeout !== null) {
                        clearInterval(this.timeout);
                        this.timeout = null;
                    }
                }
            });

    WinJS.Class.mix(Person1,
        WinJS.Binding.mixin,
        WinJS.Binding.expandProperties({ name: "", color: "" })
    );
    function onNameChange(newValue) {
        var span = document.getElementById("bindableSpan");
        span.innerText = newValue;
    }

    function onColorChange(newValue) {
        var span = document.getElementById("bindableSpan");
        span.style.color = newValue;
    }
    WinJS.UI.Pages.define("/pages/list/listdata.html", {
        // This function is called whenever a user navigates to this page. It
        // populates the page elements with the app's data.
        ready: function (element, options) {

            var myPerson = new Person1();
            myPerson.bind("name", onNameChange);
            myPerson.bind("color", onColorChange);
            myPerson.start();

            var Person = WinJS.Binding.define({
                name: "",
                color: "",
                birthday: "",
                petname: "",
                dessert: ""
            });

            this.people = [
                new Person({ name: "Bob", color: "red", birthday: "2/2/2002", petname: "Spot", dessert: "chocolate cake",list:[1,2,3] }),
                new Person({ name: "Sally", color: "green", birthday: "3/3/2003", petname: "Xena", dessert: "cherry pie", list: [1, 2, 3] }),
                new Person({ name: "Fred", color: "blue", birthday: "2/2/2002", petname: "Pablo", dessert: "ice cream", list: [1, 2, 3] }),
            ];
            this.selector = document.getElementById("templateControlObjectSelector");
            this.selector.addEventListener("change", function(evt) {
                templateItem = [];
                templateElement = document.getElementById("templateDiv");
                var renderElement = document.getElementById("templateControlRenderTarget");
                renderElement.innerHTML = "";

                var selected = evt.target.selectedIndex;
                var templateControl = templateElement.winControl;

                while (selected >= 0) {
                    var item = templateElement.winControl.render(this.people[selected--])
                        .done(function (result) {
                            renderElement.appendChild(result);
                        });
                    templateItem.push(item);
                }
            }.bind(this), false);

            document.getElementById('changeperson').addEventListener("click", this.changeData.bind(this), false);
        },
        changeData: function () {
            var counts = this.selector.selectedIndex;
            for(var i=0;i<=counts;i++)
                this.people[i].name = new Date + this.people[i].name;
        }
    });
})();
