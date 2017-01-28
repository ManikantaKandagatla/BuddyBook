#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>
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
void set_bit_v_init()
{
	FILE *fp = fopen("fb.bin","rb+");
	fseek(fp,2,0);
		fprintf(fp,"1111111111");
	fclose(fp);
}

void write_bit_vector_to_fb_file()
{
	long long int i=0,length_of_bit_v,tot_len;
	FILE *fp=fopen("C:/Users/ManiKanta Kandagatla/Documents/Visual Studio 2012/Projects/BuddyBook/BuddyBook/fb.bin","wb");
		fprintf(fp,"K4");
		length_of_bit_v=1024*1024*8;
		while(i<=length_of_bit_v)
		{
				fputc('0',fp);
				i++;
		}
		fclose(fp);
}

void create_block_id(char *filename)
{
	int i=0,start;
	char grp_names[10][10]={"Education","Sports","Movie","Fashion","Politics","Coding","Tech","Cooking","Gadgets","Govt"};
	char desc[10][48]={"description","description","description","description","description","description","description","description","description","description"};
	FILE *fp=fopen(filename,"rb+");
	start=2+1024*1024*8;
	fseek(fp,start,0);
	struct Group g[10];
	for(i=0;i<10;i++)
	{
		strcpy(g[i].g_name,grp_names[i]);
		strcpy(g[i].desc,desc[i]);
		g[i].msg_c=0;
		g[i].user_c=0;
		fseek(fp,start+i*128,SEEK_SET);
		fwrite(&g[i],sizeof(struct Group),1,fp);
	}
	fclose(fp);
}


void main()
{
	write_bit_vector_to_fb_file();
	set_bit_v_init();
	create_block_id("fb.bin");
}
