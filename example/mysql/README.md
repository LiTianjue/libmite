#登陆
mysql -u root -p

#创建数据库
CREATE DATABASE nfcid;

#选择数据库
USE nfcid;

#创建表
CREATE TABLE dev_list(id int,serial varchar(50),flag int,desc varchar(100));

#查看表
DESC dev_list

#插入数据
INSERT INTO dev_list VALUES(1,'201603160001',1,'pc-device');
INSERT INTO dev_list VALUES(2,'201603160002',0,'pc-device');
INSERT INTO dev_list VALUES(3,'201603160003',1,'phone-device');
INSERT INTO dev_list VALUES(4,'201603160004',1,'arm-device');
INSERT INTO dev_list VALUES(5,'201603160005',1,'nfc-device');

#查找数据
SELECT * FROM dev_list
SELECT serial,flag FROM dev_list
SELECT * FROM dev_list WHERE id=2


