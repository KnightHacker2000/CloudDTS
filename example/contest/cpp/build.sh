#!/bin/sh
#
# Author: dts
# Date: 2021/05/13
#
APP_HOME=demo.cpp

OUT_NAME=demo

rm -rf $OUT_NAME


#g++ -g $APP_HOME -std=c++11 -o $OUT_NAME
g++ -ggdb -O2 $APP_HOME -std=c++11 -o $OUT_NAME

#(函数)启动程序
echo "Build done"