1. 修改named.conf 配置服务地址和请求地址

2. 修改named.rfc1912.zones 添加一个我们的zone文件

3. 在/var/named/目录下新建一个自己的 zone 文件(正向解析)，并修改所属为named

4. 修改正向解析文件，并重启服务 service named restart

-------------------------


示例解析文件说明
* wwww.fengfan.gov.cn		解析为 1.1.1.1
* mail.fengfan.gov.cn		解析为 1.1.1.1
* web1.fengfan.gov.cn		解析为 1.1.1.1
* web2.fengfan.gov.cn		解析为 1.1.1.2
* web.fengfan.gov.cn		的别名为 web1.fengfan.gov.cn
