#include "item.h"
#include <iostream>

int main()
{
	Goods gd;
	gd.loadItemInformation("./data/goods.txt");
	gd.setPrivilege(1);
	gd.settleCounts();
}
