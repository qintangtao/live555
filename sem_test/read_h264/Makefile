# 简化第一版
# OBJS   	代替  依赖文件
# CC     	代替  gcc
# CFLAGS 	代替  编译命令

# 简化第二版
# $^ 		代替 构造所需文件列表所有文件的名字
# $< 		代替 构造所需文件列表第一个文件的名字
# RM 		代替 rm -f
# $@ 		代替 目标文件

# 简化第三版
# 6-11行代码相似性很强，可以提取出一个公式模版
#      %.o:%.c
#	       $(CC) $^ $(CFLAGS)  -o $@
# 百分号相当于一个通配符



CC = arm-hisiv200-linux-gcc
CFLAGS=-c -Wall -g
OBJS=main.o sem_comm.o

read_h264:$(OBJS)
	$(CC) $^ -o $@

%.o:%.c
	$(CC) $< $(CFLAGS) -o $@

#main.o:main.c
#	$(CC) $^ $(CFLAGS) -o $@
#sem_comm.o:sem_comm.c
#	$(CC) $^ $(CFLAGS) -o $@


clean:
	$(RM) *.o read_h264 -r