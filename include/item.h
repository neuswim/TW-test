#ifndef _ITEM_H_
#define _ITEM_H_

#include <string>
#include <vector>
#include <map>
using namespace std;

typedef struct ITEM {
	string code; //bar code of the item
	string name; //name of the item
	int type; //category of the item
	float price; //price of the item
	string unit; //unit of the item
	int buyCnt; //but Cnt free M
	int giveCnt; //buy M free Cnt
	float discount; //discount 95%
} item;


class Goods {
private:
	map<string, item> gmap;
	int privilege; //0=discount, 1=two-for-one

	void insertItems(map<string, int>& codeList, string code, int cnt);
	float calculate(map<string, int>& codeList);
	void display(map<string, int>& cartList, map<string, float>& itemPrices, 
			map<string, int>& giveItems, map<string, float>& saveDiscount);

public:
	Goods();
	void setPrivilege(int p);
	int loadItemInformation(const char* filepath);
	int settleCounts();

	friend ifstream &operator>>(ifstream &inf, item &it);
};


#endif
