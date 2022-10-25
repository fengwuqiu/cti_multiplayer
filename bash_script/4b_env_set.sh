#!/bin/bash
mkdir /home/cti/log
sudo apt-get update
sudo apt-get install libzmq3-dev -y
sudo apt-get install qt5-default -y
sudo apt-get install qtmultimedia5-dev -y
sudo apt-get install libqt5multimedia5-plugins -y
sudo apt-get install unclutter -y
sudo echo “@unclutter -idle 5 -root” >> /etc/xdg/lxsession/LXDE/autostart 
cd /home/cti/cti_multiplayer 
rm *.o
rm moc*
qmake
make