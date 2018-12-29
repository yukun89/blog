golang's log模块主要提供了3类接口。分别是 “Print 、Panic 、Fatal ”，对每一类接口其提供了3中调用方式，分别是 "Xxxx 、 Xxxxln 、Xxxxf"

对于 log.Fatal 接口，会先将日志内容打印到标准输出，接着调用系统的 os.exit(1) 接口，退出程序并返回状态 1 。但是有一点需要注意，由于是直接调用系统接口退出，defer函数不会被调用.

对于log.Panic接口，该函数把日志内容刷到标准错误后调用 panic 函数. 在Panic之后声明的defer是不会执行的。

 你也可以自定义Logger类型， log.Logger提供了一个New方法用来创建对象。

`func New(out io.Writer, prefix string, flag int) *Logger`

该函数一共有三个参数：

（1）输出位置out，是一个io.Writer对象，该对象可以是一个文件也可以是实现了该接口的对象。通常我们可以用这个来指定日志输出到哪个文件。
（2）prefix 我们在前面已经看到，就是在日志内容前面的东西。我们可以将其置为 "[Info]" 、 "[Warning]"等来帮助区分日志级别。
（3） flags 是一个选项，显示日志开头的东西，可选的值有：

```
Ldate         = 1 << iota     // 形如 2009/01/23 的日期
Ltime                         // 形如 01:23:23   的时间
Lmicroseconds                 // 形如 01:23:23.123123   的时间
Llongfile                     // 全路径文件名和行号: /a/b/c/d.go:23 
Lshortfile                    // 文件名和行号: d.go:23
LstdFlags     = Ldate | Ltime // 日期和时间
```
eg:

```
package main
import (
    "log"
    "os"
)
func main(){
    fileName := "Info_First.log"
    logFile,err  := os.Create(fileName)
    defer logFile.Close()
    if err != nil {
        log.Fatalln("open file error")
    }
    debugLog := log.New(logFile,"[Info]",log.Llongfile)
    debugLog.Println("A Info message here")
    debugLog.SetPrefix("[Debug]")
    debugLog.Println("A Debug Message here ")
}
```
