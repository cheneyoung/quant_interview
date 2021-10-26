/*** 
 * @Author: yangzuo
 * @Date: 2021-08-30 20:04:14
 * @Email: yangzuo@tencent.com
 * @LastEditors: yangzuo
 * @LastEditTime: 2021-10-26 17:03:46
 * @FilePath: /quant/mini_project/project.h
 */
#include <iostream>
#include <map>
#include <stdlib.h>
#pragma pack(push, 1)

enum MsgType {
	SnapShot = 1,
	Update = 2,
	Delete = 3
};

struct MarketData
{
	uint8_t msgType;
	char symbol[6];		// 股票代码，比如 600000
	double bidPrice[5];	// 买价
	uint32_t bidSize[5];	// 买数量
	double askPrice[5];	// 卖价
	uint32_t askSize[5];	// 卖数量
};

struct UpdateData
{
	uint8_t msgType;
	char symbol[6];
	uint8_t side;		//  1-Bid   2-Ask
	uint8_t level; 		//  1 to 5
	double price;
	uint32_t size;
};

struct DeleteData
{
	uint8_t msgType;
	char symbol[6];
	uint8_t side;		//  1-Bid   2-Ask
	uint8_t level; 		//  1 to 5
};


class BidAsk {
    private:
        std::map<int,MarketData> current;
    public:
        ~BidAsk();
        void SnapShot(const MarketData &data);
        void UpdateLevel(const UpdateData &data);
		void DeleteLevel(const DeleteData &data);
		void PrintSnapShot();
		void PrintMarketData(MarketData &data);
};

#pragma pack(pop)