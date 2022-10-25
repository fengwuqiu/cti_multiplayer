用gdb模式运行

touch  .gdbinit    #.gdbini在主目录下

 .gdbinit文件内容：
file cti_multiplayer
r

自启start文件：
cd /home/cti/cti_multiplayer
#./cti_multiplayer
gdb

