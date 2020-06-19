#include"MW.h"

dataread_deal::dataread_deal(string strin_, string strout_)//类构造函数
{
	strin = strin_;
	strout = strout_;
	recnum = 0;
}

dataread_deal::~dataread_deal()//类析构函数
{

}

double dataread_deal::str2num(string& s)//将字符串转化为数字
{
	double  num;
	int k1 = 0;
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] == ' ')
			k1++;
	}
	if (k1 == s.length())
		num = 0;
	else
	{
		stringstream ss(s);
		ss >> num;
	}
	return num;
}

void dataread_deal::count_record()//计算所给数据文件里有效的历元个数
{
	string str;
	ifstream infile(strin);
	infile.seekg(2009);
	while (!infile.eof())
	{
		infile >> str;
		if (str == "16")
		{
			for (int i = 0; i < 6; i++)
			{
				infile >> str;
			}
			if (str == "0")
				recnum++;	
		}
		
	}
	infile.close();
}

void dataread_deal::dataread()//将时间、prn以及相应的数据存储到对应结构体创建的数组中
{
	int record = 0;
	recstruct = new rec[recnum];//为结构体数组开辟内存
	string str;
	ifstream infile(strin);
	infile.seekg(2009);//跳过头文件部分

	while (!infile.eof())
	{
		getline(infile, str);

    	if (str.length()>33&&str.substr(1, 2) == "16"&&str.substr(28, 1) == "0")//判断每个历元的数据状态是否正常
		{
			int k, m;
			double L1, L2, C1, P1, C2, P2;

			for (int j = 0; j < 5; j++)//存储时间
			{
				recstruct[record].time[j] = str.substr(1 + 3 * j, 2);
			}
			recstruct[record].time[5] = str.substr(16, 10);

			recstruct[record].num = str2num(str.substr(30, 2));//存储数据单元个数
			
			recstruct[record].prn = new string[recstruct[record].num];//存储PRN
			for (k = 0; 32 + 3 * k < str.length(); k++)
			{
				recstruct[record].prn[k] = str.substr(32 + 3 * k, 3);
			}
			
			if (k < recstruct[record].num)//判断是否存储prn完毕，没有就继续存储
			{
				m = k;
				getline(infile, str);
				for (k = m; k < recstruct[record].num; k++)
				{
					recstruct[record].prn[k] = str.substr(32 + 3 * (k - m), 3);
				}
			}
			
			recstruct[record].obser = new double*[recstruct[record].num];//存储数据
			for (int i = 0; i < recstruct[record].num; i++)
			{
				recstruct[record].obser[i] = new double[5];
			}
			for (int i = 0; i < recstruct[record].num; i++)
			{
				getline(infile, str);
				L1 = str2num(str.substr(1, 13));
				L2 = str2num(str.substr(18, 12));
				if (str.length() < 66)
					P1 = C1 =str2num(str.substr(50, 12));
				else
				{
					C1 = str2num(str.substr(50, 12));
					P1 = str2num(str.substr(66, 12));
				}
				
				getline(infile, str);
				if (str.length() < 18)
					P2 = C2 = str2num(str.substr(2, 12));
				else
				{
					C2 = str2num(str.substr(2, 12));
					P2 = str2num(str.substr(18, 12));
				}
				
				recstruct[record].obser[i][0] = L1;
				recstruct[record].obser[i][1] = L2;
				recstruct[record].obser[i][2] = P1;
				recstruct[record].obser[i][3] = P2;
				if (P1 == 0)
					recstruct[record].obser[i][2] = C1;
				if (P2 == 0)
					recstruct[record].obser[i][3] = C2;

				getline(infile, str);
			}
			record++;
		}

	}
}

void dataread_deal::deal()//处理存储的数据同时存储处理的结果
{
	for (int i = 0; i < recnum; i++)
	{
		for (int j = 0; j < recstruct[i].num; j++)
		{
			recstruct[i].obser[j][4] = C*(recstruct[i].obser[j][0] - recstruct[i].obser[j][1]) / (f1 - f2) 
				                      - (f1*recstruct[i].obser[j][2] + f2*recstruct[i].obser[j][3]) / (f1 + f2);
		}
	}
}

void dataread_deal::resultout()//将每颗卫星的M—W组合值输出到指定文件
{
	ofstream outfile(strout);
	for (int i = 0; i < recstruct[0].num; i++)
	{
		outfile << recstruct[0].prn[i] << endl;
		if (recstruct[0].obser[i][0] * recstruct[0].obser[i][1] * recstruct[0].obser[i][2] * recstruct[0].obser[i][3] != 0)
		{
			for (int k = 0; k < 6; k++)
			{
				outfile << recstruct[0].time[k] << "     ";
			}
			outfile << recstruct[0].obser[i][4] << endl;
		}
		
		for (int j = 1; j < recnum; j++)
		{
			for (int m = 0; m < recstruct[j].num; m++)
			{
				if (recstruct[j].prn[m] == recstruct[0].prn[i] && 
					recstruct[j].obser[m][0] * recstruct[j].obser[m][1] * recstruct[j].obser[m][2] * recstruct[j].obser[m][3] != 0)
				{
					for (int k = 0; k < 6; k++)
					{
						outfile << recstruct[j].time[k] << "     ";
					}
					outfile << recstruct[j].obser[m][4] << endl;
					break;
				}

			}
		}
		outfile << endl;
	}
	outfile.close();
}

int main()
{
	dataread_deal RD("C:\\Users\\tension fly\\Desktop\\数据.txt", "C:\\Users\\tension fly\\Desktop\\result.txt");//创建RD类同时初始化
	RD.count_record();//调用历元计数函数
	RD.dataread();//调用数据读取函数
	RD.deal();//调用数据处理函数
	RD.resultout();//调用结果输出函数
	cout << "数据处理完成！";
	getchar();
	return 0;
}