#ifndef _ITEM_H_
#define _ITEM_H_

#include <string>
#include <vector>
#include <map>
using namespace std;

//define the struct of item
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
	map<string, item> gmap; //item code -> item info
	int privilege; //1=two-for-one, 2=discount

	void insertItems(map<string, int>& codeList, string code, int cnt);
	void calculate(map<string, int>& codeList);
	void display(map<string, int>& cartList, map<string, float>& itemPrices, 
			map<string, int>& giveItems, map<string, float>& saveDiscount);

public:
	Goods();
	void setPrivilege(int p); //set the privilege as discount/two-for-one
	int loadItemInformation(const char* filepath); //load item info from file
	void settleAccounts(string sline); //do settlement for accounts

	friend ifstream &operator>>(ifstream &inf, item &it);
};

#endif /* _ITEM_H_ */
