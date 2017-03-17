cp config /etc/polipo/config

sslocal -c shadowsocks.json -d start
/etc/init.d/polipo restart

export http_proxy="http://127.0.0.1:8123/"""
