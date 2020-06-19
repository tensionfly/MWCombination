#ifndef MW_H_
#define MW_H_
#include"string"
#include"iostream"
#include"fstream"
#include"iomanip"
#include"sstream"
using namespace std;
#define f1 1.57542e9 //L1��Ƶ��
#define f2 1.22760e9 //L2��Ƶ��
#define C 299792458//����

struct rec
{
	string time[6];//ÿ����¼����Ԫ
	int num;//��Ԫ�ڵ����Ǹ���
	string *prn;//ָ����Ԫ��ÿ�����ǵ�prn��ŵ�ָ��
	double **obser;//ָ��ÿ����Ԫ���ǹ۲����ݵ�ָ��obser[recnum][5]
};

class dataread_deal
{
private:
	string strin,strout;//��������������ļ��ĵ�ַ
	int recnum;//��¼�ĸ���
	rec *recstruct;//��¼�ṹ���ָ��

public:
	dataread_deal(string strin_,string strout_);//���������ļ���������ĵ�ַ
	double str2num(string& s);
	void count_record();//��¼�ĸ���
	void dataread();//�����ݶ���ṹ����
	void deal();//��������
	void resultout();//����������
	~dataread_deal();//��������

};

#endif
