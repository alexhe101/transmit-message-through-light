//����ͼƬ��ַ�������άvector
vector<vector<uchar> > img_decode(char* path)   //pathΪͼƬ·��
{
    Mat img = imread(path);                // ��ͼƬ����Mat������
//       ��ʾԭͼƬ
//       namedWindow("old", WINDOW_NORMAL);
//       imshow("old", img);
//      waitKey(0);
    int w = img.cols * img.channels();     //����Ϊ3ͨ�������Ҫ��ͼƬ��ͨ����
    int h = img.rows;

    vector<vector<uchar> > array(h, vector<uchar>(w));      //��ʼ����άvector
    for (int i = 0; i < h; i++)
    {
        uchar* inData = img.ptr<uchar>(i);            //ptrΪָ��ͼƬ����ָ�룬����iΪ����
        for (int j = 0; j < w; j++)
        {
            array[i][j] = inData[j];
        }
    }
    return array;
}
/*
//����ͼƬ��ַ���������
void decode(char* path)   //pathΪͼƬ·����bufΪ��������
{
    Mat img = imread(path);
 //   namedWindow("old", WINDOW_NORMAL);
//    imshow("old", img);
 //   waitKey(0);
    int w = img.cols*img.channels();     //����Ϊ3ͨ��
    int h = img.rows;


    if (img.isContinuous())     //���������������ԣ�������ͼ����ȫչ������һ��
    {
        w = w * h;
        h = 1;              //��Ϊһ��
    }
    uchar** buf = new uchar * [h];
    for (int i = 0; i < h; i++)
        buf[i] = new uchar[w];
    for (int i = 0; i < h; i++)
    {
        uchar* inData = img.ptr<uchar>(i);
        for (int j = 0; j < w; j++)
        {
            buf[i][j]= inData[j];
        }
    }
    for (int i = 0; i < h; i++)
      delete[] buf[i];
    delete[] buf;
}
*/