#define _CRT_SECURE_NO_WARNINGS

#include"ZipTool.h"
#include<ctime>

/*���Խ���:
10M���ļ���ʹ��ASCII��д��ʱ��ʹ��ZLIB�㷨�������аٷ�֮87��ѹ����
��ʹ��7z�㷨ʱ��������0.58��ѹ����*/
int main()
{
	unsigned char* src = new unsigned char[1024*1024*10];
	srand((unsigned)time(NULL));
	for (int i = 0; i < 1024 * 1024 * 10; i++)
	{
		src[i] = rand()%128;
	}
	unsigned long dstLen = 0;
	unsigned long srcLen = 1024 * 1024 * 10;
	unsigned char* dst = ZipTool::zip(src, srcLen, &dstLen);
	cout <<dstLen<<endl;
	//unsigned long unzipLen = 1024;
	//cout << dstLen<<endl;
	//unsigned char* unzipdest = ZipTool::unzip(dst, dstLen, &unzipLen);
	//cout << unzipLen<<endl;
	//for (int i = 0; i < 10; i++)
	//{
	//	printf_s("%d %d\n", src[i], unzipdest[i]);
	//}
	//return 0;
}