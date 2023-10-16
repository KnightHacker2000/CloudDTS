#!/bin/sh
#
# Author: dts
# Date: 2021/05/13
#
#
# jdk路径
JAVA_HOME=$JAVA_HOME
# java程序所在路径
APP_HOME=$PWD
# java主程序
APP_MAINCLASS=com.alibaba.dts.mycode.DataTransferService
# jvm参数，自行设置
JAVA_OPTS="-ms1024m -mx1024m -Xmn512m -Djava.awt.headless=true -XX:MaxPermSize=256m"

# 启动程序
echo "start"

JAVA_CMD="$JAVA_HOME/bin/java $JAVA_OPTS -jar $APP_HOME/target/dts-contest-java-1.0-SNAPSHOT.jar $1=$2 $3=$4"
eval $JAVA_CMD

echo "end"
