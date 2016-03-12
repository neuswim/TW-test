#include "item.h"
#include "configfile.h"
#include <iostream>

int main()
{
	ConfigFile cfg("./conf/settle.conf");
	string goodsFile = cfg.getvalue<string>("goods");
	int promotion = cfg.getvalue<int>("promotion");
	
	Goods gd;
	gd.loadItemInformation(goodsFile.c_str());
	gd.setPrivilege(promotion);
	string sline;
	while(cin>>sline) {
		gd.settleAccounts(sline);
	}
}
