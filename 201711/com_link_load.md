摘要：本文主要讲解elf文件格式，通过readelf命令结合底层的相关数据结构，讲解相关内容，分析程序运行的基本原理。
本文来源：elf 文件格式探秘——程序运行背后的故事 http://blog.csdn.net/trochiluses/article/details/10373921

## 1.elf 文件格式概览

elf文件大体上由文件头和相关的section组成，而每个section由header和data组成。总体而言，ELF文件由四部分构成:

* ELF Header
* Program Header table
* sections
* section header table.

![elf_header](http://ozsslej69.bkt.clouddn.com/elf_layout.png)

## 2.elf文件头

文件头的数据结构:
```
   typedef struct {
         unsigned char e_ident[EI_NIDENT];
         Elf32_Half    e_type;
         Elf32_Half    e_machine;
         Elf32_Word    e_version;
         Elf32_Addr    e_entry;
         Elf32_Off     e_phoff;
         Elf32_Off     e_shoff;
         Elf32_Word    e_flags;
         Elf32_Half    e_ehsize;
         Elf32_Half    e_phentsize;
         Elf32_Half    e_phnum;
         Elf32_Half    e_shentsize;
         Elf32_Half    e_shnum;
         Elf32_Half    e_shstrndx;
    } Elf32_Ehdr;
```
使用readelf命令查看文件头:

>$ readelf -h /bin/ls
  1 ELF Header:
  2   Magic:   7f 45 4c 46 01 01 01 00 00 00 00 00 00 00 00 00
  3   Class:                             ELF32
  4   Data:                              2's complement, little endian
  5   Version:                           1 (current)
  6   OS/ABI:                            UNIX - System V
  7   ABI Version:                       0
  8   Type:                              EXEC (Executable file)
  9   Machine:                           Intel 80386
 10   Version:                           0x1
 11   Entry point address:               0x804be34
 12   Start of program headers:          52 (bytes into file)
 13   Start of section headers:          103388 (bytes into file)
 14   Flags:                             0x0
 15   Size of this header:               52 (bytes)
 16   Size of program headers:           32 (bytes)
 17   Number of program headers:         9
 18   Size of section headers:           40 (bytes)
 19   Number of section headers:         28
 20   Section header string table index: 27

在 readelf 的输出中, 比较重要的有以下几行.
第 8 行，Type 表示文件类型。ELF 文件有 3 种类型，一种是如上所示的 Relocatable file 可重定位目标文件，一种是可执行文件(Executable)，另外一种是共享库(Shared Library) .
第 11 行，程序的虚拟地址入口点.
第 20 行，section 头字符串表索引号，从 Section Headers 输出部分可以看到其内容的偏移在 0xa0 处，从此处开始到0xcf 结束保存着各个 sections 的名字，如 .data，.text，.bss等。

 链接器从 .rel.text  就可以知道哪些地方需要进行重定位(relocate) 。
.symtab 是符号表。
 Ndx 是符号表所在的 section 的 section header 编号。如 .data 段的 section header 编号是 3，而string1，string2，lenght 都是在 .data 段的。


## 3.符号表和重定位

符号表是符号和地址的映射，主要用于链接：如果有一个外部申明extern in function，我们引用了function，那么连接器就要查找function对应的地址，从而来调用它。和符号表紧密相关的是重定位，我们可以在“动态链接”这一节详细理解什么是重定位。

## 4.section and segments


elf文件格式制定了elf文件的两种视角：链接和执行。这样给系统的设计者提供了很大的灵活性。我们接下来讲解目标文件中的section，这些section将被链接成可执行文件。一个或者多个section被映射到一个segment里，segment属于可执行文的一部分。section可以说是link角度的概念；segment是执行角度的概念.

### 4.1 segments

如同我们在前面提到的那样，elf文件除了包含整体的文件头以外，还包含各个部分的program headers，这些program headers向操作系统描述了程序load和execute所需要的一切信息，它描述的是系统准备程序运行所需要的一个段和其他信息。联系内存的段式管理，同一个段的属性是相同的，下面是一个传统的program headers的数据结构.

```
typedef struct {
          Elf32_Word p_type;
          Elf32_Off  p_offset;
          Elf32_Addr p_vaddr;
          Elf32_Addr p_paddr;
          Elf32_Word p_filesz;
          Elf32_Word p_memsz;
          Elf32_Word p_flags;
          Elf32_Word p_align;
    }
```

在elf总体的header里面，e_phoff, e_phnum and e_phentsize表示program headers的偏移，数量和大小，有了这些信息我们可以轻易知道program headers的相关情况，从而利用它们。如同上面提到的那样，program headers并不仅仅是segments信息。

* p_type定义了program header的类型（PT_INTERP表明这个program header是一个string类型的指针，指向针对一个二进制文件的解释器）。我们原来对比过编译型语言和解释型语言，而且作出了以下区分：编译器建立了一个可以独立运行的二进制文件。那么为什么它需要一个解释器呢？一般而言，实际情况下更为复杂：现代系统装载可执行文件需要很大的灵活性，为了达到这一目的，一些信息只能在程序运行的时候获取。我们将在动态链接一章里解释这个问题。因此，需要对二进制文件做出一些小的更改以让它能够在实时运转的时候正常工作。因此，实际情况下的二进制解释器是一个动态装载器，它用一些列步骤完成可执行文件的加载和运行.

* p_offset相对首地址的偏移
* p_vaddr虚拟地址（一些嵌入式系统中没有实现虚拟内存，那么它等于p_paddr
* p_paddr物理内存地址
* p_filesz文件大小
* p_memsz实际在内存中占的空间大小，一般大于前者，多余部分用0填充;
* p_flags读写可执行权限位
* p_align对齐.
好了，我们现在知道，有了段在文件中的地址和偏移、大小；又有了虚拟地址；这提供了把一个elf文件加载进入内存的必要信息。所以，我们在加载的时候需要根据每个program header，进行相关的内存复制工作。

看例子：
>$ readelf -l /bin/ls
>
Elf file type is EXEC (Executable file)
Entry point 0x804be34
There are 9 program headers, starting at offset 52
>
Program Headers:
  Type           Offset   VirtAddr   PhysAddr   FileSiz MemSiz  Flg Align
  PHDR           0x000034 0x08048034 0x08048034 0x00120 0x00120 R E 0x4
  INTERP         0x000154 0x08048154 0x08048154 0x00013 0x00013 R   0x1
      [Requesting program interpreter: /lib/ld-linux.so.2]
  LOAD           0x000000 0x08048000 0x08048000 0x18d24 0x18d24 R E 0x1000
  LOAD           0x018ef8 0x08061ef8 0x08061ef8 0x003e8 0x01008 RW  0x1000
  DYNAMIC        0x018f0c 0x08061f0c 0x08061f0c 0x000e0 0x000e0 RW  0x4
  NOTE           0x000168 0x08048168 0x08048168 0x00044 0x00044 R   0x4
  GNU_EH_FRAME   0x015f7c 0x0805df7c 0x0805df7c 0x006f4 0x006f4 R   0x4
  GNU_STACK      0x000000 0x00000000 0x00000000 0x00000 0x00000 RW  0x4
  GNU_RELRO      0x018ef8 0x08061ef8 0x08061ef8 0x00108 0x00108 R   0x1
>
 Section to Segment mapping:
  Segment Sections...
   00
   01     .interp 
   02     .interp .note.ABI-tag .note.gnu.build-id .gnu.hash .dynsym .dynstr .gnu.version .gnu.version_r .rel.dyn .rel.plt .init .plt .text .fini .rodata .eh_frame_hdr .eh_frame 
   03     .ctors .dtors .jcr .dynamic .got .got.plt .data .bss 
   04     .dynamic 
   05     .note.ABI-tag .note.gnu.build-id 
   06     .eh_frame_hdr 
   07
   08     .ctors .dtors .jcr .dynamic .got 

上述各段组成了最终在内存中执行的程序，其还提供了各段在虚拟地址空间和物理地址空间中的大小、位置、标志、访问授权和对齐方面的信息。各段语义如下：

* PHDR 保存程序头表
* INTERP 指定程序从可行性文件映射到内存之后，必须调用的解释器，它是通过链接其他库来满足未解析的引用，用于在虚拟地址空间中插入程序运行所需的动态库。
* LOAD 表示一个需要从二进制文件映射到虚拟地址空间的段，其中保存了常量数据（如字符串），程序目标代码等。
* DYNAMIC段保存了由动态连接器（即INTERP段中指定的解释器）使用的信息。

### 4.2 sections

多个section组成segment。section将二进制文件[注意,section是文件的角度，不是内存的角度]组织成逻辑区域，用于在编译器和链接器之间交流信息。section的头部和segment有些类似：
```
    typedef struct {
            Elf32_Word sh_name;
            Elf32_Word sh_type;
            Elf32_Word sh_flags;
            Elf32_Addr sh_addr;
            Elf32_Off  sh_offset;
            Elf32_Word sh_size;
            Elf32_Word sh_link;
            Elf32_Word sh_info;
              Elf32_Word sh_addralign;
              Elf32_Word sh_entsize;
    }
```

sh_type具有更多的类型：SH_PROGBITS表明这是程序将要使用的数据的seciton，其他的值表示是否它是一个符号表等等。allocate属性表示这个section需要被分配内存。接下来，看一个实例：

```
#include <stdio.h>
int big_big_array[10*1024*1024];
char *a_string = "Hello, World!";
int a_var_with_value = 0x100;
int main(void)
{
	big_big_array[0] = 100;
	printf("%s\n", a_string);
	a_var_with_value += 20;
}
```

编译后执行：

>$ readelf --all ./sections
    ELF Header:
     ...
      Size of section headers:           40 (bytes)
      Number of section headers:         37
      Section header string table index: 34
    Section Headers:
    [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al
    [ 0]                   NULL            00000000 000000 000000 00      0   0  0
    [ 1] .interp           PROGBITS        10000114 000114 00000d 00   A  0   0  1
    [ 2] .note.ABI-tag     NOTE            10000124 000124 000020 00   A  0   0  4
    [ 3] .hash             HASH            10000144 000144 00002c 04   A  4   0  4
    [ 4] .dynsym           DYNSYM          10000170 000170 000060 10   A  5   1  4
    [ 5] .dynstr           STRTAB          100001d0 0001d0 00005e 00   A  0   0  1
    [ 6] .gnu.version      VERSYM          1000022e 00022e 00000c 02   A  4   0  2
    [ 7] .gnu.version_r    VERNEED         1000023c 00023c 000020 00   A  5   1  4
    [ 8] .rela.dyn         RELA            1000025c 00025c 00000c 0c   A  4   0  4
    [ 9] .rela.plt         RELA            10000268 000268 000018 0c   A  4  25  4
    [10] .init             PROGBITS        10000280 000280 000028 00  AX  0   0  4
    [11] .text             PROGBITS        100002b0 0002b0 000560 00  AX  0   0 16
    [12] .fini             PROGBITS        10000810 000810 000020 00  AX  0   0  4
    [13] .rodata           PROGBITS        10000830 000830 000024 00   A  0   0  4
    [14] .sdata2           PROGBITS        10000854 000854 000000 00   A  0   0  4
    [15] .eh_frame         PROGBITS        10000854 000854 000004 00   A  0   0  4
    [16] .ctors            PROGBITS        10010858 000858 000008 00  WA  0   0  4
    [17] .dtors            PROGBITS        10010860 000860 000008 00  WA  0   0  4
    [18] .jcr              PROGBITS        10010868 000868 000004 00  WA  0   0  4
    [19] .got2             PROGBITS        1001086c 00086c 000010 00  WA  0   0  1
    [20] .dynamic          DYNAMIC         1001087c 00087c 0000c8 08  WA  5   0  4
    [21] .data             PROGBITS        10010944 000944 000008 00  WA  0   0  4
    [22] .got              PROGBITS        1001094c 00094c 000014 04 WAX  0   0  4
    [23] .sdata            PROGBITS        10010960 000960 000008 00  WA  0   0  4
    [24] .sbss             NOBITS          10010968 000968 000000 00  WA  0   0  1
    [25] .plt              NOBITS          10010968 000968 000060 00 WAX  0   0  4
    [26] .bss              NOBITS          100109c8 000968 2800004 00  WA  0   0  4
    [27] .comment          PROGBITS        00000000 000968 00018f 00      0   0  1
    [28] .debug_aranges    PROGBITS        00000000 000af8 000078 00      0   0  8
    [29] .debug_pubnames   PROGBITS        00000000 000b70 000025 00      0   0  1
    [30] .debug_info       PROGBITS        00000000 000b95 0002e5 00      0   0  1
    [31] .debug_abbrev     PROGBITS        00000000 000e7a 000076 00      0   0  1
    [32] .debug_line       PROGBITS        00000000 000ef0 0001de 00      0   0  1
    [33] .debug_str        PROGBITS        00000000 0010ce 0000f0 01  MS  0   0  1
    [34] .shstrtab         STRTAB          00000000 0011be 00013b 00      0   0  1
    [35] .symtab           SYMTAB          00000000 0018c4 000c90 10     36  65  4
    [36] .strtab           STRTAB          00000000 002554 000909 00      0   0  1
  Key to Flags:
    W (write), A (alloc), X (execute), M (merge), S (strings)
    I (info), L (link order), G (group), x (unknown)
    O (extra OS processing required) o (OS specific), p (processor specific)
  There are no section groups in this file.
   ...
  Symbol table '.symtab' contains 201 entries:
     Num:    Value  Size Type    Bind   Vis      Ndx Name
  ...
      99: 100109cc 0x2800000 OBJECT  GLOBAL DEFAULT   26 big_big_array
  ...
     110: 10010960     4 OBJECT  GLOBAL DEFAULT   23 a_string
  ...
     130: 10010964     4 OBJECT  GLOBAL DEFAULT   23 a_var_with_value
  ...
     144: 10000430    96 FUNC    GLOBAL DEFAULT   11 main

* type 说明
PROGBITS(程序必须解释的信息，如二进制代码)，
STRTAB用于存储与ELF格式有关的字符串，但与程序没有直接关联，如各个节的名称(.text, .comment)
* bss段
	首先，来看big_big_array变量：在0x100109cc处，我们可以看见它在.bss这个段中（因为0x100109cc是.bss开始的地方），注意.bss的属性nobits，表明它在磁盘上是不占用数据存储空间的。
* text段
	main函数，注意main函数在.text段中。

>关于程序入口——不是main的地址:
    $ /bin/ls
    main is : 0x10000430
    $ readelf --headers /bin/ls | grep 'Entry point'
      Entry point address:               0x100002b0
    $ objdump --disassemble /bin/ls | grep 100002b0
 	100002b0 <_start>:
    100002b0:       7c 29 0b 78     mr      r9,r1
实际上，程序入口地址是“_start”，而这个地址空间和程序运行时候的地址空间是独立的。 

* data
	保存初始化过的数据，这是普通程序数据的一部分，可以在程序运行期间修改。
* rodata
	保存了只读数据，可以读取但不能修改，例如printf语句中的所有静态字符串封装到该节。
* init 和 fini
	保存了进程初始化和结束所用的代码，这通常是由编译器自动添加的。
* hash是一个散列表，允许在不对全表元素进行线性搜索的情况下，快速访问所有符号表项。

## 5.readelf常用选项

```
  readelf [-a|--all]
               [-h|--file-header]
               [-l|--program-headers|--segments]
               [-S|--section-headers|--sections]
               [-s|--syms|--symbols]
               elffile...
```

参考文献：
[1]Chapter 8. Behind the process coderwall.com
[2]ELF格式文件符号表全解析及readelf命令使用方法 http://blog.csdn.net/edonlii/article/details/8779075
