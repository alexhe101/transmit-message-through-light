#include<iostream>

#include"Decode.h"
#include"FileConvert.h"
#include<cstdio>
#include<cstdlib>
#include"code.h"

#include<algorithm>
#include"Qrdecode.h"
#define CRC_CCITT 0x1021
using namespace std;
using namespace cv;

int main()
{

	int fps = 10;
	FileConvert converter;

	string path = "imageSrc\\";
	int function;
	cout << "function one:encode file to video" << endl;
	cout << "function two:decode video to file" << endl;
	cout << "Calculate bit error rate" << endl;
	cout << "input function you choose" << endl;
	cin >> function;
	if (function == 1)
	{
		//选择输入字符串or生成随机比特流//根据字符串生成文件的接口还没有写
		unsigned char* bytes = NULL;
		unsigned long size;
		cout << "Please input size :";
		char filename[] = "in.bin";
		cin >> size;//需要生成文件的大小，单位字节
		converter.GenerateRandFile(filename, size);
		bytes = converter.FileToByte(filename, &size);//读取文件生成的Byte数组	

		Code::Main(bytes, size, "imageSrc\\", "png", 1);  // tag：0裁剪图，1标准图

		converter.PicTransToVideo(fps);
		return 0;
	}
	//
	//	用手机录制视频并拍摄

	else if (function == 2)
	{
		Qrdecode mydecode;
		mydecode.mydecode();
	}
	else if (function == 3)
	{
		char inname[] = "out.bin";
		char outname[] = "in.bin";
		unsigned long inSize, outSize= 0;
		unsigned char* inByte = converter.FileToByte(inname, &inSize);
		unsigned char* outByte = converter.FileToByte(outname, &outSize);
		unsigned long totalnum = 1, wrongnum = 0;
		int cnt = 0;
		while (cnt++ < min(inSize, outSize))
		{
			totalnum++;
			if (inByte[cnt]!= outByte[cnt]) wrongnum++;
		}
		double n;
		n = ((1.0*wrongnum) / totalnum);
		cout << "bit error rate is " << n << endl;
		return 0;
	}
}