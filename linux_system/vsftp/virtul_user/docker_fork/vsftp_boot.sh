#!/bin/bash

#yum -y update && yum clean all
#yum -y install httpd && yum clean all
#RUN yum install -y \
#	vsftpd \
#	db4-utils \
#	db4

export FTP_USER=fengfan
export FTP_PASS=123456
export PASV_ADDRESS=**IPv4**
export PASV_MIN_PORT=21100
export PASV_MAX_PORT=21110
export LOG_STDOUT=Yes

cp vsftpd.conf /etc/vsftpd/
cp vsftpd_virtual /etc/pam.d/
cp run-vsftpd.sh /usr/sbin/

chmod +x /usr/sbin/run-vsftpd.sh
mkdir -p /home/vsftpd/
chown -R ftp:ftp /home/vsftpd/


/usr/sbin/run-vsftpd.sh
