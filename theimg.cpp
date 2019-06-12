#include "theimg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma region IMGstruct

theimg::theimg()
{
	m_rawdate = new unsigned char;
	m_img.bmpRGB = new unsigned char;
}

theimg::~theimg()
{
	delete m_rawdate;
	delete m_img.bmpRGB;
}

int theimg::LoadBMPfile(const char *file)
{
	FILE *f_op;
	f_op = NULL;


	unsigned short buf_short;
	unsigned int   buf_unint;
	int	buf_int;

	/** open file **/
	f_op = fopen(file, "rb");
	if (f_op == NULL)
	{
		printf("open file fail\n");
		return -1;
	}

	fseek(f_op, 0, SEEK_SET);
	
	/* Read File Header */
	fread(&buf_short, sizeof(unsigned short), 1, f_op);
	m_img.bmpFhead.bfType = buf_short;
	fread(&buf_unint, sizeof(unsigned int), 1, f_op);
	m_img.bmpFhead.bfSize = buf_unint;
	fread(&buf_short, sizeof(unsigned short), 1, f_op);
	m_img.bmpFhead.bfReserved1 = buf_short;
	fread(&buf_short, sizeof(unsigned short), 1, f_op);
	m_img.bmpFhead.bfReserved2 = buf_short;
	fread(&buf_unint, sizeof(unsigned int), 1, f_op);
	m_img.bmpFhead.bfOffset = buf_unint;

	/* Read Info Header */
	fread(&buf_unint, sizeof(unsigned int), 1, f_op);
	m_img.bmpIhead.biSize = buf_unint;
	fread(&buf_int, sizeof(int), 1, f_op);
	m_img.bmpIhead.biwidth = buf_int;
	fread(&buf_int, sizeof(int), 1, f_op);
	m_img.bmpIhead.biheight = buf_int;
	fread(&buf_short, sizeof(unsigned short), 1, f_op);
	m_img.bmpIhead.biPlanes = buf_short;
	fread(&buf_short, sizeof(unsigned short), 1, f_op);
	m_img.bmpIhead.biBitCount = buf_short;
	fread(&buf_unint, sizeof(unsigned int), 1, f_op);
	m_img.bmpIhead.biCompression = buf_unint;
	fread(&buf_unint, sizeof(unsigned int), 1, f_op);
	m_img.bmpIhead.biImageSize = buf_unint;
	fread(&buf_int, sizeof(int), 1, f_op);
	m_img.bmpIhead.biXPelsPerMeter = buf_int;
	fread(&buf_int, sizeof(int), 1, f_op);
	m_img.bmpIhead.biYPelsPerMeter = buf_int;
	fread(&buf_unint, sizeof(unsigned int), 1, f_op);
	m_img.bmpIhead.biClrUsed = buf_unint;
	fread(&buf_unint, sizeof(unsigned int), 1, f_op);
	m_img.bmpIhead.biClrImportant = buf_unint;

	

	/* Read Color Table */
	const int tmp_clr_size = (m_img.bmpFhead.bfOffset - sizeof(m_img.bmpFhead) - sizeof(m_img.bmpIhead));
	m_img.bmpRGB = (unsigned  char *)malloc(tmp_clr_size);
	if (m_img.bmpRGB == NULL)
	{
		printf("color alloc fail\n");
		return -1;
	}
	else
	{
		memset(m_img.bmpRGB, 0xFF, tmp_clr_size );
		fread(m_img.bmpRGB, tmp_clr_size, 1, f_op);
	}
	const int thesize16 = m_img.bmpIhead.biwidth * m_img.bmpIhead.biheight * 3;
	const int thesize8 = m_img.bmpIhead.biwidth * m_img.bmpIhead.biheight;
	if (m_img.bmpIhead.biBitCount > 8) {
		m_rawdate = (unsigned char *)malloc(thesize16);
		memset(m_rawdate, 0xFF, thesize16);
		if (m_rawdate == NULL) {
			printf("malloc images_s error\n");
		}
		fread(m_rawdate, thesize16, 1, f_op);
	}
	else if (m_img.bmpIhead.biBitCount <= 8) {
		m_rawdate = (unsigned char *)malloc(thesize8);

		memset(m_rawdate, 0xFF, thesize8);
		if (m_rawdate == NULL) {
			printf("malloc images_s error\n");
		}
		fread(m_rawdate, thesize8, 1, f_op);
	}

	m_width = m_img.bmpIhead.biwidth;
	m_height = m_img.bmpIhead.biheight;
	fclose(f_op);

	return 0;
}

int theimg::Savefile(const char *file)
{
	FILE *f_out;
	f_out = NULL;
	const int tmp_clr_size = (m_img.bmpFhead.bfOffset - sizeof(m_img.bmpFhead) - sizeof(m_img.bmpIhead));
	const int thesize16 = m_img.bmpIhead.biwidth * m_img.bmpIhead.biheight * 3;
	const int thesize8 = m_img.bmpIhead.biwidth * m_img.bmpIhead.biheight;

	if (!fopen(file, "rb"))//�ɮ׬O�_�s�b 
		f_out = fopen(file, "wb");//���s�b�s�بüg�J��� 
	else
		f_out = fopen(file, "rb+");//�s�b�hŪ���üg�J��� 


	fwrite(&m_img.bmpFhead, sizeof(m_img.bmpFhead), 1, f_out);
	fwrite(&m_img.bmpIhead, sizeof(m_img.bmpIhead), 1, f_out);

	fwrite(m_img.bmpRGB, tmp_clr_size, 1, f_out);
	
	if (m_img.bmpIhead.biBitCount > 8)
	{
		fseek(f_out, m_img.bmpFhead.bfOffset, SEEK_SET);
		fwrite(m_rawdate, thesize16, 1, f_out);
	}
	else if (m_img.bmpIhead.biBitCount <= 8)
	{
		fseek(f_out, m_img.bmpFhead.bfOffset, SEEK_SET);
		fwrite(m_rawdate, thesize8, 1, f_out);
	}
	fclose(f_out);
	return 0;
}

int theimg::GetImgWidth()
{
	return m_width;
}

int theimg::GetImgHeight()
{
	return m_height;
}

unsigned char*theimg::GetRaw()
{
	return m_rawdate;
}

unsigned char *theimg::GetColorTable()
{
	return m_img.bmpRGB;
}

unsigned char *theimg::GetReverse()
{
	const int thesize16 = m_img.bmpIhead.biwidth * m_img.bmpIhead.biheight * 3;
	const int thesize8 = m_img.bmpIhead.biwidth * m_img.bmpIhead.biheight;
	unsigned char *newraw = new unsigned char;
	if (m_img.bmpIhead.biBitCount > 8) {
		newraw = (unsigned char *)malloc(thesize16);
		memset(newraw, 0xFF, thesize16);
		if (newraw == NULL) {
			printf("malloc images_s error\n");
		}
	}
	else if (m_img.bmpIhead.biBitCount <= 8) {
		newraw = (unsigned char *)malloc(thesize8);
		memset(newraw, 0xFF, thesize8);
		if (newraw == NULL) {
			printf("malloc images_s error\n");
		}
	}

	for (int j = 0; j < m_height;j++)
	{
		for (int i = 0 ; i < m_width; i++)
		{
			//*(newraw + (m_width-1-i) +  (m_height-1-j)* m_width) = *(m_rawdate +i + j* m_width );
			*(newraw + i + (m_height - 1 - j)* m_width) = *(m_rawdate + i + j * m_width);
		}
	}
	return newraw;
}

int theimg::ChangedRaw(unsigned char *tmpraw)
{
	m_rawdate = tmpraw;
	return 0;
}

int theimg::SetBMPInfo(int tarwidth, int tarheight)
{
	m_img.bmpFhead.bfType = 0x4D42;
	m_img.bmpFhead.bfOffset = 1024 + 14 + 40;
	m_img.bmpFhead.bfReserved1 = 0;
	m_img.bmpFhead.bfReserved2 = 0;
	

	m_img.bmpIhead.biSize = 40;
	m_img.bmpIhead.biPlanes = 1;
	m_img.bmpIhead.biBitCount = 8;
	m_img.bmpIhead.biCompression = 0;
	m_img.bmpIhead.biImageSize = 0;
	m_img.bmpIhead.biXPelsPerMeter = 0;
	m_img.bmpIhead.biYPelsPerMeter = 0;
	m_img.bmpIhead.biClrUsed = 0;
	m_img.bmpIhead.biClrImportant = 0;
	
	m_img.bmpIhead.biwidth = tarwidth;
	m_img.bmpIhead.biheight = tarheight;
	m_img.bmpFhead.bfSize = m_img.bmpFhead.bfOffset + tarwidth * tarheight;

	/* Set Color Table */
	const int tmp_clr_size = (m_img.bmpFhead.bfOffset - sizeof(m_img.bmpFhead) - sizeof(m_img.bmpIhead));
	m_img.bmpRGB = (unsigned  char *)malloc(tmp_clr_size);
	if (m_img.bmpRGB == NULL)
	{
		printf("color alloc fail\n");
		return -1;
	}
	else
	{
		memset(m_img.bmpRGB, 0xFF, tmp_clr_size);
		for (int i = 0; i < 256; i++)
		{
			*(m_img.bmpRGB + i*4) = i;
			*(m_img.bmpRGB + i*4 + 1) = i;
			*(m_img.bmpRGB + i*4 + 2) = i;
			*(m_img.bmpRGB + i*4 + 3) = i;
		}
	}
	return 0;
}

unsigned char *theimg::Binary(unsigned char *raw, int wid, int hei, int kbit)
{
	unsigned char thress = 1;
	
	unsigned char *newraw = new unsigned char;
	newraw = (unsigned char *)malloc(wid*hei);
	for (int j = 0; j < hei; j++)
	{
		for (int i = 0; i < wid; i++)
		{
			int group = 256 / (thress << kbit);
			int tmp = *(raw + i + j * wid) / group;
			if (group*(tmp + 1) == 256)
				*(newraw + i + j * wid) = 255;
			else
				*(newraw + i + j * wid) = (unsigned char)(group*tmp);
		}
	}
	return newraw;
}

void theimg::Show()
{
	printf("Bitmap Type : %x\n", m_img.bmpFhead.bfType);
	printf("Bitmap Size : %d\n", m_img.bmpFhead.bfSize);
	printf("Bitmap Reserved : %d%d\n", m_img.bmpFhead.bfReserved1, m_img.bmpFhead.bfReserved2);
	printf("Bitmap Offset : %d\n", m_img.bmpFhead.bfOffset);

	printf("Bitmap Info Size : %d\n", m_img.bmpIhead.biSize);
	printf("Bitmap Image Width : %d\n", m_img.bmpIhead.biwidth);
	printf("Bitmap Image Height : %d\n", m_img.bmpIhead.biheight);
	printf("Bitmap Plane : %d\n", m_img.bmpIhead.biPlanes);
	printf("Bitmap BitCount : %d\n", m_img.bmpIhead.biBitCount);
	printf("Bitmap Compression : %d\n", m_img.bmpIhead.biCompression);
	printf("Bitmap ImageSize : %d\n", m_img.bmpIhead.biImageSize);
	printf("Bitmap XResolution : %d\n", m_img.bmpIhead.biXPelsPerMeter);
	printf("Bitmap YResolution : %d\n", m_img.bmpIhead.biYPelsPerMeter);
	printf("Bitmap Color Used : %d\n", m_img.bmpIhead.biClrUsed);
	printf("Bitmap Color Important : %d\n", m_img.bmpIhead.biClrImportant);

}

#pragma endregion IMGstruct

/**********************************************************************************************/

RLEtable *theimg::BuildRLE(unsigned char *raw, int wid,int hei,int threshold)
{
	// �W�hcol����, �Ӽhcol����, �Ȧs����1, �Ȧs����2, ��ƶ�����, ��ƶ��Ȧs����
	RLEtable *parentcol, *currcol,  *tmpnode1, *tmpnode2, *Blob, *tmpblob;   
	parentcol = new RLEtable{ 0,0,0,0,NULL,NULL,NULL,NULL,0 };    // ���Ыغc�l
	currcol = new RLEtable{ 0,0,0,0,NULL,NULL,NULL,NULL,0 };
	Blob = new RLEtable{ 0,0,0,0,NULL,NULL,NULL,NULL,0 };
	
	bool headexitflag = false;    // �_�l�`�I�X��
	int blobindex = 1;  

	for (int j = 0; j < hei; ++j)   // �}�l�v�����y
	{
		tmpnode2 = currcol;         // �Ӽhcol�_�l��}
		for (int i = 0; i < wid; ++i)
		{
#pragma region imgproce&colbuild
			if (*(raw + i + j * wid) > threshold) // �ճB
			{
				*(raw + i + j * wid) = 255;    // �G�Ȥ��ܥ�
				headexitflag = false;          // ��U�X��
			}
			else // �³B
			{
				*(raw + i + j * wid) = 0;      // �G�Ȥ��ܶ�
				if (!headexitflag) // �S���X�� �N���`�I�_�l�B
				{
					RLEtable *newnode = new RLEtable{ 0,i,i,j,NULL,NULL,NULL,NULL,0 };    // �s�W�`�I
					tmpnode2->beside = newnode;
					tmpnode2 = tmpnode2->beside;
					headexitflag = true;    // �߰_�`�I�_�I�X��
				}
				else 
				{
					tmpnode2->Xend = i;
				}
			}// �³B�B�z����
#pragma endregion imgproce&colbuild
		}// �汽�y����
		headexitflag = false;    // �N�����Y�_��
#pragma region connect&blob
		
		//PrintDebug(parentcol, 1);
		//PrintDebug(currcol, 0);

		tmpnode1 = parentcol->beside;    // �W�h�_�l��}
		tmpnode2 = currcol->beside;      // �Ӽh�_�l��}
		while (tmpnode2 != NULL)    // currcol�h ���`�I�s�b
		{
			while (tmpnode1 != NULL)    // parentcol�h ���`�I�s�b
			{
				if (tmpnode1->Xend >= tmpnode2->Xstart && tmpnode1->Xstart <= tmpnode2->Xend)  // ��q�u�q�ۥ�
				{
					tmpnode1->count += 1;        // �Q�s�����ƭp��
					tmpnode2->count += 1;

					if (tmpnode2->count == 1) // m�r��, I�r��, �ˤ髬���U
					{
						if (tmpnode1->nextnode == NULL) // I�r��
						{
							tmpnode1->nextnode = tmpnode2;
							tmpnode2->parenode = tmpnode1;
							tmpnode2->Index = tmpnode1->Index;             // ��sBlob�s��
							// �Ҧ� 3 : �`�I, �Ҧ�, �첾�q, �W�[�q
							ChangeNodetar(Blob, 3, tmpnode2->Index, 1);    // ��sBlob �� size �j�p
						}
						else
						{
							if (tmpnode1->count == 1) // �ˤ髬���U
							{
								// �Ҧ� 3 : �`�I, �Ҧ�(next), �첾�q
								tmpblob = FindNodeHead(tmpnode1, 3, NULL); // ��̫�@�I
								tmpblob->nextnode = tmpnode2;
								tmpnode2->parenode = tmpblob;
								tmpnode2->Index = tmpblob->Index;             // ��sBlob�s��
								// �Ҧ� 3 : �`�I, �Ҧ�, �첾�q, �W�[�q
								ChangeNodetar(Blob, 3, tmpnode2->Index, 1);    // ��sBlob �� size �j�p
							}
							else // m�r��
							{
								// �Ҧ� 3 : �`�I, �Ҧ�(next), �첾�q
								tmpblob = FindNodeHead(tmpnode1, 3, tmpnode1->count-1);
								if (tmpblob->nextnode != NULL)
								{
									tmpblob->nextnode->parenode = tmpnode2;
									tmpnode2->nextnode = tmpblob->nextnode;
								}
								tmpblob->nextnode = tmpnode2;
								tmpnode2->parenode = tmpblob;
								tmpnode2->Index = tmpblob->Index;              // ��sBlob�s��
								ChangeNodetar(Blob, 3, tmpnode2->Index, 1);    // ��sBlob �� size �j�p
							}
						}
					}
					else // U�r��, �L��										///////�ˤ髬����, �ˤ髬�k�U
					{
						if (tmpnode1->nextnode == NULL) // U�r��
						{
							// �Ҧ� 1 : �`�I, �Ҧ�(parent), �첾�q
							tmpblob = FindNodeHead(tmpnode1, 1, NULL);         // ���Blob�ꭺ�I
							tmpnode2->parenode->nextnode = tmpblob->nextnode;  // ����OBlob��U�@�I
							tmpblob->nextnode->parenode = tmpnode2->parenode;
							// �Ҧ� 2 : �`�I, �Ҧ�, ��s�q(��Index)
							ChangeNodetar(tmpblob->nextnode, 2, tmpnode2->parenode->Index, NULL);  // ��s�쥻Blob��s��(�@�@) 
							ChangeNodetar(Blob, 3, tmpnode2->Index, tmpblob->Ypos);    // ��sBlob �� size �j�p
							tmpblob->nextnode = NULL;      // �N �쥻Blob �쵲�_�}
							DeleteBlobNode(tmpblob, 0);       // �R��blob�L�θ`�I
							tmpnode1->nextnode = tmpnode2;
							tmpnode2->parenode = tmpnode1;
						}
					}
				}
				tmpnode1 = tmpnode1->beside;    // parecol �k�첾
			} // �W�h���L�@��
			tmpnode2 = tmpnode2->beside;        // currcol �k�첾
			tmpnode1 = parentcol->beside;       // parecol ���m
		} // �����s�����Y

#pragma endregion connect&blob

#pragma region newblob
		tmpnode2 = currcol->beside;
		while (tmpnode2 != NULL) // ���ycurrcol �P�_�O�_��Root�`�I
		{
			if (tmpnode2->count == 0)
			{   // Blob format : Blob�s��, �L, �L, �`�@�X���϶�(size), �U, �k, ��, �W, �L
				RLEtable *newnode = new RLEtable{ blobindex,0,0,1,NULL,NULL,NULL,NULL,0 };
				// �Ҧ� 2 : �`�I, �Ҧ�(beside), �첾�q
				tmpblob = FindNodeHead(Blob, 2, NULL);    // ���Blob�̥k�B�i�洡�J
				tmpblob->beside = newnode;                // Blob�쵲(�k��)
				newnode->preside = tmpblob;
				newnode->nextnode = tmpnode2;             // �P�_�l�`�I�쵲(�U�W)
				tmpnode2->parenode = newnode;
				tmpnode2->Index = newnode->Index;         // �]�wBlob�s��
				blobindex += 1;
			}
			tmpnode2 = tmpnode2->beside;                  // currcol �k�첾
		}
#pragma endregion newblob

		// �Ҧ� 1 : �`�I, �Ҧ�, ��s�q(��count)
		ChangeNodetar(currcol, 1, 0, 0); // �M��currcol �� count
		parentcol->beside = currcol->beside;  // �Ӽh��T���ܤW�hcol
		currcol->beside = NULL;

	}// �C���y����

	if (parentcol != NULL)
		CleanNode(parentcol);
	if (currcol != NULL)
		CleanNode(currcol);

	return Blob;
}

void theimg::CleanNode(RLEtable *tar)
{
	tar->beside = NULL;
	tar->preside = NULL;
	tar->nextnode = NULL;
	tar->parenode = NULL;
	delete tar;
}

RLEtable *theimg::FindNodeHead(RLEtable *curr, int mode, int pos)// mode 1 : parenode, mode 2 besidenode, mode 3 : nextnode 
{
	switch (mode)
	{
	case 1:
		curr = curr->parenode;
		while (curr->parenode != NULL)
		{
			curr = curr->parenode;
		}
		break;
	case 2:
		while (curr->beside != NULL)
		{
			curr = curr->beside;
		}
		break;
	case 3:
		if (pos == NULL)
		{
			curr = curr->nextnode;
			while (curr->nextnode != NULL)
			{
				curr = curr->nextnode;
			}
		}
		else
		{
			for (int i = 0; i < pos; i++)
			{
				curr = curr->nextnode;
			}
		}
		break;
	default:
		break;
	}
	return curr;
}

void theimg::ChangeNodetar(RLEtable *curr, int item, int val, int val2)
{
	switch (item)
	{
	case 1:
		while (curr->beside != NULL)
		{
			curr = curr->beside;
			curr->count = val;
		}
		break;
	case 2:
		while (curr != NULL)
		{
			curr->Index = val;
			curr = curr->nextnode;
		}
		break;
	case 3:
		while (curr != NULL)
		{
			if (curr->Index == val)
			{
				curr->Ypos += val2;
				return ;
			}
			curr = curr->beside;
		}
		break;
	default:
		break;
	}
}

void theimg::DeleteBlobNode(RLEtable *blobtar, int mode)
{
	if (mode == 0)
	{
		RLEtable *prenode = blobtar->preside;
		prenode->beside = blobtar->beside;
		if (blobtar->beside != NULL)
			blobtar->beside->preside = prenode;
		CleanNode(blobtar);
	}
	else
	{
		RLEtable *prenode = blobtar->parenode;
		prenode->nextnode = blobtar->nextnode;
		if (blobtar->nextnode != NULL)
			blobtar->nextnode->parenode = prenode;
		CleanNode(blobtar);
	}
}


void theimg::PrintBlob(RLEtable *blobhead)
{
	RLEtable *tmpblob, *tmpnode;
	tmpblob = blobhead->beside;
	while (tmpblob != NULL)
	{
		printf("<< Blobnode %d , nodeSize : %d >>\n", tmpblob->Index, tmpblob->Ypos);
		tmpnode = tmpblob->nextnode;
		while (tmpnode != NULL)
		{
			printf("Blobnum: %d, Xs: %d, Xe: %d, Y: %d, Cnt: %d\n", tmpnode->Index, tmpnode->Xstart, tmpnode->Xend, tmpnode->Ypos, tmpnode->count);
			tmpnode = tmpnode->nextnode;
		}
		tmpblob = tmpblob->beside;
	}
}

void theimg::PrintDebug(RLEtable *nodehead, int mode)
{
	RLEtable *tmpnode;
	tmpnode = nodehead->beside;
	if (mode == 1)// parecol
	{
		printf("Pare->\n");
		while (tmpnode != NULL)
		{
			printf("BLOB: %d, Xs: %d, Xe: %d, Y: %d, Cnt: %d\n", tmpnode->Index, tmpnode->Xstart, tmpnode->Xend, tmpnode->Ypos, tmpnode->count);
			tmpnode = tmpnode->beside;
		}
	}
	else// currcol
	{
		printf("Curr->\n");
		while (tmpnode != NULL)
		{
			printf("BLOB: %d, Xs: %d, Xe: %d, Y: %d, Cnt: %d\n", tmpnode->Index, tmpnode->Xstart, tmpnode->Xend, tmpnode->Ypos, tmpnode->count);
			tmpnode = tmpnode->beside;
		}
	}
	
}

ItemInfo *theimg::GetInfoFromBlob(RLEtable *blob, int filtersize, int wid, int hei)
{
	RLEtable *tmp1, *tmp2;
	int xmin = 0, xmax = 0,ymin = 0, ymax = 0;
	//double xsum = 0.0, ysum = 0.0;
	ItemInfo *itmnode, *tmpitmnode;
	itmnode = new ItemInfo{ {{0,0},{0,0},{0,0},{0,0}},0,0.0,NULL };   // ���Ыغc�l
	//InitItm(itmnode);
	tmpitmnode = itmnode;

	bool initflag = true;

	tmp1 = blob->beside;
	while (tmp1 != NULL)
	{
		if (tmp1->Ypos < filtersize)
		{
			while (tmp1->nextnode != NULL) // �M���U�`�I
			{
				tmp2 = tmp1->nextnode;
				DeleteBlobNode(tmp2, 1);
			}
			DeleteBlobNode(tmp1, 0); // �M��blob�`�I
			tmp1 = blob->beside;
		}
		else
		{
			tmp2 = tmp1->nextnode;
			while (tmp2 != NULL)
			{
				if (initflag) // �Ĥ@���n�и`�I
				{
					ItemInfo *newitm = new ItemInfo{ {{0,0},{0,0},{0,0},{0,0}},0,0.0,NULL }; ;
					//InitItm(newitm);
					tmpitmnode->next = newitm;
					itmnode->targetNum += 1;
					tmpitmnode = tmpitmnode->next;
					initflag = false;

					xmin = tmp2->Xstart;
					xmax = tmp2->Xend;
					ymin = tmp2->Ypos;
					ymax = tmp2->Ypos;
				}

				if (xmin > tmp2->Xstart)
				{
					xmin = tmp2->Xstart;
					tmpitmnode->points[0].x = tmp2->Xstart;
					tmpitmnode->points[0].y = tmp2->Ypos;
				}
				if (xmax < tmp2->Xend)
				{
					xmax = tmp2->Xend;
					tmpitmnode->points[1].x = tmp2->Xend;
					tmpitmnode->points[1].y = tmp2->Ypos;
				}
				if (ymin > tmp2->Ypos)
				{
					ymin = tmp2->Ypos;
					tmpitmnode->points[2].x = (tmp2->Xstart + tmp2->Xend) / 2;
					tmpitmnode->points[2].y = tmp2->Ypos;
				}
				if (ymax < tmp2->Ypos)
				{
					ymax = tmp2->Ypos;
					tmpitmnode->points[3].x = (tmp2->Xstart + tmp2->Xend) / 2;
					tmpitmnode->points[3].y = tmp2->Ypos;
				}
					
				// x���W , y ���W
				//xsum = ((tmpnode3->Xstart + tmpnode3->Xend)/wid)*(tmpnode3->Xend - tmpnode3->Xend + 1) / 2 + xsum;
				//ysum = (tmpnode3->Xend - tmpnode3->Xend + 1)*((tmpnode3->Ypos) / hei) + ysum;
				DeleteBlobNode(tmp2, 1); // �M�Ŧ��`�I
				tmp2 = tmp1->nextnode;
			}

			// xsum �B�z
			// ysum �B�z
			// tmpitmnode->theta == ?
			DeleteBlobNode(tmp1, 0);
			tmp1 = blob->beside;
		}
		// ��l��
		//xsum = 0.0;
		//ysum = 0.0;
		initflag = true;
	}
	if (blob != NULL)		// �M��blob
		CleanNode(blob);

	return itmnode;
}

void theimg::InitItm(ItemInfo *ITM)
{
	for (int pp = 0; pp < 4; pp++)
	{
		ITM->points[pp].x = 0;
		ITM->points[pp].y = 0;
	}
	ITM->next = NULL;
	ITM->targetNum = 0;
	ITM->theta = 0.0;
}

void theimg::CleanItem(ItemInfo *theITM)
{
	ItemInfo *pre = theITM->next;
	while (pre != NULL)
	{
		theITM->next = pre->next;
		pre->next = NULL;
		delete pre;
		pre = theITM->next;
	}
	if (theITM != NULL)
		delete theITM;
	
}

void theimg::PrintITM(ItemInfo *ITM)
{
	printf("<<ITM Head Size : %d >>\n", ITM->targetNum);
	ITM = ITM->next;
	while (ITM != NULL)
	{
		printf("[xmin:( %d, %d), ymin:( %d, %d), xmax:( %d, %d), ymax:( %d, %d)]\n", 
			ITM->points[0].x, ITM->points[0].y,
			ITM->points[2].x, ITM->points[2].y,
			ITM->points[1].x, ITM->points[1].y,
			ITM->points[3].x, ITM->points[3].y);
		ITM = ITM->next;
	}
}