certificate-automation
======================

Simple scripts designed to automate the OpenSSL CA and certificate issuing process.


### Create a Certificate Authority (key and self-signed cert):

1.  Execute on command line.

```bash
sh setup_certauth.sh
```

2.  Follow on-screen steps.

### Create a Server Certificate (with key, signed by CA):

1.  Execute on command line.

```bash
sh make_server_cert.sh <server-name>
```

2.  Follow on-screen steps.

### Create a Client Certificate (with key, pkcs [.p12] cert for browser):

1.  Execute on command line.

```bash
sh new_client_cert.sh <client-name>
```

2.  Follow on-screen steps.

### add by andy 
1. 默认密钥文件都是没有密码的
2. 默认RSA 1024 , 365天
