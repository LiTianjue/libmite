#!/bin/bash
cp named.* 	/etc
cp *.zone /var/named/
chown named /var/named/*.zone
