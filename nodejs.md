#nodejs 入门指南
nodejs是基于javascript的，本文主要的目的是让初学者能够快速入门nodejs，能够看懂基本的nodejs程序。后续的文章将持续介绍nodejs的几个实用事例。

#变量
var a=1
*1.先声明，后使用 2.弱类型*

#循环

	for(int i = 0; i < foo; i++)
	{
	}

	for(var key in foo) {
		console.log(key + ": " + foo[key]);
	}

#json object

	var dog = {
	  "pre" : "嘘~",
	  "sub" : {
		"name" : "蛋花汤",
		"act"  : "在睡觉",
		"time" : 12
		},
	  "suf" : [ "我说了", "它在睡觉", "就是在睡觉" ]
	};
	dog["pre"], dog.pre

#类对象

#组织
一个文件就是一个模块，而文件路径就是模块名。require、exports、module三个预先定义好的变量可以在编写模块的时候使用。

require相当于import；
exports相当于public等，用于模块导出；

