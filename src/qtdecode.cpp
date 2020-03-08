#include "qtdecode.h"
#include"Decode.h"
#include"FileConvert.h"
#include<iostream>
using namespace std;
using namespace cv;

void qtdecode::mydecode()
{
	int indexptr = 0;//���ڴ洢Ŀ������ĵ�ǰ�±�
	Decode dec;
	string path = "imageSrc\\";
	FileConvert converter;
	Mat img;
	int numOfPic = converter.GetFilesNumber(path);
	char filename[10];
	sprintf_s(filename, "%05d.png", numOfPic);//���һ��
	img = imread(path + filename);//�����ͼƬ��������
	int length;
	unsigned char* output = NULL;
	int type;
	unsigned char* tmp = dec.decode(img, length, type);//Ϊ�˻�ȡ���һ�ŵĳ���
	int total_length = (numOfPic - 1) * 1054 + length;
	//Mat image;//ԴͼƬ
	Mat dst;//��������
	int i, j;
	char outfilename[] = "out.bin";
	int count=0;//ͼ���ļ�����
	char fileName[128];

	cout << total_length << endl;
	cout << indexptr;

	if (type == SINGLE)//���ֻ�е���ͼ
	{
		output = tmp;//lengthΪ���ļ�����Ч���ݳ���
	}//�����˳�
	else //if (type == BEGIN)//����ͼƬ�ĵ�һ��ͼƬ
	{
		//lengthΪ���ļ��ĳ���
		int tmplen = MAXSIZE;
		output = new unsigned char[total_length];//���ڴ������ݵ�����
		//memcpy(output + indexptr, tmp, tmplen);//���Ƶ����յ�������
		//indexptr += tmplen;
		delete[]tmp;
	}//��������end��normal��

	int count = 0;
	int tmplen;
	while (true)
	{
		sprintf_s(fileName, "imageOutput\\%05d.png", count++);
		//image = imread(fileName);//����ͼ��
		if (img.data == NULL) break;
		//dec.findQranchor(image, dst);
		/*for (i = 0; i <= 96; i++)
		{
			for (j = 0; j <= 96; j++)
			{
				output
				fwrite;
			}
		}*/
		unsigned char* tmp = dec.decode(img, tmplen, type);
		if (tmplen > 1054) tmplen = 1054;
		cout << filename << " " << tmplen << endl;
		memcpy(output + indexptr, tmp, tmplen);//���Ƶ����յ�������
		indexptr += tmplen;
		delete[]tmp;
	}
	converter.ByteToFile(output, outfilename, total_length);

}