
//�����άvecotr����ʾ�����ͼƬ��������ͼƬ��ָ����ַ
void img_code(vector<vector<uchar> > array, char* path)
{
    size_t h = array.size();                  
    size_t w = array[0].size();
    //��ʼ��ͼƬ�����س���
    Mat img(h, (size_t)(w/3), CV_8UC3);           //����ΪRGB��ͼ����������Ҫ����3��
    for (size_t i = 0; i < h; i++)
    {
        uchar* outData = img.ptr<uchar>(i);
        for (size_t j = 0; j < w; j++)
        {
             outData[j] = array[i][j];
        }
    }
    namedWindow("new", WINDOW_NORMAL);
    imshow("new", img);
    waitKey(0);
    imwrite("Ŀ���ַ\\ͼƬ", img);         //���ͼƬ
}