# 16-2

## 一个关于图书的数据库

实现了一个library.h，位于include文件夹，相应的.c文件在lib文件夹里

只有增和查，但基本上满足了题目的需求了(删和改其实也差不多...之后有空再写吧)

测试数据存放在testdata文件里

使用makefile组织编译，测试终端是Git Bash和PowerShell

测试结果图片在TestPic文件夹里

还有改动了教材库genlib.h中bool类型的实现，换成了用stdbool.h，不然一直谜之warning(
