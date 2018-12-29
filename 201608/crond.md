## 0.cron简介
工具是一个以系统服务的形式存在。通过/etc/rc.d/init.d 或者 /etc/init.d目录下的启动脚本crond来启动。 也可能直接通过 service crond start 命令来启动。

cron 会从如下这些地方读取安装的任务：
1>. /etc/anacrontab
这个文件存着系统级的任务。它主要用来运行每日的(daily)，每周的(weekly),每月的(monthly)的任务。一般 不在此文件安装自己的任务。以下是/etc/anacrontab内容的一个例子：
```
# /etc/anacrontab: configuration file for anacron
# See anacron(8) and anacrontab(5) for details.
SHELL=/bin/sh
PATH=/sbin:/bin:/usr/sbin:/usr/bin
MAILTO=root
# the maximal random delay added to the base delay of the jobs
RANDOM_DELAY=45
# the jobs will be started during the following hours only
START_HOURS_RANGE=3-22
#period in days   delay in minutes   job-identifier   command
1       5       cron.daily              nice run-parts /etc/cron.daily
7       25      cron.weekly             nice run-parts /etc/cron.weekly
@monthly 45     cron.monthly            nice run-parts /etc/cron.monthly
```
2>. /etc/cron.d/
此目录下存放的是系统级任务的任务文件。
`0hourly  raid-check  sysstat`

3>. /var/spool/cron/
此目录下存放各个用户的任务文件。各个用户的任务存放在以自已用户名为文件名的任务文件中。此文件中的指令行没有用户域。


## 1.Crontab约束
当用户名记录在文件/usr/lib/cron/cron.allow时，该用户的crontab有效。当/usr/lib/cron/cron.allow文件不存在时，用户名没被记录在/usr/lib/cron/cron.deny文件中，该用户的crontab也有效。当只存在/usr/lib/cron/cron.deny且为空时，所有的用户都可使用crontab。当前两个文件都不存在时，只有root用户可以使用crontab。Deny或allow记录以每行一个用户名的形式记录信息。

## 2.     Crontab 命令

export EDITOR=vi ;为crontab文件指定一个编辑器。　
crontab -e 　编辑该用户的crontab，当指定crontab 不存在时新建。 　
crontab -l 　列出该用户的crontab。 　
crontab -r 　删除该用户的crontab。 　
crontab -u<用户名称> 　指定要设定crontab的用户名称。
crontab –v 显示上一次编辑的时间（只在某些操作系统上可用）

## 3.      Crontab 文件
Crontab语法一个crontab文件用五个段来定义：天，日期和时间，和一个要定期执行的命令代码。
```
*    *  *  *   *  command to be executed
-    -   -   -    -
|    |    |    |    |
|    |    |    |    +----- day of week (0 - 6) (Sunday=0)
|    |    |    +------- month (1 - 12)
|    |    +--------- day of month (1 - 31)
|    +----------- hour (0 - 23)
+------------- min (0 - 59)
``` 
所有的值都必须在相应的范围之内，否则视为无效。在填值区域内可以是*也可以是以”，”分隔的一组值。值可以是一个数据也可以是用连接符连起来的两个数(表示范围)。注：日期的格式可以是星期，也可以是一个月中的天。假如两个都有值，则在这两个时间都会执行。

## 4. Crontab 实例 
下面是crontab文件中的一行，意思是在每天下午6:30中删除临时文件(临时文件存放在/home/someuser/tmp)。
`30    18    *    *    *        rm /home/someuser/tmp/*`

