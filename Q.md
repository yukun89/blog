　一个promise是一个带有".then()"方法的对象，其代表的是一个操作的结果可能还没有或不知道，无论谁访问这个对象，都能够使用".then()"方法加入回调等待操作出现成功结果或失败时的提醒通知.

promise是一个异步编程的抽象，它是一个返回值或抛出exception的代理对象，一般promise对象都有一个then方法，这个then方法是我们如何获得返回值(成功实现承诺的结果值，称为fulfillment)或抛出exception(拒绝承诺的理由，称为rejection)，then是用两个可选的回调作为参数，我们可以称为onFulfilled和OnRejected：

var promise = doSomethingAync()
promise.then(onFulfilled, onRejected)

onFulfilled && onRejected 是两个函数，分别promise执行成功和失败后调用的方法；其中两个方法的参数分别是result对象和error。
