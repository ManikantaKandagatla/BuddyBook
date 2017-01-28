#pragma warning(disable: 4996)
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>

struct testcases
{
	unsigned int a;
}TC[10]={
	{241},
	{123456},
	{121},
	{211},
	{111212},
	{333222}
};

void swap(char *a,char *b)
{
	char t;
	t=*a;
	*a=*b;
	*b=t;
}

int len(char *a)
{
	int i=0;
	while(a[i])
		i++;
	return i;
}

char * to_string(int n)
{
	char *number = (char *)malloc(sizeof(char));
	int k=0;
	while(n>0)
	{
		number[k++] = '0'+ n%10;
		n = n/10;
		number = (char*)realloc(number,sizeof(char)*(k+1));
	}
	number[k]='\0';
	strrev(number);
	return number;
}

int succesors(char *input,char *number)
{
	int i,length=len(input),j,count = 1;
	while(1)
	{
		for(i=length-2;i>=0;i--)
		{
				if(input[i]<input[i+1])
					break;
		}
		if(strcmp(input,number)==0)
			return count;
		for(j=length-1;j>i;j--)
		{
					if(input[i]<input[j])
					break;
		}
		swap(&input[i],&input[j]);
		j=length-1;
		i++;
		for(;i<j;i++,j--)
			swap(&input[i],&input[j]);
		count++;
	}
}

void sort(char *a)
{

	int i,j,length=len(a);
	for(i=0;i<length;i++)
		for(j=i+1;j<length;j++)
		{
			if(a[i]>a[j])
				swap(&a[i],&a[j]);
		}
}

void main()
{
	char *input,*temp;
	int i,pos;
	bool flag;
	for(i=0;i<6;i++)
	{
		flag=true;
		temp = to_string(TC[i].a);
		input=(char*)calloc(sizeof(char),len(temp));
		strcpy(input, temp);
		sort(temp);
		pos = succesors(temp,input);
		printf("pos=%d\n",pos);
	}
	getche();
}