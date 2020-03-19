#include "Qrdecode.h"
#include"Decode.h"
#include"FileConvert.h"
#include<iostream>
using namespace std;
using namespace cv;


void Qrdecode::mydecode()
{
	char fileName[128];
	char tmpName[128];
	int indexptr = 0;//���ڴ洢Ŀ������ĵ�ǰ�±�
	Decode dec;
	string path = "imageOutput\\";
	FileConvert converter;
	//converter.VideoTransToPic();
	/*
	�Ȱ�����ͼƬת��ΪROI
	*/
	int count = 1;
	Mat img;
	Mat dst;
	int foundindex = 88;
/*	while (true)
	{
		sprintf_s(fileName, "imageOutput\\%05d.png", count++);
		img = imread(fileName);
		if (img.data == NULL) break;
		int found = dec.findQranchor(img, dst);
		if (found == 1)
		{
			foundindex = count - 1;
			sprintf_s(tmpName, "imageTmp\\%05d.png", foundindex);
			imwrite(tmpName, dst);
			cout << "find anchor " << fileName << endl;
		}
		else
		{
			cout << "not found" << fileName;
		}
	}
	*/
	//foundindex = 87;
	//����ÿ��ͼƬ�����ͬ�����Ƿ����仯
		//������Ҫ�����ִ��봫��array�У���ȡ��ά������
	vector<unsigned char> valid;
	vector<string> fileNames;
	int currentFlag = 0;
	int originFlag = 0;
	char beginPic[120];
	sprintf_s(fileName, "imageTmp\\%05d.png", 1);
	fileNames.push_back(fileName);
	img = imread(fileName);
	currentFlag = dec.getFlag(img);
	originFlag = currentFlag;
	for (int i = 2; i <= foundindex; i++)
	{
		sprintf_s(fileName, "imageTmp\\%05d.png", i);
		img = imread(fileName);
		if (img.data == NULL) continue;
		currentFlag = dec.getFlag(img);
		if (currentFlag == 2 && originFlag == 1 || currentFlag == 1 && originFlag == 2)
		{
			if (dec.getRate(img) - 0.51 < 0)
			{
				fileNames.push_back(fileName);
				originFlag = currentFlag;
			}
		}
		else
		{
			continue;
		}

	}



	int numOfPic = fileNames.size();//converter.GetFilesNumber(path);
	cout << "num of true pic " << numOfPic << endl;
	char filename[10];
	///sprintf_s(filename, "%05d.png", numOfPic);//���һ��
	img = imread(fileNames[numOfPic - 1]);//�����ͼƬ��������
	int length;
	unsigned char* output = NULL;
	int type;
	unsigned char* tmp = dec.decode(img, length, type, valid);//Ϊ�˻�ȡ���һ�ŵĳ���
	int total_length = (numOfPic - 1) * 1018 + length;
	//Mat image;//ԴͼƬ
	int i, j;
	char outfilename[] = "out.bin";
	count = 0;//ͼ���ļ�����


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

	count = 0;
	int tmplen;
	valid.clear();
	while (true)
	{
		if (count == numOfPic) break;
		//sprintf_s(fileName, "imageOutput\\%05d.png", count++);
		img = imread(fileNames[count++]);//����ͼ��
		unsigned char* tmp = dec.decode(img, tmplen, type, valid);
		if (tmplen > 1018) tmplen = 1018;
		cout << "dealing with " << fileNames[count - 1] << " " << tmplen << endl;
		memcpy(output + indexptr, tmp, tmplen);//���Ƶ����յ�������
		indexptr += tmplen;
		delete[]tmp;
	}
	while (valid.size()<total_length)
	{
		valid.push_back(0xff);
	}
	converter.ByteToFile(output, outfilename, total_length);
	unsigned char* v = new unsigned char[valid.size()* sizeof(unsigned char)];
	memcpy(v, &valid[0], valid.size() * sizeof(unsigned char));
	converter.ByteToFile(output, outfilename, total_length);

	char vfname[] = "v.bin";
	converter.ByteToFile(v, vfname, total_length);
}