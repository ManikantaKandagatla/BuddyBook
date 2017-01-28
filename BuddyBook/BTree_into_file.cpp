#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
#pragma warning(disable:4996)

struct metadata
{
	int pointers[3];
	int values[2];
	char flag[4];
};

struct leafnode
{
	int next;
	int pre;
	char name[12];
};

int create_metadata_node()
{
	int mdt_cur;
	FILE *fp=fopen("test.txt","r+");
	fseek(fp,0,SEEK_END);
	mdt_cur=ftell(fp);
	metadata *m=(metadata*)calloc(sizeof(metadata),1);
	m->pointers[0]=m->pointers[1]=m->pointers[2]=-1;
	fwrite(m,sizeof(m),1,fp);
	fclose(fp);
	printf("mdt:%d ",mdt_cur);
	return mdt_cur;
}

int create_leaf_node(leafnode lfn,int i,int n)
{
	int lfn_cur;
	FILE *fp=fopen("test.bin","rb+");
	fseek(fp,0,SEEK_END);
	lfn_cur=ftell(fp);
	if(i==0)
		lfn.pre=-1;
	else lfn.pre=24*(i-1);
	if(i==n-1)
		lfn.next=-1;
	else lfn.next=24*(i+1);
	fwrite(&lfn,sizeof(lfn),1,fp);
	fclose(fp);
	printf("lfn_data:%s lfn.pre:%d lfn.next:%d lfn.value:%d \n",lfn.name,lfn.pre,lfn.next,lfn.value);
	return lfn_cur;
}

void insertion()
{
	int i,lfn_cur;
	FILE *fp=fopen("test.txt","rb+");
	FILE *root = fopen("root.txt","r+");
	FILE *fp_data=fopen("data.bin","r");
	leafnode lfn;
	for(i=0;i<9;i++)
	{
		fread(&lfn,sizeof(lfn),1,fp_data);
		lfn_cur=create_leaf_node(lfn,i,9);
	}
}

void search(int n,int meta_pos)
{
	metadata mdt;
	FILE *fp=fopen("test.bin","r");
	fseek(fp,meta_pos,SEEK_SET);
	fread(&mdt,sizeof(mdt),1,fp);
	leafnode lfn;
	if(meta_pos<216)
		printf(" leaf_pos:%d\n",meta_pos);
	else if(mdt.values[0]>=n)
	{
		printf("metapos :%d\n ",mdt.pointers[0]);
		search(n,mdt.pointers[0]);
	}
	else if(mdt.values[1]<=n)
	{
		printf("metapos :%d\n ",mdt.pointers[2]);
		search(n,mdt.pointers[2]);
	}
	else if(mdt.values[0]<n&&mdt.values[1]>n)
		{
			printf("metapos :%d\n ",mdt.pointers[1]);
			search(n,mdt.pointers[1]);
		}
	fclose(fp);
}

void writedata()
{
	int i;
	struct leafnode lf;
	FILE *fp=fopen("data.bin","w");
	char name[20]={"groupA"};
	for(i=0;i<9;i++)
	{
		strcpy()
		fwrite(&lf,sizeof(lf),1,fp);
		name[5]+=1;
	}
	fclose(fp);
}

void build_final_metadata(metadata *mdt,int datan,int n,int k,int meta_start)
{
	int i=0,flag=0,next_meta_start;
	FILE *fp=fopen("test.bin","r+");
	fseek(fp,0,SEEK_END);
	next_meta_start=ftell(fp);
	fseek(fp,0,SEEK_SET);
	metadata m;
	if(n==1)
		return;
	for(i=0;i<n;i++)
	{
		if(i==n-1)
			{
				i++;
				flag=1;
		}
		if(i%3==0&&i!=0)
		{
			printf("metameta offset :%d\n",meta_start);
			mdt[k].pointers[0]=meta_start;
			mdt[k].pointers[1]=meta_start+24;
			mdt[k].pointers[2]=meta_start+48;
			fseek(fp,meta_start,SEEK_SET);
			fread(&m,sizeof(m),1,fp);
			mdt[k].values[0]=m.values[1];
			fseek(fp,24,SEEK_CUR);
			fread(&m,sizeof(m),1,fp);
			mdt[k].values[1]=m.values[0];
			fseek(fp,0,SEEK_END);
			fwrite(&mdt[k],sizeof(mdt[k]),1,fp);
			k++;
			meta_start+=72;
		}
		if(flag==1)
			break;
	}
	fclose(fp);
	build_final_metadata(mdt,datan,n/3,k,next_meta_start);
}

void build_init_metadata(int n)
{
	FILE *fp=fopen("test.bin","rb+");
	metadata mdt[14];
	leafnode lfn;
	int i,k=0,flag=0,next_meta_start;
	fseek(fp,0,SEEK_END);
	next_meta_start=ftell(fp);
	fseek(fp,0,SEEK_SET);
	for(i=0;i<n;i++)
	{
		if(i==n-1)
			{
				i++;
				flag=1;
		}
		if(i%3==0&&i!=0)
		{
			mdt[k].pointers[2]=24*(i-1);
			mdt[k].pointers[0]=24*(i-3);
			mdt[k].pointers[1]=24*(i-2);
			fseek(fp,24*(i-3),SEEK_SET);
			fread(&lfn,sizeof(lfn),1,fp);
			mdt[k].values[0]=lfn.value;
			fseek(fp,24,SEEK_CUR);
			fread(&lfn,sizeof(lfn),1,fp);
			mdt[k].values[1]=lfn.value;
			fseek(fp,0,SEEK_END);
			printf("meta inserted at:%d\n",ftell(fp));
			fwrite(&mdt[k],sizeof(mdt[k]),1,fp);
			k++;
		}
		if(flag==1)
			break;
	}
	fclose(fp);
	build_final_metadata(mdt,n,n/3,k,next_meta_start);
	for(i=0;i<n/2;i++)
		{
			printf("pointers %d %d %d ",mdt[i].pointers[0],mdt[i].pointers[1],mdt[i].pointers[2]);
			printf("values %d %d\n",mdt[i].values[0],mdt[i].values[1]);
	}
}



void main()
{
	int n,meta_pos;
	FILE *fp;
	/*writedata();
	insertion();
	build_init_metadata(9);*/
	fp=fopen("test.bin","r");
	fseek(fp,0,SEEK_END);
	meta_pos=ftell(fp)-24;
	while(1)
	{
	printf("\n\n\n\Enter a group to look up:");
	scanf("%d",&n);
	search(n,meta_pos);
	}
	fclose(fp);
	getche();
}