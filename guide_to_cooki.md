##1.Cooki 背景：为什么要使用cooki？
1.cooki是为了解决什么问题？
弥补HTTP无状态的缺陷。例如，我们访问某个网站，点击*购物*，此时，需要用到用户account。虽然我们在登录的时候使用了account和password，但是一旦登录完成，对应的状态也就丢失了。也就是说，server端并不知道现在客户端是在登录状态还是已经logout——我们需要一种机制能够记录客户端目前的状态。

其中，一个常见的场景是**认证Cooki**： authentication cookies are the most common method used by web servers to know whether the user is logged in or not, and which account they are logged in with. 

>为何C-S模式下不会记录状态信息呢？在技术上，是可以一直保持状态信息的。但是，在大量并发请求等状况下，如果client和server需要将每次的连接都保存起来，这是对server端资源的巨大消耗。所以，我们把状态信息放到cooki中，通过http头放入http request之中。

##2.cooki的原理：what is cooki?
Cooki is a small piece of data sent from a website and stored in a user's web browser while the user is browsing that website. Every time the user loads the website, the browser sends the cookie back to the server to notify the website of the user's previous activity.
key word:是server发送，存储在客户端，需要浏览器的支持。

*cooki structure*
Name,value,attributes(任意多个),对于chrome，我们可以按照如下方式查看访问某个站点对应的cooki：点击浏览器地址栏中左侧的图标。

##3.Cooki 实践：how to use cooki?


Cookies are set using the HTTP Set-Cookie header, sent in an HTTP response. This header instructs the browser to store the cookie and send it back in future requests to the server (the browser will, of course, ignore this header if it does not support cookies or has disabled cookies).

As an example, the browser sends its first request to the homepage of the www.example.org website:

>GET /index.html HTTP/1.1
Host: www.example.org
...

The server responds with two Set-Cookie headers:

>HTTP/1.0 200 OK
Content-type: text/html
Set-Cookie: theme=light
Set-Cookie: sessionToken=abc123; Expires=Wed, 09 Jun 2021 10:18:14 GMT
...

The server's HTTP response contains the contents of the website's homepage. But it also instructs the browser to set two cookies. The first, "theme", is considered to be a "session" cookie, since it does not have an Expires or Max-Age attribute. Session cookies are typically deleted by the browser when the browser closes. The second, "sessionToken" contains an "Expires" attribute, which instructs the browser to delete the cookie at a specific date and time.

Next, the browser sends another request to visit the spec.html page on the website. This request contains a Cookie header, which contains the two cookies that the server instructed the browser to set.

>GET /spec.html HTTP/1.1
Host: www.example.org
Cookie: theme=light; sessionToken=abc123
...

This way, the server knows that this request is related to the previous one. The server would answer by sending the requested page, and possibly adding other cookies as well using the Set-Cookie header.

The value of a cookie can be modified by the server by including a Set-Cookie header in response to a page request. The browser then replaces the old value with the new value.

The term "cookie crumb" is sometimes used to refer to a cookie's name-value pair.[25]

Cookies can also be set by scripting languages such as JavaScript that run within the browser. In JavaScript, the object document.cookie is used for this purpose. For example, the instruction document.cookie = "temperature=20" creates a cookie of name "temperature" and value "20".

##4.相似技术对比:session

