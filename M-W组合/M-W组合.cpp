#include"MW.h"

dataread_deal::dataread_deal(string strin_, string strout_)//�๹�캯��
{
	strin = strin_;
	strout = strout_;
	recnum = 0;
}

dataread_deal::~dataread_deal()//����������
{

}

double dataread_deal::str2num(string& s)//���ַ���ת��Ϊ����
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

void dataread_deal::count_record()//�������������ļ�����Ч����Ԫ����
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

void dataread_deal::dataread()//��ʱ�䡢prn�Լ���Ӧ�����ݴ洢����Ӧ�ṹ�崴����������
{
	int record = 0;
	recstruct = new rec[recnum];//Ϊ�ṹ�����鿪���ڴ�
	string str;
	ifstream infile(strin);
	infile.seekg(2009);//����ͷ�ļ�����

	while (!infile.eof())
	{
		getline(infile, str);

    	if (str.length()>33&&str.substr(1, 2) == "16"&&str.substr(28, 1) == "0")//�ж�ÿ����Ԫ������״̬�Ƿ�����
		{
			int k, m;
			double L1, L2, C1, P1, C2, P2;

			for (int j = 0; j < 5; j++)//�洢ʱ��
			{
				recstruct[record].time[j] = str.substr(1 + 3 * j, 2);
			}
			recstruct[record].time[5] = str.substr(16, 10);

			recstruct[record].num = str2num(str.substr(30, 2));//�洢���ݵ�Ԫ����
			
			recstruct[record].prn = new string[recstruct[record].num];//�洢PRN
			for (k = 0; 32 + 3 * k < str.length(); k++)
			{
				recstruct[record].prn[k] = str.substr(32 + 3 * k, 3);
			}
			
			if (k < recstruct[record].num)//�ж��Ƿ�洢prn��ϣ�û�оͼ����洢
			{
				m = k;
				getline(infile, str);
				for (k = m; k < recstruct[record].num; k++)
				{
					recstruct[record].prn[k] = str.substr(32 + 3 * (k - m), 3);
				}
			}
			
			recstruct[record].obser = new double*[recstruct[record].num];//�洢����
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

void dataread_deal::deal()//����洢������ͬʱ�洢����Ľ��
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

void dataread_deal::resultout()//��ÿ�����ǵ�M��W���ֵ�����ָ���ļ�
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
	dataread_deal RD("C:\\Users\\tension fly\\Desktop\\����.txt", "C:\\Users\\tension fly\\Desktop\\result.txt");//����RD��ͬʱ��ʼ��
	RD.count_record();//������Ԫ��������
	RD.dataread();//�������ݶ�ȡ����
	RD.deal();//�������ݴ�����
	RD.resultout();//���ý���������
	cout << "���ݴ�����ɣ�";
	getchar();
	return 0;
}