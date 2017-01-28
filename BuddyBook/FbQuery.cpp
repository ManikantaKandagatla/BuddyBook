#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#pragma warning(disable:4996)

struct comments// total size of 128bytes
{
	char user_name[16];
	int like_c;
	char comment[108];
};

struct Message //total size of 256bytes
{
	int user_c;
	int likes_c;
	int no_of_comments;
	char user_name[8];
	char msg_content[176];
	 int cmts_single[10],cmts_double[4],cmts_thriple;
};

struct Group// total size of 128 bytes
{
	int user_c;
	int msg_c;
	char g_name[12];
	char desc[48];
	int msg_single[10], msg_double[4],msg_thriple;
};

struct messages
		{
			int cmt_c;
			int lik_c;
			struct comments *cmts;
};

struct group
{
	int msg_c;
	struct messages *msg;
};


struct user
{
	struct group grps[10];
	char username[6];
}*users_chain=NULL;

void load_data()
{
	Group grp;
	FILE *fp=fopen("fb.bin","r");
	fseek(fp,2+(1024*1024*8),SEEK_SET);
	for(i=0;i<10;i++)
	{
		fread(&grp,sizeof(grp),1,fp);
		if(users_chain==NULL)
		{
			user new_user
		}
		groups[i].msg_c=grp.msg_c;
		
	}
}


char* Trim_query(char *query)// making the query into simple divisible stream of tokens which is done using delimiters like [space] or ,
{
	int i=0,j=0,count=0;
	char *Tokens=(char*)malloc(sizeof(char)*strlen(query));
	while(query[i]==' ')
		i++;
	while(query[i]!='\0')
		if(query[i]==' '&&query[i+1]==' '||query[i]==' '&&query[i+1]==','||query[i]==' '&&Tokens[j-1]==','||query[i]==' '&&query[i+1]=='\0')// removing multiple spaces within a query
						i++;
		else if(query[i]=='[')
		{
				Tokens[j++]=query[i++];
				while(query[i]!=']')
				{
					if(query[i]==' ')
						i++;
					else Tokens[j++]=query[i++];
				}				
		}
		else Tokens[j++]=query[i++];
	if(j!=0) 
	{
		Tokens=(char*)realloc(Tokens,sizeof(char)*j);
		Tokens[j]='\0';
	}
	else return "";
	printf("query:%s\n",Tokens);
	return Tokens;
}

void get_max_word_len(char *Tokens,int *maxword_len,char delimeter,int *words)
{
	int i=0,count=0;
	*words=0;
	while(Tokens[i]!='\0')
	{
		if((Tokens[i]==delimeter))
			{
				if(*maxword_len<count)
					{
						*maxword_len=count;
						count=0;
					}
				(*words)++;
			}
		else count++;
		i++;
	}
	if(*maxword_len<count)
		*maxword_len=count;
	(*words)++;
	//printf("words:%d  max_len:%d\n",*words,*maxword_len);
}

char** get_Tokens(char *Token_stream,int max_len,char delimiter,int words)
{
	int i,k=0,j=0;
	char **Tokens=(char**)calloc(sizeof(char**),words),*temp=(char*)calloc(sizeof(char),max_len);
	for(i=0;i<words;i++)
		Tokens[i]=(char*)calloc(sizeof(char),max_len);
	i=0;
	while(Token_stream[i]!='\0')
	{
		if(Token_stream[i]==delimiter)
			{
				Tokens[k][j]='\0';//Tokenizing query as get [selector] [filter] [constraints] into a 2d array
				j=0;
				k++;
			}
		else Tokens[k][j++]=Token_stream[i];
		i++;
	}
	Tokens[k][j]='\0';
	return Tokens;
}

int* get_limits(char *numbers,int *flag,int *c)
{
	int i=0,n=0,k=0,*temp=(int*)calloc(sizeof(int),2);
	if(numbers[0]=='\0')
		return NULL;
	while(numbers[i]!='\0')
	{
		if(numbers[i]==':')
			{
				temp[k++]=n;
				n=0;
			}
		else if(numbers[i]>='0'&&numbers[i]<='9')
			n=n*10+numbers[i]-'0';
		else 
		{
			*flag=3;// error in indexing
			break;
		}
		i++;
	}
	temp[k++]=n;
	*c=k;
	return temp;
}

void map_intermediates(char **intermediates,int index,int *flag,int words,int *return_values,int *limits,int *operation)
{
	int i,k,j,*l,c,n;
	char *temp=(char*)calloc(sizeof(char),7),*numbers=(char*)calloc(sizeof(char),7);
	switch(index)
	{
		case 0:
			if(!strcmp(intermediates[0],"get")==0||words!=1)
				*flag=1; //get expected
			break;
		case 2:if(!strcmp(intermediates[0],"from")==0||words!=1)
				   *flag=2;// from expected
				break;
		case 1:
					
		case 3:
			for(i=0;i<words;i++)
			{
				k=j=c=0;
				while(intermediates[i][k]!='\0')
				{
					if(intermediates[i][k]=='[')
					{
						temp[j]='\0';
						j=0;
						k++;
						while(intermediates[i][k]!=']')
								numbers[j++]=intermediates[i][k++];
						numbers[j]='\0';
						l=get_limits(numbers,flag,&c);
						if(*flag!=3)
							set_limits(l,limits,temp,c,flag);
						else return;
					}
					else temp[j++]=intermediates[i][k++];
				}
			}break;
		case 4:
			if(strcmp(intermediates[0],"")==0)
			{}
			else
			{
			for(i=0;i<words;i++)
			{
				if(strcmp(intermediates[i],"max")==0)
						operation[0]=1;
				else if(strcmp(intermediates[i],"min")==0)
						operation[1]=1;
				else 
					{
						if(intermediates[i][0]=='<')
							operation[2]=1;
						else if(intermediates[i][0]=='>')
							operation[3]=1;
						else if(intermediates[i][0]=='=')
							operation[4]=1;
						n=number(intermediates[i],flag);
						if(*flag!=6)
							limits[14]=n;
					}	
			}
			}break;
	}
}

void startQuerying()
{
	char query[200],*q,**Tokens,**intermediates[5];
	struct student **collection;
	int max_len=0,i,words,w,t,students,*return_values=(int*)calloc(sizeof(int),11),*operation=(int*)calloc(sizeof(int),6),flag,limits[15]={0,100,0,13,0,154,0,18,0,8,0,6,0,3,-1},init_limits[15]={0,100,0,13,0,154,0,18,0,8,0,6,0,3,-1};
	while(1)
	{
		printf(">>>");
		gets(query);
		q=Trim_query(query);
		get_max_word_len(q,&max_len,' ',&words);
		Tokens=get_Tokens(q,max_len,' ',words);
		flag=students=0;
		for(i=0;i<words;i++)
		{
			max_len=0;
			w=0;
			get_max_word_len(Tokens[i],&max_len,',',&w);
			intermediates[i]=get_Tokens(Tokens[i],max_len,',',w);
			map_intermediates(intermediates[i],i,&flag,w,return_values,limits,operation);
		}
		if(flag==0)
			{
				collection=collect_entries(limits,return_values,&students);
				t=operation[0];
				for(i=1;i<5;i++)
					t|=operation[i];
				printf("operations:%d students_colctd:%d\n",t,students);
				if(!t)
					print(collection,return_values,students);
				else
					{
						if(operation[0]==1||operation[1]==1)
							collection=process_min_max(collection,operation,limits,&students);
						else collection=process_lt_gt_eq(collection,operation,limits,&students);
						print(collection,return_values,students);
					}
			}
		for(i=0;i<15;limits[i]=init_limits[i++]);
		for(i=0;i<11;return_values[i++]=0);
		for(i=0;i<6;operation[i++]=0);
	}
}

void main()
{

}

