1.建立工程
express -e PROJECT

2.下载依赖包

cd PROJECT;npm install

3.启动服务

npm start

本地3000端口被打开，浏览器输入localhost:3000，访问成功。

注：3000是express框架的默认端口，我们可以在bin/www中寻找到根据。

目前为止，我们可以分析程序框架：
	www:程序入口，完成app，server等的监听、绑定等操作。
	app:创建应用框架express实例，view与引擎相关设置，use相关的句柄设置。
	route:user,router：这里设定了两个路由，绑定了路径和句柄。

note:
	//routes.js
   router.get('/', function(req, res, next) {
     res.send('respond with a resource');
   });
//app.js
 54 app.use('login',routes);
 55 app.use('logout',routes);
 56 app.use('home',routes);
	以上分别是应用级中间件和路由级中间件；而我们往往需要将路由级中间件挂在到应用。
