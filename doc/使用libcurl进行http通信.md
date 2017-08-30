#使用libcurl进行http通信


## 1. 为什么用使用LibCurl

- 开源免费
- 代码跨平台的可移植
- 线程安全
- IpV6兼容，简单易于使用

-----------------------------------------------------
通常情况下，在嵌入式环境中，为了减少依赖，降低目标程序的大小，对于简单的Http请求，通过字符串拼接，构造出http协议字段，然后通过tcp协议发送出去，就能完成简单的http请求操作
但是当我们的需要更多更复杂的操作时，如添加一些特殊的报头，发送不同的文件，如果仅靠文本字符串拼接工作量会大大增加，代码也会变得复杂混乱，这时候使用libcurl库，虽然一定程度上增加了目标程序的大小，但是会使得我们的代码简单易读，也更加的易于维护
所以在实际应用当中，当http协议不再是简单的请求时候，还是非常推荐使用Libcurl库

-----------------------------------------------------

##2. 在嵌入式平台上的编译和定制

为了能在多种平台上运行，libcurl的开发者们做了大量的工作，所以在我们实际使用中，跨平台编译还是很简单的
只要注意
	1.  指定交叉编译器（CC），根据平台不同指定不同的交叉编译工具
	2.  指定make install时库的存放路径
	3.  去掉了其中的一些功能 ：如去掉ftp,https,samba等暂时无关的功能，可以只指定 --enable-http
	4.  使用静态编译：不需要拷贝动态库到目标机器，方便程序发布

-----------------------------------------------------

##3. libcurl 的使用
### 3.1 使用libcurl的基本流程
	
1. 调用curl_global_init()初始化libcurl
2. 调用curl_easy_init()函数得到 easy interface型指针
3. 调用curl_easy_setopt()设置传输选项
4. 根据curl_easy_setopt()设置的传输选项，实现回调函数以完成用户特定任务
5. 调用curl_easy_perform()函数完成传输任务
6. 调用curl_easy_cleanup()释放内存

### 3.2主要函数设置
**在整过过程中设置curl_easy_setopt()参数是最关键的**
常见的选项
	1.     CURLOPT_URL 
			设置访问URL
	2.       CURLOPT_WRITEFUNCTION，CURLOPT_WRITEDATA
			设置接受数据的回调函数和指针
	3.     CURLOPT_TIMEOUT，CURLOPT_CONNECTIONTIMEOUT:
			CURLOPT_TIMEOUT 由于设置传输时间，CURLOPT_CONNECTIONTIMEOUT 设置连接等待时间

###3.3 返回码
	
http常见的返回码有：
	```
	200 - 请求成功
	301 - 资源（网页等）被永久转移到其它URL
	404 - 请求的资源（网页等）不存在
	500 - 内部服务器错误
	```
	
> 这里要注意这和 curl_easy_perform 函数的返回是不一样的

 curl_easy_perform 的返回值的意义
	1.    CURLE_OK 
	    任务完成一切都好
	2     CURLE_UNSUPPORTED_PROTOCOL
    不支持的协议，由URL的头部指定
	3     CURLE_COULDNT_CONNECT
    不能连接到remote 主机或者代理
	4     CURLE_REMOTE_ACCESS_DENIED
    访问被拒绝
	5     CURLE_HTTP_RETURNED_ERROR
    Http返回错误
	6     CURLE_READ_ERROR
	
 > 要获取详细的错误描述字符串，可以通过const char *curl_easy_strerror(CURLcode errornum ) 这个函数取得

	
	要像获得http的返回码
	CURLcode curl_easy_getinfo(CURL *curl, CURLINFO info, ... )
	1.CURLINFO_RESPONSE_CODE



###3.4 HTTP 头
	
当使用libcurl发送http请求时，它会自动添加一些http头。
一方面他比起文本拼接的方法简单智能，另一方面可能也会添加了错误的或者我们不需要的头
我们可以通过CURLOPT_HTTPHEADER属性手动替换、添加或删除相应的HTTP消息头。
	 
	
Host

    http1.1（大部分http1.0)版本都要求客户端请求提供这个信息头。

Pragma

    "no-cache"。表示不要缓冲数据。

Accept

    "*/*"。表示允许接收任何类型的数据。

Expect

    以POST的方式向HTTP服务器提交请求时，libcurl会设置该消息头为"100-continue"，它要求服务器在正式处理该请求之前，返回一个"OK"消息。如果POST的数据很小，libcurl可能不会设置该消息头。

修改消息头
HTTP协议提供了消息头，请求消息头用于告诉服务器如何处理请求；响应消息头则告诉浏览器如何处理接收到的数据。在libcurl中，你可以自由的添加这些消息头：

```cpp
	struct curl_slist *headers=NULL; /* init to NULL is important */		
	headers = curl_slist_append(headers, "Hey-server-hey: how are you?");
	headers = curl_slist_append(headers, "X-silly-content: yes");
	/* pass our list of custom made headers */
	curl_easy_setopt(easyhandle, CURLOPT_HTTPHEADER, headers);
	curl_easy_perform(easyhandle); /* transfer http */
	curl_slist_free_all(headers); /* free the header list */
```
	
 对于已经存在的消息头，可以重新设置它的值：
 
```cpp
	headers = curl_slist_append(headers, "Accept: Agent-007");  
	headers = curl_slist_append(headers, "Host: munged.host.line"); 
```

删除消息头
对于一个已经存在的消息头，设置它的内容为空，libcurl在发送请求时就不会同时提交该消息头：
    
```cpp
	headers = curl_slist_append(headers, "Accept:"); 
```

--------------------------------------------

###3.5当使用libcurl POST 大数据的注意事项
当使用libcurl的POST方式时，如果POST数据的大小大于1024个字节（**典型的我们发送过车数据或者抓拍图片的时候**），
	libcurl不会直接发送POST请求，而是会分为两步执行请求：
	
	<1> 发送一个请求，该请求头部包含一个Expect： 100-continue的字段，用来询问server是否愿意接受数据
	<2> 当接收到从server返回的100-continue的应答后，它才会真正的发起POST请求，将数据发送给server

> 对于“100-continue"这个字段，RFC文档（http://www.w3.org/Protocols/rfc2616/rfc2616-sec8.html#sec8.2.3）是这么解释的：
它可以让客户端在发送请求数据之前去判断服务器是否愿意接收该数据，如果服务器愿意接收，客户端才会真正发送数据，
这么做的原因是如果客户端直接发送请求数据，但是服务器又将该请求拒绝的话，这种行为将带来很大的资源开销。

所以为了避免这种情况，libcurl在发送大于1024字节的POST请求时采用了这种方法，但是相对的，它会引起请求延迟的加大
另外并不是所有的server都会正确处理并且应答”100-continue“，比如lighttpd，就会返回417”Expectation Failed“，造成请求逻辑出错。

**解决办法:**

```cpp
// POST数据的大于1024个字节
struct curl_slist *headerlist = NULL;
static const char buf[] = "Expect:";
headerlist = curl_slist_append(headerlist, buf); /* initalize custom header list */
curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist); /* set header*/
curl_slist_free_all(headerlist); /* free slist
```

-----------------------------------------------------

##4. 出错排查

传输失败总是有原因的。你可能错误的设置了一些libcurl的属性或者没有正确的理解某些属性的含义，或者是远程主机返回一些无法被正确解析的内容。
典型的排查方法

 - 将**CURLOPT_VERBOSE**属性设置为1，libcurl会输出通信过程中的一些细节。如果使用的是http协议，请求头/响应头也会被输出。
 - 将**CURLOPT_HEADER**设为1，这些头信息将出现在消息的内容中。
 - 抓包分析

> 一个可能的错误是发送了服务器不支持的编码类型，比如有的服务器只支持utf-8的字符



##5.示例代码
> 实际项目中通过libcurl 发送 xml文本数据和 jpeg 图片文件的示例代码