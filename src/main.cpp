#include<iostream>
#include"Decode.h"
#include"FileConvert.h"
#include<cstdio>
#include<cstdlib>
#include"code.h"
using namespace std;
using namespace cv;
int main()
{
	int function;
	cout << "function one:encode file to video" << endl;
	cout << "function two:decode video to file" << endl;
	cout << "Calculate bit error rate" << endl;
	cout << "input function you choose" << endl;
	cin >> function;
	FileConvert converter;
	if (function == 1)
	{
		//ѡ�������ַ���or�������������//�����ַ��������ļ��Ľӿڻ�û��д
		unsigned long size;
		cout << "Please input size :";
		cin >> size;//��Ҫ�����ļ��Ĵ�С����λ�ֽ�
		char filename[] = "in.bin";
		converter.GenerateRandFile(filename, size);//��������������ļ�
		unsigned char* bytes = converter.FileToByte(filename, &size);//��ȡ�ļ����ɵ�Byte����
		
		/*
		һ����bytes����ͼ��Ĵ���
//		������ʽΪ%05d.png
//		�����imageSrcĿ¼
//		*/
		Code::Main(bytes, size, "imageSrc\\", "png");

		int fps = 10;//������ÿ��10֡����
		converter.PicTransToVideo(fps);
		return 0;
	}	/*
//
//	���ֻ�¼����Ƶ������
//	*/
	else if (function == 2)
	{
		int indexptr = 0;//���ڴ洢Ŀ������ĵ�ǰ�±�
		Decode decoder;
		int length;
		converter.VideoTransToPic();
		unsigned char* output=NULL;
		int numOfPic=0;//��ȡimageOutputĿ¼���ļ�����
		Mat img;
		img = imread("imageOutput\\00001.png");//��0��ͼƬ��������
		int type = decoder.getType(img);
		if (type == SINGLE)//���ֻ�е���ͼ
		{
			output = decoder.decode(img, SINGLE, length);
		}
		else //if (type == BEGIN)//����ͼƬ�ĵ�һ��ͼƬ
		{
			Mat tmpimg;
			decoder.rotate(img, tmpimg);
			int tmplen;
			length = decoder.getLength(tmpimg);
			output = new unsigned char[length];//���ڴ������ݵ�����
			unsigned char* tmp = decoder.decode(tmpimg, type, tmplen);//����ǰ��ά��������ݴ���������
			memcpy(output + indexptr, tmp, tmplen);//���Ƶ����յ�������
			indexptr += tmplen;
			delete[]tmp;
		}//��������end��normal��


		for (int i = 2; i <= numOfPic; i++)
		{
			string path = "imageOutput\\";
			char filename[10];
			sprintf_s(filename, "%05d.png", i);
			img = imread(path + filename);
			type = decoder.getType(img);
			int tmplen;
			unsigned char* tmp = decoder.decode(img, type, tmplen);//����ǰ��ά��������ݴ���������
			memcpy(output + indexptr, tmp, tmplen);//���Ƶ����յ�������
			indexptr += tmplen;
			delete[]tmp;
		}//��������ͼƬ
		char filename[] = "out.bin";
		converter.ByteToFile(output, filename, length);

	}
	else if (function == 3)
	{
		FileConvert file;
		FILE *fp1, *fp2;
		unsigned long num1=0,num2=0;
		fopen_s(&fp1,"in.bin", "rb");
		fopen_s(&fp2,"out.bin", "rb");
		num1 = file.GetFileSize(fp1);
		num2 = file.GetFileSize(fp1);
		unsigned char* dst1 = new unsigned char[num1];
		unsigned char* dst2 = new unsigned char[num2];
		fread(dst1, sizeof(unsigned char), num1, fp1);
		fread(dst2, sizeof(unsigned char), num1, fp2);
		unsigned long totalnum=1, wrongnum=0,m=0;
		while (totalnum<=(num1+1))
		{
			totalnum++;
			if (dst1[m] != dst2[m]) wrongnum++;
			m++;
		}
		double n;
		n = 1 - (wrongnum / totalnum);
		cout << "bit error rate is " << n << endl;
		return 0;
	}
}

