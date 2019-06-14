#ifndef IMGSTRUCT_H
#define IMGSTRUCT_H



#pragma once

#pragma pack(push)
#pragma pack(2)

typedef struct tagbmpfileheader {
	unsigned short bfType;
	unsigned int   bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned int   bfOffset;
}bmpfileheader;

typedef struct tagbmpinfoheader {
	unsigned int biSize;
	int biwidth;
	int biheight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	unsigned int biCompression;
	unsigned int biImageSize;
	int biXPelsPerMeter;
	int biYPelsPerMeter;
	unsigned int biClrUsed;
	unsigned int biClrImportant;
}bmpinfoheader;


typedef struct tagbmpstruct {
	tagbmpfileheader bmpFhead;
	tagbmpinfoheader bmpIhead;
	unsigned char *bmpRGB;
}BMP;

#pragma pack(pop)

typedef struct RLEtb {
	int Index;
	int Xstart;
	int Xend;
	int Ypos;
	RLEtb *nextnode;
	RLEtb *beside;
	RLEtb *parenode;
	int count;
}RLEtable;

typedef struct the_p {
	int x;
	int y;
}the_point;

typedef struct iteminfo {
	the_p points[4];  // 0:xmin,1:xmax,2:ymin,3:ymax
	int targetNum;
	double theta;
	iteminfo *next;
}ItemInfo;

#endif // !IMGSTRUCT_H