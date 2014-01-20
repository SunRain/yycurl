功能：
使用libcurl库以observer模式为基础的多线程下载工具。
支持n线程同时下载,默认是3进程同时下载。

目前支持终端显示下载进度
使用ctrl+c结束显示

使用方法：
安装cmake,libcurl以及openssl libssl-dev

~/yycurl>cd build

~/yycurl/build>cmake ..

~/yycurl/build>make

~/yycurl/build>./bin/yycurl

一些bug：
断点续传时，出现Requested range was not delivered by the server，目前使用重联解决，重联次数暂时不设上限。
