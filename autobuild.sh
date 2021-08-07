##################################################
# File Name: autuobuild.sh
# Author: Zhu Rui
# mail: zhurui.alan@foxmail.com
# Created Time: 2021年08月07日 星期六 13时14分23秒
##################################################

set -x
rm -rf `pwd`/build/*
cd `pwd`/build &&
	cmake .. &&
	make
