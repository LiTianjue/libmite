#!/usr/bin/python

import httplib

url="http://192.168.1.120/UpgradeVersionAction_check.action?os=x86&version=2.4.0"

conn=httplib.HTTPConnection("192.168.1.120")

#conn.request(method="GET",url=url)
conn.request(method="POST",url=url)

response = conn.getresponse()

res = response.read()

print res
