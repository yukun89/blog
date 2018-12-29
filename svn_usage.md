svn merge
svn merge是合并两个分支，有四种应用场景。
1) trunk中的全部修改，合并到branch
svn merge ^/trunk BRANCH_WD
2) 将trunk中，特定版本的修改，合并到branch
svn merge -c version_number ^/trunk BRANCH-WD

或者使用另外的方法：
* 生成diff文件
* 打补丁：patch -p0 < file.diff


