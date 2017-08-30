# 为什么用使用LibCurl



# 传统的嵌入式http请求如何发送，优劣


# 在嵌入式平台上的编译和定制
	CC指定编译器，--prefix=/home/hank/http/hisi_http指定make install时库的存放路径，这里编译成静态库，去掉了其中的一些功能。

# 使用
http://www.cnblogs.com/lidabo/p/4159589.html
	libcurl默认以GET方式提交请求：
	libcurl使用的HTTP消息头

     当使用libcurl发送http请求时，它会自动添加一些http头。我们可以通过CURLOPT_HTTPHEADER属性手动替换、添加或删除相应的HTTP消息头。

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
    ```c
	struct curl_slist *headers=NULL; /* init to NULL is important */		
	headers = curl_slist_append(headers, "Hey-server-hey: how are you?");
	headers = curl_slist_append(headers, "X-silly-content: yes");
	/* pass our list of custom made headers */
	curl_easy_setopt(easyhandle, CURLOPT_HTTPHEADER, headers);
	curl_easy_perform(easyhandle); /* transfer http */
	curl_slist_free_all(headers); /* free the header list */
    ```
    对于已经存在的消息头，可以重新设置它的值：
    ```c
	headers = curl_slist_append(headers, "Accept: Agent-007");  
	headers = curl_slist_append(headers, "Host: munged.host.line"); 
	```

	删除消息头

    对于一个已经存在的消息头，设置它的内容为空，libcurl在发送请求时就不会同时提交该消息头：
    ```c
	headers = curl_slist_append(headers, "Accept:"); 
	```

	HTTP版本

    每一次http请求，都包含一个表示当前使用http版本的消息头。libcurl默认使用HTTP 1.1。可以通过CURLOPT_HTTP_VERSION属性来设置具体的版本号：
    ```c
	curl_easy_setopt(easy_handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_0); 
	```

	libcurl 使用post的注意事项
	当使用libcurl的POST方式时，如果POST数据的大小大于1024个字节，libcurl不会直接发送POST请求，而是会分为两步执行请求：
	<1> 发送一个请求，该请求头部包含一个Expect： 100-continue的字段，用来询问server是否愿意接受数据
	<2> 当接收到从server返回的100-continue的应答后，它才会真正的发起POST请求，将数据发送给server
		对于“100-continue"这个字段，RFC文档（http://www.w3.org/Protocols/rfc2616/rfc2616-sec8.html#sec8.2.3）是这么解释的：
它可以让客户端在发送请求数据之前去判断服务器是否愿意接收该数据，如果服务器愿意接收，客户端才会真正发送数据，
这么做的原因是如果客户端直接发送请求数据，但是服务器又将该请求拒绝的话，这种行为将带来很大的资源开销。
所以为了避免这种情况，libcurl在发送大于1024字节的POST请求时采用了这种方法，但是相对的，它会引起请求延迟的加大，
另外并不是所有的server都会正确处理并且应答”100-continue“，比如lighttpd，就会返回417”Expectation Failed“，造成请求逻辑出错。
解决办法:
	解决办法:
	```C
// POST数据的大于1024个字节
struct curl_slist *headerlist = NULL;
static const char buf[] = "Expect:";
headerlist = curl_slist_append(headerlist, buf); /* initalize custom header list */
curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist); /* set header*/
curl_slist_free_all(headerlist); /* free slist
	```


	多线程环境配置 CURLOPT_NOSIGNAL





# 出错排查
	传输失败总是有原因的。你可能错误的设置了一些libcurl的属性或者没有正确的理解某些属性的含义，或者是远程主机返回一些无法被正确解析的内容。

    这里有一个黄金法则来处理这些问题：将CURLOPT_VERBOSE属性设置为1，libcurl会输出通信过程中的一些细节。如果使用的是http协议，请求头/响应头也会被输出。将CURLOPT_HEADER设为1，这些头信息将出现在消息的内容中。

    抓包分析

    服务器不支持的编码类型


  # 建议
  	当需要发送多次请求时，应该重复使用easy handle。
  	每次执行完curl_easy_perform，licurl会继续保持与服务器的连接。接下来的请求可以使用这个连接而不必创建新的连接（如果目标主机是同一个的话）。这样可以减少网络开销。
  	对于无状态协议， 即使连接被释放了，libcurl也会缓存这些连接的会话信息，这样下次再连接到目标主机上时，就可以使用这些信息，从而减少重新连接所需的时间。
  	FTP连接可能会被保存较长的时间。因为客户端要与FTP服务器进行频繁的命令交互。对于有访问人数上限的FTP服务器，保持一个长连接，可以使你不需要排除等待，就直接可以与FTP服务器通信。