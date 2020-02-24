#include"parse.h"
void dectobin(unsigned char num, vector<unsigned char>& v)
{
	if (num / 2 != 0)
	{
		dectobin((unsigned char)(num / 2),v);
	}
	v.push_back(num % 2);
}

unsigned char* parseArray(unsigned char* src, long srcLen)//���ַ�����ת��Ϊ01��ʾ������8��
{
	unsigned char* dst = new unsigned char[srcLen *8];
	int i = 0;
	for (int j = 0; j <srcLen; j++)
	{
		vector<unsigned char> v;
		dectobin(src[j], v);
		if (v.size() != 8)
		{
			for (int k=0;k<8-v.size();k++)
			{
				dst[i++] = 0;
			}
		}
		for (int k = 0; k < v.size(); k++)
		{
			dst[i++] = v[k];
		}
	}
	return dst;
}

unsigned char* inverseParse(unsigned char* src, long srcLen)
{
	unsigned char* dst = new unsigned char[srcLen / 8];
	int index = 0;
	for (int i = 0; i < srcLen; i += 8)//��ѭ������01����
	{
		int k = 0;
		int ind = 1;
		for (int j = 7; j >= 0 && i + j < srcLen; j--)//��7λ������ת��Ϊ10����
		{
			k += (src[i + j] * ind);
			ind *= 2;
		}
		dst[index++] = (unsigned char)k;
	}
	return dst;
}