#include "HikDate.h"
#include<stdio.h>
// countDay.cpp : Defines the entry point for the console application.
//计算2个日期之间的天数
#include<stdio.h>
#include<conio.h>

/****************************************************************************************************
* @函数名称:	IsLeapYear
* @功  能：		判断是否是闰年
* @参  数：		
* @ ayear		年份
* @返回值：		true 代表闰年 false代表非闰年
* @说  明：		
****************************************************************************************************/
bool IsLeapYear( int ayear)
{
	bool Is_LeapYear;
	Is_LeapYear=((ayear%4==0&&ayear%100!=0)||(ayear%400==0));
	return Is_LeapYear;
}
/****************************************************************************************************
* @函数名称:	Monthfate
* @功  能：		判断某一月的天数
* @参  数：		
* @ date3		日期
* @返回值：		该月的天数
* @说  明：		
****************************************************************************************************/
int Monthfate(struct date nDate)
{
	switch(nDate.month)
	{
		case 1: 
		case 3: 
		case 5:
		case 7: 
		case 8: 
		case 10: 
		case 12: return 31;
			break;
		case 4: 
		case 6: 
		case 9: 
		case 11: return 30;
			break;
		case 2: 
			{
				if(IsLeapYear(nDate.year))
				{
					return 29;
				}
				else
				{	
					return 28;
				}
			}
				break;
		default: printf(" month error!\n");
	}
	return 0;
}
/****************************************************************************************************
* @函数名称:	Monthfate
* @功  能：		判断某一月的天数
* @参  数：		
* @ date3		日期
* @返回值：		该月的天数
* @说  明：		
****************************************************************************************************/
int DaysBetweenYear(struct date date1,struct date date2)
{
	int i=0;
	int yearDay=0;
	int sum_year=0;
	if(date2.month>date1.month||date2.month==date1.month&&date2.day>=date1.day)
	{
		for(i=date1.year+1;i<=date2.year;i++)
		{
			if(IsLeapYear(i))
			{
				yearDay=366;
			}
			else
				yearDay=365;
			sum_year+=yearDay;
		}
		return sum_year;
	}
	else
	{
		for(i=date1.year+1;i<date2.year;i++)
		{
			if(IsLeapYear(i))
			{
				yearDay=366;
			}
			else
				yearDay=365;
			sum_year+=yearDay;
		}
	}
	return sum_year;
}
/****************************************************************************************************
* @函数名称:	DaysBetweenMonth
* @功  能：		判断某一月的天数
* @参  数：		

* @返回值：		
* @说  明：		用来返回起始月份到年终所剩的天数
****************************************************************************************************/

int DaysBetweenMonth(struct date date1,struct date date2)
{
	int i=0;
	int monthday=0;
	int fate=0;
	int fateday=0;
	if(date2.year>date1.year&&date2.month<date1.month||date2.month==date1.month&&date2.day<date1.day)
	{
		for(i=date1.month+1;i<=12;i++)
		{
			fate=Monthfate(date1);
			monthday+=fate;
		}
		fateday=monthday+Monthfate(date1)-date1.day;
		return fateday;
	}
	else if(date2.year>=date1.year&&date2.month>date1.month||date2.month==date1.month&&date2.day>=date1.day)
	{
		for(i=date1.month+1;i<=date2.month;i++)
		{
			fate=Monthfate(date1);
			monthday+=fate;
		}
		fateday=monthday+date2.day-date1.day;
		return fateday;
	} 
	return 0;
}
/****************************************************************************************************
* @函数名称:	DaysBetweenMonth
* @功  能：		判断某一月的天数
* @参  数：		

* @返回值：		
* @说  明：		用来返回终止日期超过当年1月1日的天数
****************************************************************************************************/

int DaysoverFate(struct date date1,struct date date2)
{
	if(date2.year>date1.year&&date2.month<date1.month||date2.month==date1.month&&date2.day<date1.day)
	{
		int i=0;
		int monthday=0;
		int fate=0;
		int overday=0;
		for(i=1;i<date2.month;i++)
		{
			fate=Monthfate(date2);
			monthday+=fate;
		}
		overday=monthday+date2.day;
		return overday;
	}
	else 
	{
		return 0;
	}
	return 0;
}
/****************************************************************************************************
 * @函数名称:	DaysBetweenDate
 * @功  能：	
 * @参  数：		

 * @返回值：		
 * @说  明：	计算两个日期之间的天数
 ****************************************************************************************************/
int DaysBetweenDate(struct date date1,struct date date2)
{
	return DaysBetweenYear(date1,date2)+DaysBetweenMonth(date1,date2)+DaysoverFate(date1,date2);
}
