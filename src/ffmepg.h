#include<iostream>
#include<windows.h>
#include<string>
#include<stdio.h>
#include<tchar.h>

using namespace std;

class ffmepg
{
private:
	char fp_out1[100];//ͼƬת��Ƶ�����������Ƶ�ļ�������
	char fp_in2[100];//��ƵתͼƬ������������Ƶ�ļ�������

	
public:
	void picture_ts_video(int pic_num_persec)//��ͼƬת��Ϊ��Ƶ����������Ƶ֡��
	{
		string str1, str2, str3;
		char str4[5], str6[200];
		char a[50], b[50];
		sprintf_s(str4, "%d", pic_num_persec);
		cin >> fp_out1;
		str1 = "ffmpeg -f image2 -r ";
		str2 = " -i %d.jpg -vcodec mpeg4 ";
		str3 = str1 + str4 + str2 + fp_out1;
		sprintf_s(str6, "%s", str3.c_str());
		system(str6);
	}
	void video_ts_picture()//����Ƶת��ΪͼƬ
	{
		string str1, str2, str3, str5;
		char str6[200];
		str1 = "ffmpeg -i ";
		str2 = " %d.png";
		cin >> fp_in2;
		str3 = str1 + fp_in2 + str2;
		sprintf_s(str6, "%s", str3.c_str());
		system(str6);
	}

};


