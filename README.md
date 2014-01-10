功能：
使用libcurl库以observer模式为基础的多线程下载工具。
最多支持5线程同时下载,默认是3进程同时下载。(暂时)

目前支持终端显示下载进度
使用ctrl+c结束显示


使用方法：
安装cmake以及libcurl

~/yycurl>cd build

~/yycurl/build>cmake ..

~/yycurl/build>make

~/yycurl/build>./bin/yycurl
