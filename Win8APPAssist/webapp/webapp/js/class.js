var Person = WinJS.Class.define(function (name, age) {
    // 构造函数 
    this._name = name;
    this._age = age;
    Person.count++;
}, {
    // 原型成员 
    say: function () {
        return 'I am ' + this._name + '. I am ' + this._age + ' years old.';
    },
    getCount:function(){
        this.constructor.getCount();
    }
}, {
    // 静态成员 
    getCount:function(){
        return this.count;
    },
    count: 0
});

var Student = WinJS.Class.derive(Person, function (name, age, grade) {
    // 调用父类的构造方法 
    Person.apply(this, arguments);
    this._grade = grade;
    Student.constructor = Person.constructor
    Student.count++;
}, {
    say: function () {
        return Person.prototype.say.apply(this, arguments) + ' I am grade ' + this._grade + '.';
    }
}, {
    getCount: Person.getCount
});

var jack = new Student('jack', 25, 4);

var a = { f: Util.F, b: 'ccc' }
