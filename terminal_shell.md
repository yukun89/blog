# 终端(terminal，或者叫物理终端）


是一种设备，不是一个程序，一般说的就是能提供命令行用户界面的设备，典型的是屏幕和键盘，或其他的一些物理终端。

# 虚拟终端：
屏幕和键盘只是一个终端，可能不够用，又不想增加设备投入，就产生了虚拟终端。
gnome-terminal,urxvt，mlterm，xterm等等：
	是一个程序，职责是模拟终端设备，和虚拟终端的区别表面上在于它以 GUI 形式的窗口出现，内部则是程序结构和系统控制结构有所不同，但本质上差不多。

# 控制台（console)
显示系统消息的终端就叫控制台，Linux 默认所有虚拟终端都是控制台，都能显示系统消息。
但有时专指CLI下的模拟终端设备的一个程序，和gnome-terminal,urxvt，mlterm，xterm等相同，只是CLI和GUI界面的区别。一般console有6个，tty1-6，CTRL+ALT+fn切换。

# shell

是一个抽象概念，shell的一切操作都在计算机内部，负责处理人机交互，执行脚本等，是操作系统能正常运行的重要组成部分。

bash，ash，zsh，tcsh等是shell这个抽象概念的一种具体的实现，都是一个程序，都能生成一个进程对象。

如果想换shell的程序，可以修改/etc/passwd，把里面的/bin/bash换成你想要的shell，或者用chsh命令来切换。

shell有bash，ash，zsh，tcsh等等。


# shell与终端的关系

shell把一些信息适当的输送到终端设备，同时还接收来自终端设备的输入。一般每个shell进程都会有一个终端关联，也可以没有。

# 配色方案

终端显示的文字方案取决于两个：terminal 和 vim的配色。如果这两个配色方案不一致，可能会产生一些比较奇怪的效果。其中vim的配色方案在vimrc文件中的colorscheme选项有相关配置。

vim的配色方案：ls -l  /usr/share/vim/vim70/colors/

当然，与特定用户相关的配色方案：~/.vim/colors


