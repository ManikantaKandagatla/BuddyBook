#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<Windows.h>
#pragma warning(disable:4996)
#include<string.h>

void login();
void Session(char*);
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

int set_bit_vector(int flag) // setting bit vector and returning the next free block (256 or 128 bytes)
{
	FILE *fp=fopen("fb.bin","r+");
	int cursor;
	fseek(fp,12,0);
	while(fgetc(fp)!='0');
		//fseek(fp,1,SEEK_CUR);
	cursor=ftell(fp)-1;
	printf("\ncursor:%d\n",cursor);
	fseek(fp,-1,SEEK_CUR);
	if(flag==0)
		fwrite("11",2,1,fp); // for messages
	else fwrite("1",1,1,fp); // for groups and comments
	fclose(fp);
	return cursor;
} 
void print_Comments(unsigned int msg_cur)
{
	Message msg;
	comments cmt;
	int i,var_a,var_b,j=0,cmt_cur;
	char choice;
	FILE *fp=fopen("fb.bin","r");
	fseek(fp,msg_cur,SEEK_SET);
	fread(&msg,sizeof(msg),1,fp);
	system("cls");
	printf("Total Comments posted:%d\n",msg.no_of_comments);
	for(i=0;i<msg.no_of_comments;i++)
		{
			if(i==10)
					break;
			printf("\n\t\tComment %d\n ",i+1);
			fseek(fp,msg.cmts_single[i],SEEK_SET);
			fread(&cmt,sizeof(cmt),1,fp);
			printf("\t\t%s : %s\n\t\t%d Likes \n\n\t\t------------------------------------------------------------\n",cmt.user_name,cmt.comment,cmt.like_c);
		}
		if(msg.no_of_comments>10)
		{
			for(;i<msg.no_of_comments;i++)
			{
				if(i%5==0)
				{
				printf("\n\t\t    Press y/n to continue:");fflush(stdin);
				scanf("%c",&choice);
				if(choice!='y')
					break;
				}
				var_a=(i-10)%32;
				var_b=(i-10)/32;
				fseek(fp,msg.cmts_double[var_b]+4*(var_a),SEEK_SET);
				fread(&cmt_cur,4,1,fp);
				printf("cmt_cur:%d\n",cmt_cur);
				fseek(fp,cmt_cur,SEEK_SET);
				fread(&cmt,sizeof(cmt),1,fp);
				printf("\n\t\tComment %d\n ",i+1);
				printf("\t\t%s : %s\n\t\t%d Likes \n\n\t\t------------------------------------------------------------\n",cmt.user_name,cmt.comment,cmt.like_c);
			}
		}
	fclose(fp);
}

void Print_messages(unsigned int start,int grp_choice)// printing a message
{
	Group g;
	Message msg;
	int i,var_a,var_b,j=0,msg_cur;
	char choice;
	FILE *fp=fopen("fb.bin","r");
	fseek(fp,start+128*(grp_choice-1),SEEK_SET);
	fread(&g,sizeof(g),1,fp);
	system("cls");
	printf("--------------------------------------------------------------------------------\n\t\t\t\t%s\n--------------------------------------------------------------------------------\n",g.g_name);
	printf("Total Messages posted:%d\n",g.msg_c);
	for(i=0;i<g.msg_c;i++)
		{
			if(i==10)
					break;
			if(i%5==0&&i!=0)
				{
				printf("\n\t\t  Press y/n to continue:");fflush(stdin);
				scanf("%c",&choice);
				if(choice!='y')
					break;
				}
			printf("\n\t\tMessage %d\n ",i+1);
			fseek(fp,g.msg_single[i],SEEK_SET);
			//printf("msg_cur:%d\n",g.msg_single[i]);
			fread(&msg,sizeof(msg),1,fp);
			printf("\t\t%s : %s\n\t\t%d Likes  %d Comments\n\n\t\t------------------------------------------------------------\n",msg.user_name,msg.msg_content,msg.likes_c,msg.no_of_comments);
		}
		if(g.msg_c>10)
		{
			for(;i<g.msg_c;i++)
			{
				if(i%5==0)
				{
				printf("\t\tPress y/n to continue:");fflush(stdin);
				scanf("%c",&choice);
				if(choice!='y')
					break;
				}
				var_a=(i-10)%32;
				var_b=(i-10)/32;
				fseek(fp,g.msg_double[var_b]+4*(var_a),SEEK_SET);
				fread(&msg_cur,4,1,fp);
				//printf("msg_cur:%d\n",msg_cur);
				fseek(fp,msg_cur,SEEK_SET);
				fread(&msg,sizeof(msg),1,fp);
				printf("\n\t\tMessage %d\n ",i+1);
				printf("\t\t%s : %s\n\t\t%d Likes  %d Comments\n\t\t------------------------------------------------------------\n",msg.user_name,msg.msg_content,msg.likes_c,msg.no_of_comments);
			}
		}
		fclose(fp);
}

int update_group(unsigned int start,int grp_id)// upating the group structure data when a new message is created
{
	FILE *fp=fopen("fb.bin","rb+");
	int var_a,var_b,cursor;
	Group g;
	fseek(fp,start+128*(grp_id-1),SEEK_SET);
	fread(&g,sizeof(g),1,fp);
	g.msg_c+=1;
	if(g.msg_c<11) 
		{
			cursor=set_bit_vector(0);  // creating a block for new message (256 bytes)
			g.msg_single[g.msg_c-1]=start+128*(cursor-1);
			cursor=start+128*(cursor-1);
			fseek(fp,-128,SEEK_CUR);
	}
	else  // filling single indirect message block
		{
			var_a=(g.msg_c-11)%32; 
			var_b=(g.msg_c-11)/32;
			if(var_a==0)
				{
				cursor=set_bit_vector(1); //creating new indirect block (128 bytes)
				cursor=start+128*(cursor-1);
				printf("new messge inde:%d\n",cursor);
				g.msg_double[var_b]=cursor;
				cursor=set_bit_vector(0); // creating a new block for message (256 bytes)
				cursor=128*(cursor-1)+start;
				fseek(fp,g.msg_double[var_b],SEEK_SET);
			}
			else {
				cursor=set_bit_vector(0);  // filling 32 msgs in one indirect block (256 bytes)
				cursor=start+128*(cursor-1);
				printf("messge_cur:%d",cursor);
				fseek(fp,g.msg_double[var_b]+4*(var_a),SEEK_SET);
			}
		}
	fwrite(&cursor,sizeof(cursor),1,fp); //now placing the new messge block cursor position
	fseek(fp,start+128*(grp_id-1),SEEK_SET);
	fwrite(&g,sizeof(g),1,fp);
	fclose(fp);
	return cursor;
}

int update_Message(int message_cur,int start)// updates a messge block when new comments arrive 
{
	FILE *fp=fopen("fb.bin","rb+");
	int var_a,var_b,cursor;
	Message msg;
	fseek(fp,message_cur,SEEK_SET);
	fread(&msg,sizeof(msg),1,fp);
	msg.no_of_comments+=1;
	if(msg.no_of_comments<11) 
		{
			cursor=set_bit_vector(1);  // creating a block for new comment (128 bytes)
			msg.cmts_single[msg.no_of_comments-1]=start+128*(cursor-1);
			cursor=start+128*(cursor-1);
			fseek(fp,-256,SEEK_CUR);
	}
	else  // filling single indirect message block
		{
			var_a=(msg.no_of_comments-11)%32; 
			var_b=(msg.no_of_comments-11)/32;
			if(var_a==0)
				{
				cursor=set_bit_vector(1); //creating new indirect block (128 bytes)
				cursor=start+128*(cursor-1);
				printf("new messge inde:%d\n",cursor);
				msg.cmts_double[var_b]=cursor;
				cursor=set_bit_vector(1); // creating a new block for comment (256 bytes)
				cursor=128*(cursor-1)+start;
				fseek(fp,msg.cmts_double[var_b],SEEK_SET);
			}
			else {
				cursor=set_bit_vector(1);  // filling 32 comments in one indirect block (256 bytes)
				cursor=start+128*(cursor-1);
				printf("comment_cur:%d",cursor);
				fseek(fp,msg.cmts_double[var_b]+4*(var_a),SEEK_SET);
			}
		}
	fwrite(&cursor,sizeof(cursor),1,fp); //now placing the new comment block cursor position
	fseek(fp,message_cur,SEEK_SET);
	fwrite(&msg,sizeof(msg),1,fp);
	fclose(fp);
	return cursor;
}

void process_subThread_Comment_Like_Del(int cmt_cur,int cmt_id)// processing a comment
{
	FILE *fp;
	int cmt_ch,cmt_cur_indirect,t=cmt_cur;
	comments cmts;
	while(1)
	{
	fp=fopen("fb.bin","rb+");
	fseek(fp,cmt_cur,SEEK_SET);
	printf("1.Like 2.Edit  3.Delete: 4.Go back to comments -1.logout:    ");
	scanf("%d",&cmt_ch);
	if(cmt_id>10)
		{
			fseek(fp,t+4*(cmt_id-11)%32,SEEK_SET);
			//printf("indirect:%d\n",ftell(fp));
			fread(&cmt_cur_indirect,sizeof(cmt_cur_indirect),1,fp);
			//printf("msg_ind:%d\n",msg_cur_indirect);
			cmt_cur=cmt_cur_indirect;
			fseek(fp,cmt_cur_indirect,SEEK_SET);
		}
	else fseek(fp,cmt_cur,SEEK_SET);
	switch(cmt_ch)
	{
			case 1:
				fread(&cmts,sizeof(cmts),1,fp);// liking a comment.......
				cmts.like_c+=1;
				fseek(fp,-128,SEEK_CUR);
				fwrite(&cmts,sizeof(cmts),1,fp);
				fclose(fp);
				break;
			case 2:
				fread(&cmts,sizeof(cmts),1,fp);
				printf("\t\t\tEnter modified comment:");fflush(stdin);
				gets(cmts.comment);
				fseek(fp,-128,SEEK_CUR);
				fwrite(&cmts,sizeof(cmts),1,fp);
				fclose(fp);
				break;
			case 3:printf("will be updated soon..\n");break;
			case 4:fclose(fp);return;
			case -1:login();
			default:printf("\t\t\tInvalid choice...!!!!\n\n");
	}
	fclose(fp);
	}
}

void process_subThread_Comment(int Message_cur,int start,int msg_id,int msg_c,char *username)// processing through a message
{
	int comment_cur,cmt_choice,i,cmt_id,msg_cur_indirect,t=Message_cur;
	FILE *fp;
	comments cmt;
	Message msg;
	while(1)
	{
	fp=fopen("fb.bin","rb+");
	printf("\n\t\t\t1.Write Comment\t2.View Comments\n\t\t\t3.Like\t4.Back to Messages\t-1:logout\n");
	scanf("%d",&cmt_choice);
	if(msg_id>10)
		{
			fseek(fp,t+4*(msg_id-11)%32,SEEK_SET);
			printf("indirect:%d\n",ftell(fp));
			fread(&msg_cur_indirect,sizeof(msg_cur_indirect),1,fp);
			printf("msg_ind:%d\n",msg_cur_indirect);
			Message_cur=msg_cur_indirect;
			fseek(fp,msg_cur_indirect,SEEK_SET);
		}
	else fseek(fp,Message_cur,SEEK_SET);
	switch(cmt_choice)
	{
		case 1://new comment
			printf("\t\t\tComment Here:");fflush(stdin);
			gets(cmt.comment);
			cmt.like_c=0;
			comment_cur=update_Message(Message_cur,start);
			strcpy(cmt.user_name,username);
			fseek(fp,comment_cur,SEEK_SET);
			fwrite(&cmt,sizeof(cmt),1,fp);
			break;
		case 2:// view comment
			fread(&msg,sizeof(msg),1,fp);
			print_Comments(Message_cur);
			if(msg.no_of_comments!=0)
			{
				printf("\t\t\t\t\tGo through comment:");
				scanf("%d",&cmt_id);
				if(cmt_id<10)
					process_subThread_Comment_Like_Del(msg.cmts_single[cmt_id-1],cmt_id);
				else process_subThread_Comment_Like_Del(msg.cmts_double[(msg.no_of_comments-10)/32],cmt_id);
			}
			else printf("\t\t\t\t\t\tNo comments!!!!!!!!\n");break;
		case 3: //like message
			fread(&msg,sizeof(msg),1,fp);
			msg.likes_c+=1;
			fseek(fp,-256,SEEK_CUR);
			fwrite(&msg,sizeof(msg),1,fp);
			break;
		case 4:
			fclose(fp);
			return;
		case -1:login();
		default:printf("\t\tInvalid choice...!!!!\n\n");
	}
	fclose(fp);
}
}

void process_Thread_Message(char *file_name,int grp_choice,unsigned int start,char *username)// process a message
{
	unsigned int msg_choice,i=0,cursor,msg_id;
	FILE *fp=fopen(file_name,"rb+");
	struct Message msg;
	struct Group g;
	while(1)
	{
	printf("\n\n\t\t1.Write New Message 2.Go through Message\n\t\t3.Back to groups:  -1.logout        ");
	scanf("%d",&msg_choice);fflush(stdin);
	fp=fopen(file_name,"rb+");
	fseek(fp,start+128*(grp_choice-1),SEEK_SET);
	switch(msg_choice)
	{
		case 1:// writing new message
				strcpy(msg.user_name,username);
				printf("Enter message:");
				gets(msg.msg_content);
				fflush(stdin);
				msg.likes_c=msg.no_of_comments=0;
				cursor=update_group(start,grp_choice);
				fseek(fp,cursor,SEEK_SET);
				fwrite(&msg,sizeof(msg),1,fp);
				fclose(fp);
				Print_messages(start,grp_choice);
				break;
		case 2:// going thriugh a message
				fread(&g,sizeof(g),1,fp);
				if(g.msg_c!=0)
				{
					printf("\t\t\t Choose a Message to view:");
					scanf("%d",&msg_id);
					if(msg_id<=g.msg_c&&msg_id>0)
					{
						if(msg_id<11)
							process_subThread_Comment(g.msg_single[msg_id-1],start,msg_id,g.msg_c,username);
						else 
							process_subThread_Comment(g.msg_double[(g.msg_c-10)/32],start,msg_id,g.msg_c,username);
					}
					else printf("\noops choose a valid Message in the group...\n");
				}
				else printf("\t\tNo messages to choose!!!!!\n");
				break;
		case 3:return;
		case -1:login();
		default:printf("\t\tInvalid choice...!!!!\n\n");
	}
	fclose(fp);
	}
}

int* get_all_grps_notif_c(char *filename,int start)// refreshing the groups with their activites
{
	int i,*notify_arr=(int*)calloc(sizeof(int),10);
	struct Group g;
	FILE *fp;
	fp=fopen(filename,"r");
	for(i=0;i<10;i++)
	{
		fseek(fp,start+128*(i),SEEK_SET);
		fread(&g,sizeof(g),1,fp);
		notify_arr[i]=g.msg_c;
	}
	return notify_arr;
	fclose(fp);
}

void login()// user login(nominal)
{
	char username[8];
	while(1)
	{
	system("cls");
	printf("--------------------------------------------------------------------------------\n\t\t\t\tBuddyy Book\n--------------------------------------------------------------------------------\n");
	printf("Login Username:");
	fflush(stdin);
	gets(username);
	if(strlen(username)>8)
			printf("Enter username should be less than 8 characters!!!!!!\n");
	else Session(username);
	}
}

void main()
{
	login();
	getche();
}

void Session(char *username) // exploring through all the available feature of fb
{
	int grp_choice,byte_vector_len=1024*1024*8;
	int *notification_c;
	while(1)
	{
		notification_c=get_all_grps_notif_c("fb.bin",byte_vector_len+2);
		system("cls");
		printf("--------------------------------------------------------------------------------\n\t\t\t\tBuddyy Book(%s)\n",username);
		printf("-------------------------------------------------------------------------------");
		printf("\n1.Education (%d)\n2.Sports (%d)\n3.Movie (%d)\n4.Fashion (%d)\n5.Politics (%d)\n6.Coding (%d)\n7.Tech (%d)\n8.Cooking (%d)\n9.Gadgets (%d)\n10.Govt (%d) -1.logout\n",notification_c[0],notification_c[1],notification_c[2],notification_c[3],notification_c[4],notification_c[5],notification_c[6],notification_c[7],notification_c[8],notification_c[9]);
		printf("-------------------------------------------------------------------------------\n");
		printf("Choose your desired Group to view:        ");
		scanf("%d",&grp_choice);
		if(grp_choice!=-1)
			{
				Print_messages(byte_vector_len+2,grp_choice);
			process_Thread_Message("fb.bin",grp_choice,byte_vector_len+2,username);
		}
		else return;
	}
	getche();
}