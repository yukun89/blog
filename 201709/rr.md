rr aspires to be your primary C/C++ debugging tool for Linux, replacing — well, enhancing — gdb. You record a failure once, then debug the recording, deterministically, as many times as you want. The same execution is replayed every time.

##Install and prepare.

###Install
Here is a good [ref](http://rr-project.org/).

###Prepare

* Gdb
* Package of debuginfo is required.
`yum install kernel-debuginfo kernel-debuginfo-common`
* Kernel version.
The latest version only works for kernel after RedHat(3.10.0-327.10.1.el7). Currenly node205 and node214 is not available for rr, only 
## How to use rr.

