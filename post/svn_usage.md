##1. svn st --help
status (stat, st): 显示工作副本中目录与文件的状态。

>用法: status [PATH...]
  未指定参数时，只显示本地修改的条目(没有网络访问)。
  使用 -q 时，只显示本地修改条目的摘要信息。
  使用 -u 时，增加工作版本和服务器上版本过期信息。
  使用 -v 时，显示每个条目的完整版本信息。
问号表示不在版本控制范围之内

【?：不在svn的控制中；M：内容被修改；C：发生冲突；A：预定加入到版本库；K：被锁定】

##2. svn update如果后面没有目录，默认将当前目录以及子目录下的所有文件都更新到最新版本。
svn update -r 200 test.php(将版本库中的文件test.php还原到版本200)
svn update test.php(更新，于版本库同步。如果在提交的时候提示过期的话，是因为冲突，需要先update，修改文件，然后清除svn resolved，最后再提交commit)
svn d

>update的结果：
U:表示从服务器收到文件更新了
G:表示本地文件以及服务器文件都已更新,而且成功的合并了 
其他的如下:
A:表示有文件或者目录添加到工作目录
R:表示文件或者目录被替换了.
C:表示文件的本地修改和服务器修改发生冲突


##3. 删除文件
svn delete path -m “delete test fle“
例如：svn delete svn://192.168.1.1/pro/domain/test.php -m “delete test file”
或者直接svn delete test.php 然后再svn ci -m ‘delete test file‘，推荐使用这种
简写：svn (del, remove, rm)

##4. 查看日志
svn log path
例如：svn log test.php 显示这个文件的所有修改记录，及其版本号的变化

##5. 查看文件详细信息
svn info path
例如：svn info test.php

##6. 比较差异
svn diff path(将修改的文件与基础版本比较)
例如：svn diff test.php
svn diff -r m:n path(对版本m和版本n比较差异)
例如：svn diff -r 200:201 test.php
简写：svn di

##7. 将两个版本之间的差异合并到当前文件
svn merge -r m:n path
例如：svn merge -r 200:205 test.php（将版本200与205之间的差异合并到当前文件，但是一般都会产生冲突，需要处理一下）

##8. 撤销本地修改
svn revert path
