#include<stdio.h>
#include<alloc.h>
#include<graphics.h>
#include<public.h>
#include"show.h"
ShowBMP256Picture();
typedef struct
{
	int bfType;//类型
	long bfSize;//大小
	int bfResered1;
	int bfResered2;//保留字
	long bfoffBits;//位图阵列起始位置
	
	 
}BITMAPFILEHEADER;
typedef struct
{
	long biSize;//结构大小
	long biWidth;//宽度
	long biHeight;//高度
	int biPlanes;//位平面数目 
	int biBitCount;//每个像素所占的位数
	long biCompress;//压缩类型
	long biSizeImage;//位图大小
	long biXPeIsPerMeter;//水平分辨率
	long biYPeIsPerMeter;//垂直分辨率
	long biClrUsed;//使用的颜色数目
	long biclrImprotant;//重要的颜色数目 
	
}BITMAPINFOHEADER;
unsigned int width,depth,d,b;
int ColorNum;
char palette[768];
BITMAPFILEHEADER FileHeader;
BITMAPINFOHEADER InfoHeader;
char far *p;
main(void)
{	FILE *fp;
fp=fopen("C:\\STOCK\\1234","rb");
b=(unsigned)InfoHeader.biHeight;
width=(unsigned)InfoHeader.biWidth;
depth=(width>640)?640:width;
ShowBMP256Picture();
}
ShowBMP256Picture(FILE *fp)
{
	union REGS reg;
	register int i,j;
	unsigned char color;
	setmode(93);//设置分辨率
	fseek(fp,54,SEEK_SET);
	for(i=0;i<b;i++)
	{
		palette[3*i+2]=(char)fgetc(fp);
		palette[3*i+1]=(char)fgetc(fp);
		palette[3*i]=(char)fgetc(fp);
		fgetc(fp);
		}
		LoadPalette(palette);//初始化调色板
		fseek(fp,FileHeader.bfoffBits+438,SEEK_SET);
		for(i=0;i<b;i++)
		{
			for(j=0;j<depth;j++)
			{
				color=fgetc(fp);
				reg.h.al=color;
				reg.h.ah=0x0c;
				reg.h.bh=0x01;
				reg.x.cx=j;
				reg.x.dx=b-i;
				int86(0x10,&reg,&reg);//用中断写像素
			}
			if(width>640)
			{
				fseek(fp,width-640,SEEK_CUR);
			}
		}
			getch();
			setmode(0x3);
			b=(unsigned)InfoHeader.biHeight;
				
			}
void setmode(int mode)
{
	union REGS reg;
	reg.h.ah=0;
	reg.h.al=mode;
	int86(0x10,&reg,&reg);
}
void LoadPalette(char *p)
{
	union REGS regs;
	struct SREGS sregs;
	int i;
	regs.h.ah=0;
	regs.x.ax=0x1012;
	regs.x.bx=0;
	regs.x.cx=256;
	regs.x.dx=FP_OFF(p);
	sregs.es=FP_SEG(p);
	int86x(0x10,&regs,&regs,&sregs);
}
				

	
	

