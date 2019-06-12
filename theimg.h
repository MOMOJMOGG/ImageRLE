#ifndef THEIMG_H
#define THEIMG_H


#pragma once
#include "imgstruct.h"

class theimg
{
public:
	theimg();
	~theimg();

private:
	int m_width;
	int m_height;
	unsigned char *m_rawdate;
	BMP m_img;

public:
	int LoadBMPfile(const char *file);
	int Savefile(const char *file);
	int GetImgWidth();
	int GetImgHeight();
	unsigned char *GetRaw();
	unsigned char *GetColorTable();
	unsigned char *GetReverse();
	int ChangedRaw(unsigned char *tmpraw);
	int SetBMPInfo(int tarwidth, int tarheight);
	void Show();
	unsigned char *Binary(unsigned char *raw, int wid, int hei, int kbit);
	RLEtable *BuildRLE(unsigned char *raw, int wid,int hei, int threshold);
	RLEtable *FindNodeHead(RLEtable *curr, int mode, int pos);
	void ChangeNodetar(RLEtable *curr, int item, int val, int val2);
	void DeleteBlobNode(RLEtable *blobtar,int mode);
	void PrintBlob(RLEtable *blobhead);
	void PrintDebug(RLEtable *nodehead,int mode);
	ItemInfo *GetInfoFromBlob(RLEtable *blob, int filtersize, int wid, int hei);
	void InitItm(ItemInfo *ITM);
	void PrintITM(ItemInfo *ITM);
	void CleanNode(RLEtable *tar);
	void CleanItem(ItemInfo *theITM);
};
#endif // !THEIMG_H