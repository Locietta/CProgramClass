# 16-10

## **teach.c** 答题系统

原程序的问题在于它重复了上一问题的解释或者这一问的描述,那么
只需要将描述和问题本身分开,分别处理即可.

## 具体的改动

1.在questionT中加入了qdescription成员,加入了符号常量MaxLinesPerDesciption.

2.增加了过程函数ReadQuestionDescription(),用于读入不需重复的问题描述,
并且以"-----"作为描述的结束标记.

3.相应地修改了ReadOneQuestion()和AskQuestion().使得他们符合新的输入样式,并且只在
第一次跳转时打印问题描述(跳转到自己的,将一直打印描述,如cs.dat中的问题6).

4.修改了cs.dat中的数据格式(在问题描述和问题内容之间加入了"-----"作为分割).
