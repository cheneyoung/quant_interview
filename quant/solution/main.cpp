/*** 
 * @Author: yangzuo
 * @Date: 2021-09-06 20:19:44
 * @Email: yangzuo@tencent.com
 * @LastEditors: yangzuo
 * @LastEditTime: 2021-10-26 19:53:13
 * @FilePath: /quant/mini_project/main.cpp
 */
#include "project.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <ostream>
using namespace std;


void write_data() {
    MarketData data1 = {
        1, {'6','0','0','0','0','0'}, {1,2,3,4,5}, {100,200,300,400,500},
        {1,2,3,4,5}, {100,200,300,400,500}
    };
    MarketData data2 = {
        1, {'6','0','0','0','1','0'}, {2,4,6,7,8}, {100,200,300,400,500},
        {1,2,3,4,5}, {100,200,300,400,500}
    };
    MarketData data3 = {
        1, {'6','0','0','0','0','1'}, {11,21,31,41,51}, {100,200,300,400,500},
        {1,2,3,4,5}, {100,200,300,400,500}
    };

    UpdateData u_data[3] = {
        2, {'6','0','0','0','1','0'},1,2,12,2000,
        2, {'6','0','0','0','0','0'},2,3,10,1000,
        2, {'6','0','0','0','0','1'},2,1,15,11000
    };

    UpdateData d_data[2] = {
        3, {'6','0','0','0','0','0'}, 1, 2, 0, 0,
        3, {'6','0','0','0','1','0'}, 2, 4, 0, 0     
    };
    ofstream iofile("data.dat", ios::binary);
	if(!iofile)
	{
		std::cerr<<"open error!"<<endl;
		abort( ); //退出程序
	}

    iofile.write((char*)&data1,sizeof(data1));
    iofile.write((char*)&data2,sizeof(data2));
    iofile.write((char*)&data3,sizeof(data3));
    for(int i = 0;i < 3; ++i) {
		iofile.write((char*)&u_data[i],sizeof(u_data[i]));
    }
    for(int j = 0;j < 2; ++j) {
		iofile.write((char*)&d_data[j],sizeof(d_data[j]));
    }
    iofile.close();
    return;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "wrong input" << std::endl;
        return -1;
    }
    char *path = argv[1];
    BidAsk *ba = new BidAsk();
    // write_data();
    ifstream infile;
    infile.open(path,ios::binary);
    if(!infile)
    {
        std::cout<<"The infile open error!"<<endl;
        abort( ); //退出程序
    }
    else
    {
        int read_bytes = 0;
        bool print_flag = false;
        while(infile.good() && !infile.eof()) {

            MarketData *md=new MarketData;
            infile.read((char*)md,sizeof(MarketData));
            if (unsigned(md->msgType) == 1) {
                read_bytes += sizeof(MarketData);
                ba->SnapShot(*md);
            } else {
                if (!print_flag) {
                    ba->PrintSnapShot();
                    print_flag = true;
                }
                UpdateData *ud=new UpdateData;
                infile.seekg(read_bytes, ios::beg);
                infile.read((char*)ud,sizeof(UpdateData));
                if (unsigned(md->msgType) == 2) {
                    read_bytes += sizeof(UpdateData);
                    ba->UpdateLevel(*ud);
                } else {
                    DeleteData *dd=new DeleteData;
                    infile.seekg(read_bytes, ios::beg);
                    infile.read((char*)dd,sizeof(DeleteData));
                    if (unsigned(dd->msgType) == 3) {
                        read_bytes += sizeof(DeleteData);
                        ba->DeleteLevel(*dd);
                    }
                }
            }
        }
    }
    // std::cout << "current marketdata is " << std::endl;
    ba->PrintSnapShot();
    infile.close();
    return 0;
}