执行文件在bin 目录下

（一）服务器
1）启动方式。执行ftpserver [IP地址] port启动FTP服务。其中IP地址为可选项。若不提供服务器IP地址，FTP服务将自动绑定到本地网络的IP。

（二）客户端
登录管理。客户端在shell中输入myftp username:passwd@ftp-server-ip:ftp-server-port登录ftpserver。


然后在客户端的shell下可以测试ls,cd,pwd,mkdir等命令
