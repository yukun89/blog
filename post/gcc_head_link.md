gcc指定头文件及动态链接库路径
在利用源代码进行软件编译的过程中，经常会出现以下错误：
*  undefined reference to 'crypt'
*  libdl.so: error adding symbols: DSO missing from command line
*  openssl/rsa.h: no such file or directory
这些都是软件编译、链接过程中找到不相关头文件或者动态链接库导致的。本文主要讲解此类问题的产生原因，解决办法和gcc编译过程中的常见其他问题。

##1. 头文件路径

>#include <>直接到系统指定的某些目录中去找某些头文件。
#include “”先到源文件所在文件夹去找，然后再到系统指定的某些目录中去找某些头文件。

那么gcc如何确定头文件搜索路径呢？
* 默认路径`/usr/include`
* 命令行指定`gcc -I SOMEPATH`

**头文件路径有多种，那么先后顺序如何？**

1. -I 指定的路径
2. gcc 环境变量指定的路径`C_INCLUDE_PATH, CPLUS_INCLUDE_PATH, OBJC_INCLUDE_PATH`
3. 上述指定目录`/usr/include`

###常见问题

**openssl/rsa.h: no such file or directory**
这种错误，是缺少相关头文件导致的，通常情况下，运行`apt-get install libopenssl-dev`即可解决（缺少其他头文件同理）。

----
##2. 动态链接库路径

众所周知，Linux动态库的默认搜索路径是/lib和/usr/lib。动态库被创建后，一般都复制到这两个目录中。当程序执行时需要某动态库， 并且该动态库还未加载到内存中，则系统会自动到这两个默认搜索路径中去查找相应的动态库文件，然后加载该文件到内存中。linux除了默认的搜索路径之外，还通过以下三种方法指定：

1. 配置文件/etc/ld.so.conf (编辑之后，需要运行命令ldconf来使得更改生效)
2. 通过环境变量LD_LIBRARY_PATH指定动态库搜索路径。
3. 编译目标代码时，指定动态搜索路径`-Wl,-rpath`

以上，我们一共有5个路径指定方式，它们的先后顺序是：编译时指定 > 环境变量 > 配置文件 > 默认路径

*tips*

**共享库如何命名？**
在linux系统中，共享库的命名方式是`libNAME.so`.

###常见问题

**librte_eal.a(eal.o): undefined reference to symbol 'pthread_create@@GLIBC_2.2.5' /lib/x86_64-linux-gnu/libpthread.so.0: error adding symbols: DSO missing from command line)**
这个问题比较特殊，ld找不到对应symbol不是由于对应so不在路径中，而是`DSO missing from command line`.
**解决方法**：链接阶段，指定使用的动态库:`-lpthread`

**Q:为何需要显式指定某个动态链接库？**
解答：从ubuntu11.04之后，ld的机制发生了变化。对于这样的场景：
>function spin: included in libwheel;
libcar used libwheel;
program pro used libcar;

*在原来*，pro是直接可以引用spin函数——即使它并没有use libwheel；但是这里有一个潜在的风险：一旦libcar不再use libwheel，这个依赖的传递性就会被打破，如此将产生链接错误。所以，`ld now runs with the --no-copy-dt-needed-entries option enabled by default.`。必须显式指定动态链接的函数所在的库。

详细解释可以参考(https://wiki.ubuntu.com/NattyNarwhal/ToolchainTransition)
