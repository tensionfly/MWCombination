#ifndef MW_H_
#define MW_H_
#include"string"
#include"iostream"
#include"fstream"
#include"iomanip"
#include"sstream"
using namespace std;
#define f1 1.57542e9 //L1的频率
#define f2 1.22760e9 //L2的频率
#define C 299792458//光速

struct rec
{
	string time[6];//每个记录的历元
	int num;//历元内的卫星个数
	string *prn;//指向历元内每个卫星的prn编号的指针
	double **obser;//指向每个历元卫星观测数据的指针obser[recnum][5]
};

class dataread_deal
{
private:
	string strin,strout;//输入输出的数据文件的地址
	int recnum;//记录的个数
	rec *recstruct;//记录结构体的指针

public:
	dataread_deal(string strin_,string strout_);//传入数据文件输入输出的地址
	double str2num(string& s);
	void count_record();//记录的个数
	void dataread();//将数据读入结构体中
	void deal();//处理数据
	void resultout();//结果输出函数
	~dataread_deal();//析构函数

};

#endif
