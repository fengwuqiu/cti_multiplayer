#!/bin/bash
#sleep 10
#display=`echo ${DISPLAY}`
#echo "display: "$display
export DISPLAY=:0.0
cd /home/cti/cti_multiplayer/log/
log_file_name=cti_multiplayer.log
log_file_name_bak=cti_multiplayer.log.1
log_file_size=`ls -l $log_file_name | awk '{ print $5 }'`
log_file_size=`expr $log_file_size / 1024 / 1024`
echo "log文件大小： $log_file_size"
if [ $log_file_size -gt 100 ]
then
    echo "大于100mb——备份"
	mv $log_file_name $log_file_name_bak
else 
    echo "小于100mb——正常"
fi
file_cnt=`ls -l /home/cti/cti_multiplayer/log/ | grep "^-" | wc -l`
file_limit=5
echo "file_cnt: "$file_cnt
echo "file_limit: "$file_limit
if [ $file_cnt -gt $file_limit ];
then
	file_need_remove=`expr $file_cnt - $file_limit`	
	echo "file_need_remove: "$file_need_remove
	ls -ltr | grep -v 'total' | awk '{print $9}' | head -n $file_need_remove | xargs rm -rf
	echo "remove old file success!"
fi
./home/cti/cti_multiplayer/cti_multiplayer

