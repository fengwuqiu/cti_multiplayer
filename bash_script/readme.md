1.ubuntu 18.04界面
将cti_multiplayer 拷贝到 home目录下
先按照4b_env_set.sh安装环境
删除 cti_multoiplayer/ 下所有的 .o 和 moc* 文件 rm *.O  ; rm moc*
qmake
make
1.1 设置屏幕竖屏显示
直接在设置中的display 中设置 Rotation 为 Counterclockwise,并保存设置.
1.2 设置免登录
  直接在设置 User  Accounts 中设置 Automatic Login 为on
1.3 设置不息屏
 设置中的Brightness&lock
 Turn screen off whhen inactive: Never
 Lock
 off
 Require 不勾选
1.4 设置背景为黑色, 隐藏任务栏
 把 cti_multiplayer/media/black.jpg设置为桌面背景
sudo apt-get install gnome-shell-extension-dashtodock


1.5 设置程序自启
 搜索Startup Applications  然后 add
 Name: cti_multiplayer(这个写啥都行)
 Command: /home/cti/cti_multiplayer/start_cti_multiplayer.sh
 Comment: 空

1.6 设置固定ip 192.168.1.50
1.7 删除桌面所有图标




二 LXDE桌面
1.设置自动登录桌面  
sudo touch  /etc/lightdm/lightdm.conf 
sudo vim /etc/lightdm/lightdm.conf 
添加以下内容

[SeatDefaults]
autologin-user=cti
autologin-user-timeout=0
user-session=Lubuntu
greeter-session=lightdm-gtk-greeter

2.设置竖屏显示   
xrandr --verbose | awk '/ connected/{print $1}'
显示现在是屏幕输出是什么? 为 HDMI-0
然后在下面文件中添加
vim /etc/X11/Xsession.d/55gnome-session_gnomerc   
在最后添加 xrandr --output HDMI-0(第一个命令的输出)--rotate left (方向不对的话改成right)  

3.设置ctrl+alt+t打开终端

    3.1 打开LXDE配置文件

    sudo vim ~/.config/openbox/lxde-rc.xml


    3.2.在<keyboard>(…)</keyboard>部分增加如下内容

    <keybind key=”C-A-t″>
    <action name=”Execute">
        <command>lxterminal</command>
    </action>
    </keybind>

4.系统字体太小的问题
    未找到解决办法
5.添加自启动服务
    cd  /home/cti/.config/lxsession/LXDE/
    vim autostart
    修改如下:
    @lxpanel --profile LXDE
    @pcmanfm --desktop --profile LXDE
    @lxterminal -e bash /home/cti/cti_multiplayer/start_cti_multiplayer.sh
    @xscreensaver -no-splash
6. 空闲自动隐藏菜单栏
右键菜单栏->panel settings -> advanced -> 勾选 minimize panel when not in use
7.设定桌面背景为黑色 背景图片换成黑色图片  /cti_multiplayer/media 下面有一个 black.jpg
8.设定网络ip为 192.168.1.50 直接使用界面设置

