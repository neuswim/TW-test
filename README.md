# TW-test
test program for ThoughtWorks

This work is compile in Linux OS with g++ 4.6.3 or above<br>
<br>
There are two third-party dependency lib for json and configure parser<br>
json		https://sourceforge.net/projects/supereasyjson/<br>
configure	https://github.com/benreh/configfile.git<br>
<br>
complie:  	make clean;make<br>
run:		cat data/cart.txt | ./test/test.exe<br>
<br><br>
In order to make the work easy to deploy, the item information is stored in file of "data/goods.txt" as:<br>
codebar		name		type	price	unit	buy	give	discount<br>
ITEM000001      可口可乐        1       3.00    瓶      2       1       0.95<br>
//buy-give means buy 2 free 1<br>
<br>
Test data is stored in file of "data/cart.txt" in json format as:<br>
{"cart":["ITEM000001","ITEM000001","ITEM000001","ITEM000003-5","ITEM000007-6"]}<br>
<br>
Configure file is in "conf/settle.conf":<br>
\#goods information<br>
goods = ./data/goods.txt<br>
<br>
\#promotion: if an item has both two-for-one and discount promotion, which one will be the highest priority. 1=two-for-one 2=discount<br>
promotion = 1<br>
<br>



