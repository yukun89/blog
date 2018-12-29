缓冲区原理简介：

go字节缓冲区底层以字节切片做存储，切片存在长度len与容量cap, 缓冲区写从长度len的位置开始写，当len>cap时，会自动扩容。缓冲区读会从内置标记off位置开始读(off始终记录读的起始位置)，当off==len时，表明缓冲区已全部读完并重置缓冲区(len=off=0),此外当将要内容长度+已写的长度(即len) <= cap/2时，缓冲区前移覆盖掉已读的内容(off=0，len-=off)，从避免缓冲区不断扩容。

eg:

```
package main

import (
    "bytes"
    "fmt"
)

func main() {
    byteSlice := make([]byte, 20) 
    byteSlice[0] = 1                                  // 将缓冲区第一个字节置1
    byteBuffer := bytes.NewBuffer(byteSlice)          // 创建20字节缓冲区 len = 20 off = 0
    c, _ := byteBuffer.ReadByte()                     // off+=1
    fmt.Printf("len:%d, c=%d\n", byteBuffer.Len(), c) // len = 20 off =1   打印c=1
    byteBuffer.Reset()                                // len = 0 off = 0
    fmt.Printf("len:%d\n", byteBuffer.Len())          // 打印len=0
    byteBuffer.Write([]byte("hello byte buffer"))     // 写缓冲区  len+=17
    fmt.Printf("len:%d\n", byteBuffer.Len())          // 打印len=17
    byteBuffer.Next(4)                                // 跳过4个字节 off+=4
    c, _ = byteBuffer.ReadByte()                      // 读第5个字节 off+=1
    fmt.Printf("第5个字节:%d\n", c)                    // 打印:111(对应字母o)    len=17 off=5
    byteBuffer.Truncate(3)                            // 将未字节数置为3        len=off+3=8   off=5
    fmt.Printf("len:%d\n", byteBuffer.Len())          // 打印len=3为未读字节数  上面len=8是底层切片长度
    byteBuffer.WriteByte(96)                          // len+=1=9 将y改成A
    byteBuffer.Next(3)                                // len=9 off+=3=8
    c, _ = byteBuffer.ReadByte()                      // off+=1=9    c=96
    fmt.Printf("第9个字节:%d\n", c)                    // 打印:96
}
```
go字节缓冲区ByteBuffer只能跳过未读的和丢弃已写的(即未读的)，操作会有点受限，如读已读的就无法操作。 但可使用ByteReader读缓冲区实现，使用该对象的seek函数。其余操作类似，简略。

func (b *Buffer) Read(p []byte) (n int, err error) {}
func (b *Buffer) ReadByte() (c byte, err error) {} 

* ReadBytes
ReadBytes需要一个byte作为分隔符，读的时候从缓冲器里找第一个出现的分隔符（delim），找到后，把从缓冲器头部开始到分隔符之间的所有byte进行返回，作为byte类型的slice，返回后，缓冲器也会空掉一部分

`func (b *Buffer) ReadBytes(delim byte) (line []byte, err error) {}`

```
var delim byte = 'L'
line, _ := bufs.ReadBytes(delim)
fmt.Println(bufs.String())
fmt.Println(string(line))
```


[ref](https://studygolang.com/articles/6224)


## string 和[]byte的互相转化
 string可以和byte的切片转换

1,string 转为[]byte

var str string = "test"

var data []byte = []byte(str)

 

2,byte转为string

var data [10]byte 

byte[0] = 'T'

byte[1] = 'E'

var str string = string(data[:])
