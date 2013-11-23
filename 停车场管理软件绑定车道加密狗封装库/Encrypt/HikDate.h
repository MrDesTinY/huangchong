#ifndef __HIK_DATE_H__
#define __HIK_DATE_H__

struct date
{
	int year;
	int month;
	int day;
};
int DaysBetweenDate(struct date date1,struct date date2);
int DaysoverFate(struct date date1,struct date date2);
int DaysBetweenMonth(struct date date1,struct date date2);
int DaysBetweenYear(struct date date1,struct date date2);
int Monthfate(struct date date3);
bool IsLeapYear(int ayear);
#endif //__HIK_DATE_H__