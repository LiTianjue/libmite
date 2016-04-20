#!/usr/bin/python

#http://192.168.1.120/UpgradeVersionAction_check.action?os=x86&version=2.4.0

import web
urls=(
	'/','index',
	'/test','test',
	'UpgradeVersionAction_check.action',"check"
	'UpgradeVersionAction_check',"check_test"
	)



class index:
	def GET(self):
		return "Hello,world!"

class test:
	def GET(self):
		return "Hello,test!"

class check:
	def GET(self):
		return "Hello ,Check!"

class check_test:
	def GET(self):
		return "Hello ,Check Test!"


if __name__== "__main__":
	app = web.application(urls,globals())
	app.run()
