//�������·ݴ�ӡ������
#pragma once
#define DATE_NAME "Date.dat"
#include<fstream>
#include<iostream>
#include <iomanip>
using namespace std;

int days, days2, month_firstday;
int leap_year(int year)
{   //�ж��Ƿ�Ϊ����,���귵��1��ƽ�귵��0
	return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
}

void Date(int year, int month)
{
	ofstream outfile;
	outfile.open(DATE_NAME);
	outfile << " �� һ �� �� �� �� �� " << endl;

	int monthdays_ping[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };         //ƽ��ÿ������
	int monthdays_run[12] = { 31,29,31,30,31,30,31,31,30,31,30,31 };          //����ÿ������

	//����ĳ��һ��һ���ǵڼ��죬��1900��һ��һ��Ϊ��һ�쿪ʼ����
	int days = 0;
	for (int j = 1900; j < year; j++)
	{
		if (leap_year(j))
			days += 366;
		else
			days += 365;
	}

	//Ȼ�����ĳ��ĳ�µ�һ���ǵڼ��켰���ڼ�
	int days2 = 0;
	if (leap_year(year))
		for (int i = 1; i < month;)
		{
			days2 += monthdays_run[i - 1];
			i++;
		}
	else
		for (int i = 1; i < month;)
		{
			days2 += monthdays_ping[i - 1];
			i++;
		}

	if (month == 1)
		month_firstday = (days + 1) % 7;
	else
		month_firstday = (days + 1 + days2) % 7;

	for (int i = 0; i < month_firstday; i++)                                   
		outfile << "   ";

	int count = 0;                                                          
	if (leap_year(year))
	{
		for (int i = 1; i <= monthdays_run[month - 1]; i++)
		{
			outfile << setw(3) << i;
			count += 1;
			if ((month_firstday + count) % 7 == 0)
				outfile << endl;
		}
	}
	else
	{
		for (int i = 1; i <= monthdays_ping[month - 1]; i++)
		{
			outfile << setw(3) << i;
			count += 1;
			if ((month_firstday + count) % 7 == 0)
			{
				outfile << endl;
			}
		}
	}
	outfile << endl;
	// �رմ򿪵��ļ�
	outfile.close();
}
