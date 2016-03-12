#include "item.h"
#include "json.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <algorithm>

/* 
 * @name : #overwrite
 * @function : overite ifstream for item
 * @param<IN> :
 * @return : 
 */
ifstream &operator>>(ifstream &inf, item &it)
{
	inf>>it.code>>it.name>>it.type>>it.price>>it.unit>>it.buyCnt
		>>it.giveCnt>>it.discount;
}


/* 
 * @name : #constructor
 * @function : construct an object
 * @param<IN> :
 * @return : 
 */
Goods::Goods()
{
	this->privilege = 0;
}


/* 
 * @name : setPrivilege
 * @function : set the member of privilege
 * @param<IN> p : privilege type (1=two-for-one, 0=)
 * @return : 
 */
void Goods::setPrivilege(int p)
{
	this->privilege = p;
}


/* 
 * @name : loadItemInformation
 * @function : load item information from file
 * @param<IN> filepath : path of the item info file
 * @return : 0=success  -1=error occur
 */
int Goods::loadItemInformation(const char* filepath)
{
	ifstream fin(filepath, ios::in);
	if(!fin.is_open()) {
		cerr<<"Error opening file"<<endl;
		return -1;
	}
	while(!fin.eof()) {
		item it;
		fin>>it;
		this->gmap.insert(pair<string, item>(it.code, it));
	}
	fin.close();
	return 0;
}


/* 
 * @name : display
 * @function : display the cash list
 * @param<IN> cartList : bar code of bought items and each item's quantity
 *			  itemPrices : final prices of each item
 *			  giveItems : two-for-one item and its quantity
 *			  saveDiscount : discount item and its save money
 * @return : 
 */
void Goods::display(map<string, int>& cartList, map<string, float>& itemPrices, 
		map<string, int>& giveItems, map<string, float>& saveDiscount)
{
	cout<<"***<没钱赚商店>购物清单***"<<endl;
	float total = 0;
	map<string, int>::iterator iter1;
	for(iter1 = cartList.begin(); iter1 != cartList.end(); ++iter1) {
		item it = this->gmap[iter1->first];
		cout<<"名称: "<<it.name<<", 数量: "<<iter1->second<<it.unit;
		cout.setf(ios::fixed);
		cout.precision(2);
		cout<<", 单价: "<<it.price<<"(元), 小计: "<<itemPrices[iter1->first]<<"(元)";
		if(saveDiscount.count(iter1->first) > 0)
			cout<<", 节省"<<saveDiscount[iter1->first]<<"(元)";
		cout<<endl;
		total += itemPrices[iter1->first];
	}
	if(giveItems.size() > 0) {
		cout<<"----------------------"<<endl;
		cout<<"买二赠一商品: "<<endl;
		map<string, int>::iterator iter2;
		for(iter2 = giveItems.begin(); iter2 != giveItems.end(); ++iter2) {
			item it = this->gmap[iter2->first];
			cout<<"名称: "<<it.name<<", 数量: "<<iter2->second<<it.unit<<endl;
		}
	}
	cout<<"----------------------"<<endl;
	cout<<"总计: "<<total<<"(元)"<<endl<<endl;;
}


/* 
 * @name : calculate
 * @function : calulate each item's final price by privilege rules
 * @param<IN> codeList : bar code of bought items and each item's quantity
 * @return : 
 */
void Goods::calculate(map<string, int>& codeList)
{
	map<string, float> itemPrices;
	map<string, int> giveItems;
	map<string, float> saveDiscount;

	map<string, int>::iterator iter;
	for(iter = codeList.begin(); iter != codeList.end(); ++iter) {
		string barCode = iter->first;
		if(this->gmap.count(barCode) == 0) {
			cerr<<"The item of "<<barCode<<" has not been registered."<<endl;
			continue;
		}
		item it = this->gmap[barCode];
		float cost = iter->second * it.price;
		float giftCost = 0;
		float discountCost = 0;
		if(it.buyCnt != 0 && it.giveCnt != 0) {
			int total = it.buyCnt + it.giveCnt;
			if(iter->second >= total) {
				giftCost = (iter->second - iter->second/total) * it.price;
			}
		}
		if(it.discount != 1.0) {
			discountCost = cost * it.discount;
		}

		if(giftCost > 0 && discountCost > 0) {
			if(this->privilege == 1) {
				cost = giftCost;
				giveItems.insert(pair<string, int>(iter->first, iter->second/(it.buyCnt+it.giveCnt)));
			} else {
				cost = discountCost;
				saveDiscount.insert(pair<string, float>(iter->first, cost - cost*it.discount));
			}
		} else if(giftCost != 0) {
			cost = giftCost;
			giveItems.insert(pair<string, int>(iter->first, iter->second/(it.buyCnt+it.giveCnt)));
		} else if(discountCost != 0) {
			cost = discountCost;
			saveDiscount.insert(pair<string, float>(iter->first, cost - cost*it.discount));
		}
		itemPrices.insert(pair<string, float>(iter->first, cost));
	}
	display(codeList, itemPrices, giveItems, saveDiscount);
}


/* 
 * @name : insertItems
 * @function : insert bar code to code list and record each quantity
 * @param<IN> codeList : bar code of bought items and each item's quantity
 *			  code : bar code
 *			  cnt : quantity
 * @return : 
 */
void Goods::insertItems(map<string, int>& codeList, string code, int cnt)
{
	if(codeList.count(code) > 0)
		codeList[code] += cnt;
	else
		codeList[code] = cnt;
}


/* 
 * @name : settleCounts
 * @function : do settlement for accounts 
 * @param<IN> sline : input json of cart
 * @return : 
 */
void Goods::settleAccounts(string sline)
{
	json::Value v = json::Deserialize(sline);
	if(v.GetType() == json::ObjectVal) {
		json::Object cartObj = v.ToObject();
		json::Value cartArray = cartObj["cart"];
		if(cartArray.GetType() == json::ArrayVal) {
			json::Array codeArray = cartArray.ToArray();
			map<string, int> codeList;
			for(size_t i = 0; i < codeArray.size(); ++i) {
				string code = (string)codeArray[i];
				string::size_type pos = code.find("-");
				if(pos != string::npos) {
					string cntStr = code.substr(pos+1, code.size()-pos);
					string codeStr = code.substr(0, pos);
					int cnt = atoi(cntStr.c_str());
					insertItems(codeList, codeStr, cnt);
				} else {
					insertItems(codeList, code, 1);
				}
			}

			calculate(codeList);
		}
		else {
			cerr<<"wrong format of the input data"<<endl;
		}
	} else {
		cerr<<"input data is not a json format"<<endl;
	}
}

