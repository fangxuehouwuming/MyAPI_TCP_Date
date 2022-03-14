//根据年月份打印出日历
#pragma once
#define DATE_NAME "Date.dat"
#include<fstream>
#include<iostream>
#include <iomanip>
using namespace std;

int days, days2, month_firstday;
int leap_year(int year)
{   //判断是否为闰年,闰年返回1，平年返回0
	return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
}

void Date(int year, int month)
{
	ofstream outfile;
	outfile.open(DATE_NAME);
	outfile << " 日 一 二 三 四 五 六 " << endl;

	int monthdays_ping[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };         //平年每月天数
	int monthdays_run[12] = { 31,29,31,30,31,30,31,31,30,31,30,31 };          //闰年每月天数

	//先算某年一月一日是第几天，从1900年一月一日为第一天开始计数
	int days = 0;
	for (int j = 1900; j < year; j++)
	{
		if (leap_year(j))
			days += 366;
		else
			days += 365;
	}

	//然后计算某年某月第一天是第几天及星期几
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
	// 关闭打开的文件
	outfile.close();
}
