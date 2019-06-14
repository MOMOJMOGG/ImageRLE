#include <stdio.h>
#include <stdlib.h>
#include <time.h> // time 函數所需之標頭檔
#include "theimg.h"

int main()
{
	theimg *g_img = new theimg();
	double str_t, end_t;
	g_img->LoadBMPfile("lenaaaa.bmp");
	//printf("Img width : %d, Img height : %d\n", g_img->GetImgWidth(), g_img->GetImgHeight());
	//g_img->Show();
	//printf("Raw data address : 0x%p\n", g_img->GetRaw());
	//unsigned char *tmp = g_img->GetReverse();
	//unsigned char *tmp = g_img->Binary(g_img->GetRaw(), g_img->GetImgWidth(), g_img->GetImgHeight(),3);
	//g_img->ChangedRaw(tmp);
	//g_img->SetBMPInfo(g_img->GetImgWidth(), g_img->GetImgHeight());

	//unsigned char theraw[25] = { 20,180,180,20,180,
	//							 20,20,180,20,20,
	//							 20,180,20,180,180,
	//							 20,20,20,20,180,
	//							 180,180,180,20,180 };
	//unsigned char theraw[25] = {180,20,180,20,180,
	//							180,20,20,20,20,
	//							180,20,180,20,180,
	//							20,20,180,20,180,
	//							180,20,20,20,20};
	//unsigned char theraw[25] = { 20,20,20,20,20,
	//							 20,180,20,180,20,
	//							 20,20,20,20,20,
	//							 20,180,20,180,20,
	//							 20,20,20,20,20 };
	//unsigned char theraw[100] = { 99,99,99,99,99,99,99,99,99,99,
	//							  99,0,0,0,99,0,0,0,99,99,
	//							  99,0,0,99,99,99,0,0,99,99,
	//							  99,0,0,99,99,99,0,0,99,99,
	//							  99,99,0,99,99,99,0,0,99,0,
	//							  0,99,0,99,99,99,0,0,0,0,
	//							  0,0,0,99,99,99,99,99,99,0,
	//							  0,99,99,99,0,99,0,0,0,0,
	//							  0,99,99,0,0,99,0,0,99,99,
	//							  //99,99,99,99,99,99,99,99,99,99,
	//							  0,0,0,0,0,0,0,0,99,99
	//							  };
	//unsigned char theraw[225] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	//						  0,0,0,0,0,0,0,99,0,0,0,0,0,0,0,
	//						  0,0,0,0,0,0,99,0,0,0,0,0,0,0,0,
	//						  0,0,0,0,0,0,99,0,0,0,0,0,0,0,0,
	//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	//						  0,0,0,0,0,0,0,0,0,99,0,0,0,0,0,
	//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	//						  0,0,0,0,0,0,99,0,0,0,0,0,0,0,0,
	//						  0,0,0,0,0,99,0,0,0,0,0,0,0,0,0, 
	//						  0,0,0,99,99,0,0,0,0,0,0,0,0,0,0,
	//						  0,0,0,99,0,0,0,0,0,0,0,0,0,0,0, 
	//						  0,0,0,0,99,0,0,0,0,0,0,0,0,0,0, 
	//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
	//						  0,0,99,0,0,0,0,0,0,0,0,99,0,0,0};
	//unsigned char theraw[90] = { 99,99,99,99,99,99,99,99,99,
	//						  99,99,99,0,0,0,99,0,0,
	//						  99,99,0,0,0,0,0,99,99,
	//						  99,0,0,0,0,0,0,0,99,
	//						  99,0,0,0,0,0,0,0,99,
	//						  99,0,99,0,99,0,99,0,99,
	//						  99,0,99,0,99,0,99,0,99,
	//						  99,0,99,0,99,0,99,0,99,
	//						  99,0,99,0,99,0,99,0,99,
	//						  99,99,99,99,99,99,99,99,99
	//						  };

	RLEtable *theblob;
	str_t = clock();
	//theblob = g_img->BuildRLE(theraw, 5, 5, 50);
	//theblob = g_img->BuildRLE(theraw, 10, 10, 50);
	//theblob = g_img->BuildRLE(theraw, 15, 15, 50);
	//g_img->GetImgHeight()
	theblob = g_img->BuildRLE(g_img->GetRaw(), g_img->GetImgWidth(), g_img->GetImgHeight(), 128);
	//g_img->PrintBlob(theblob);

	ItemInfo *theItm = g_img->GetInfoFromBlob(theblob,40);
	//g_img->PrintITM(theItm);
	g_img->CleanItem(theItm);
	//
	end_t = clock();
	double diff_t = difftime(end_t, str_t);
	printf("Time = %f ms\n", diff_t);
	system("pause");
	//g_img->Savefile("sailsaaaa.bmp");
	system("pause");
	
	return 0;
}