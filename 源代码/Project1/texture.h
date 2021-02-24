#pragma once
#include<time.h>
#include<gl/glut.h>
#include<vector>
#include<iostream>

using namespace std;
unsigned int ID;
unsigned int bID;
unsigned int kID;
string name = "metal";

typedef unsigned char byte;

int LoadBitmap(const char* file)
{
    unsigned int ID;      //纹理的id
    int width, height, i;
    byte* image, t;          //接受图像数据
    FILE* fp;            //文件指针
    BITMAPFILEHEADER FileHeader;    //接受位图文件头
    BITMAPINFOHEADER InfoHeader;    //接受位图信息头

    fp = fopen(file, "rb");
    if (fp == NULL)
    {
        perror("LoadBitmap");        //打开文件失败
        return -1;
    }
    fread(&FileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
    if (FileHeader.bfType != 0x4D42)  //确保文件是一个位图文件，效验文件类型
    {
        printf("Error: This file is not a bmp file!");
        fclose(fp);
        return -1;
    }
    fread(&InfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);
    width = InfoHeader.biWidth;
    height = InfoHeader.biHeight;
    if (InfoHeader.biSizeImage == 0)          //确保图像数据的大小
    {
        InfoHeader.biSizeImage = width * height * 3;
    }
    fseek(fp, FileHeader.bfOffBits, SEEK_SET);  //将文件指针移动到实际图像数据处
    image = (byte*)malloc(sizeof(byte) * InfoHeader.biSizeImage); //申请空间
    if (image == NULL)
    {
        free(image);
        printf("Error: No enough space!");
        return -1;
    }
    fread(image, 1, InfoHeader.biSizeImage, fp);

    for (i = 0; i < InfoHeader.biSizeImage; i += 3)
    {
        t = image[i];
        image[i] = image[i + 2];
        image[i + 2] = t;
    }
    fclose(fp);
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width,
        height, GL_RGB, GL_UNSIGNED_BYTE,
        image);
    return ID;
}


