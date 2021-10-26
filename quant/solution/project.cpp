/*** 
 * @Author: yangzuo
 * @Date: 2021-08-30 20:07:02
 * @Email: yangzuo@tencent.com
 * @LastEditors: yangzuo
 * @LastEditTime: 2021-09-14 12:14:39
 * @FilePath: /quant/mini_project/project.cpp
 */
#include "project.h"


void BidAsk::SnapShot(const MarketData &data) {
    int key = atoi(data.symbol);
    current.insert(std::pair<int,MarketData>(key, data));
}

void BidAsk::UpdateLevel(const UpdateData &data) {
    int key = atoi(data.symbol);
    if (data.level > 5 || data.level < 1) {
        std::cout << "invalid data level, should in [1,5]" << std::endl;
        return;
    }
    std::map<int, MarketData>::iterator iter;
    iter = current.find(key);
    if (iter == current.end()) {
        std::cout << "the key " << key << " is invalid" << std::endl;
        return;
    }
    switch(data.side) {
        case 1: {
            current[key].bidPrice[data.level-1] = data.price;
            current[key].bidSize[data.level-1] = data.size;
            break;
        }
        case 2: {
            current[key].askPrice[data.level-1] = data.price;
            current[key].askSize[data.level-1] = data.size;
            break;
        }
        default: {
            std::cout << "invalid data side " << data.side << "not 1 or 2" << std::endl;
            break;
        }
    }
}

void BidAsk::DeleteLevel(const DeleteData &data) {
    int key = atoi(data.symbol);
    if (data.level > 5 || data.level < 1) {
        std::cout << "invalid data level, should in [1,5]" << std::endl;
        return;
    }
    std::map<int, MarketData>::iterator iter;
    iter = current.find(key);
    if (iter == current.end()) {
        std::cout << "the key " << key << " is invalid" << std::endl;
        return;
    }
    switch(data.side) {
        case 1: {
            for(uint8_t i = data.level-1; i < 4; ++i) {
                current[key].bidPrice[i] = current[key].bidPrice[i+1];
                current[key].bidSize[i] = current[key].bidSize[i+1];
            }
            current[key].bidPrice[4] = 0.0;
            current[key].bidSize[4] = 0;
            break;
        }
        case 2: {
           for(uint8_t i = data.level-1; i < 4; ++i) {
                current[key].askPrice[i] = current[key].askPrice[i+1];
                current[key].askSize[i] = current[key].askSize[i+1];
            }
            current[key].askPrice[4] = 0.0;
            current[key].askSize[4] = 0;
            break;
        }
        default: {
            std::cout << "invalid data side " << data.side << "not 1 or 2" << std::endl;
            break;
        }
    }
}

void BidAsk::PrintSnapShot() {
    std::cout << "##### current snapshot start #####" << std::endl;
    std::map<int,MarketData>::iterator iter;
    for(iter=current.begin(); iter!=current.end(); ++iter) {
        PrintMarketData(iter->second);
    }
    std::cout << "##### current snapshot end #####" << std::endl;
    return;
}

void BidAsk::PrintMarketData(MarketData &data) {
    std::cout << "股票代码: ";
    for(auto x:data.symbol) {
        std::cout << x;
    }
    std::cout << std::endl;
    int num = sizeof(data.bidPrice)/sizeof(double);
    for(int i = num - 1; i >= 0; --i) {
        std::cout << "买" << char('1' + i) << "价  " << data.bidPrice[i] << "(price)  ";
        std::cout << data.bidSize[i] << "(size) " << std::endl;
    }
    std::cout << std::endl;
    for(int i = num - 1; i >= 0; --i) {
        std::cout << "卖" << char('1' + i) << "价  " << data.askPrice[i] << "(price)  ";
        std::cout << data.askSize[i] << "(size) " << std::endl;
    }
    std::cout << std::endl;
}