# 单实例 deamon

* 通过pid文件上锁的方式来实现程序的单实例运行

```cpp

if(already_running(PID_FILE)) {
	//already_running !!!
}

```

* 在daemon的实现上

```cpp
int daemonize_user();	//保留了标准出错
int daemonize_sys();	//直接调用daemon(0,0) 标准出错也关闭了
```
