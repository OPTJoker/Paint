//
//  main.c
//  Print
//
//  Created by 张祥龙 on 2017/6/13.
//  Copyright © 2017年 Allen. All rights reserved.
//

#include <stdio.h>

#include <math.h>

#include <stdlib.h>

#include <stdbool.h>
#include <string.h>
#include <dirent.h>


#pragma mark < 我的函数 >

// 判断字符c是否是数字字符
bool isNUMBER(const char c) {
    if ('0'<=c && c<='9') {
        return true;
    }
    return false;
}

// 获取.ppm文件名中的数字
int getNo(const char *str){
    
    int cnt = 0;
    
    short len = strlen(str);
    
    char c = str[len-5];    // 倒数第五位是数字的开始位置 [xxx000.ppm]
    
    short i=0;  //从后往前的位数
    
    while (isNUMBER(c)) {
        
        if (i==0) {
            cnt = atoi(&c);     // a to int 函数是把字符转成int
        }else{
            cnt += pow(10, i)*atoi(&c);    //pow(a,b)函数是求a的b次方
        }
        
        c = str[len-5-++i];
    }
    
    return cnt;
}


/**
 计算数字位数
 
 @param num 给定数字
 @return 数字位数
 */
short bitOfNum(const int num) {
    
    int a=num;
    
    short cnt = 0;
    
    if (a==0) {
        return 1;
    }
    
    while (a) {
        a = a/10;
        cnt ++;
    }
    return cnt;
}

//反转字符串
char *reverse(char *s)
{
    char temp;
    char *p = s;    //p指向s的头部
    char *q = s;    //q指向s的尾部
    while(*q)
        ++q;
    q--;
    
    //交换移动指针，直到p和q交叉
    while(q > p)
    {
        temp = *p;
        *p++ = *q;
        *q-- = temp;
    }
    return s;
}

/*
 * 功能：整数转换为字符串
 * char s[] 的作用是存储整数的每一位
 */
char *itoa(int n)
{
    int i = 0,isNegative = 0;
    static char s[100];
    if((isNegative = n) < 0) //如果是负数，先转为正数
    {
        n = -n;
    }
    do      //从各位开始变为字符，直到最高位，最后应该反转
    {
        s[i++] = n%10 + '0';
        n = n/10;
    }while(n > 0);
    
    if(isNegative < 0)   //如果是负数，补上负号
    {
        s[i++] = '-';
    }
    s[i] = '\0';    //最后加上字符串结束符
    return reverse(s);
}



/**
 遍历文件夹，获取图片名字里面的最大数字

 @param pcDirName 文件夹名字
 @return 最大数字
 */
int getLastNum(const char *pcDirName)
{
    struct dirent *pstDirent = NULL;
    DIR* pDir = NULL;
    
    if (NULL == pcDirName)
    {
        return 0;
    }
    
    pDir = opendir(pcDirName);
    if (NULL == pDir)
    {
        return 0;
    }
    
    int n = 0;
    while (NULL != (pstDirent=readdir(pDir) )){
        
        char *name = pstDirent->d_name;
        int new = getNo(name);
        n = new>n ? new : n;
    }
    
    return n;
}

#pragma mark #比赛开始
#pragma mark #比赛开始

// 比赛已知条件
#define DIM 1024

#define DM1 (DIM-1)

#define _sq(x) ((x)*(x)) // square

#define _cb(x) abs((x)*(x)*(x)) // absolute value of cube

#define _cr(x) (unsigned char)(pow((x),1.0/3.0)) // cube root

unsigned char GR(int,int);

unsigned char BL(int,int);

// 红色
unsigned char RD(int i,int j){
    
#define r(n)(rand()%n)
    // 参赛者代码
    static char c[DIM][DIM];
    return (c[i][j]==0) ? c[i][j] = !r(999) ? r(256) : RD((i+r(2))%1024,(j+r(2))%1024) : c[i][j];
    
}

// 绿色
unsigned char GR(int i,int j){
    
    // 参赛者代码
    static char c[DIM][DIM];
    return!c[i][j]?c[i][j]=!r(999)?r(256):GR((i+r(2))%1024,(j+r(2))%1024):c[i][j];
    
}

// 蓝色
unsigned char BL(int i,int j){
    
    // 参赛者代码
    static char c[DIM][DIM];
    return!c[i][j]?c[i][j]=!r(999)?r(256):BL((i+r(2))%1024,(j+r(2))%1024):c[i][j];
    
}



void pixel_write(int,int);

FILE *fp;


/**
 算出并存储每一个像素的颜色值

 @param i 横坐标
 @param j 纵坐标
 */
void pixel_write(int i, int j){
    
    static unsigned char color[3];
    
    color[0] = RD(i,j)&255;
    
    color[1] = GR(i,j)&255;
    
    color[2] = BL(i,j)&255;
    
    fwrite(color, 1, 3, fp);
    
}


#pragma mark -MAIN-
int main(int argc, const char * argv[]) {
    
    // 要遍历的文件夹地址
    char *dir = "/Users/zhanglei/Desktop/Print/imgs/";  // 随便给一个存在的文件夹路径就行(必须存在，不然crash)
    
    // 基于上次图片名称里的最大数字，+1生成新图片的序号
    int num = getLastNum(dir)+1;
    
    // 文件路径+文件名前半截
    char *fileName = (char *)malloc( (strlen(dir) + strlen("img"))*sizeof(char) );
    
    strcat(fileName, dir);      // fileName = "/Users/zhanglei/Desktop/Print/imgs/"
    strcat(fileName, "img");    // fileName = "/Users/zhanglei/Desktop/Print/imgs/img"
    
    // 文件后缀名
    char *extention = ".ppm";
    
    // 要为新变量申请足够空间
    long s = sizeof(char) * (strlen(fileName)+bitOfNum(num)+strlen(extention));
    
    // 新图片地址字符串
    char *newAdd = (char *)malloc(s);
    
    // 新图片的序号 int转字符串
    char *numStr = itoa(num);
    
    // 拼接新地址字符串
    strcat(newAdd, fileName);   //  newAdd = "/Users/zhanglei/Desktop/Print/imgs/"
    strcat(newAdd, numStr);     //  newAdd = "/Users/zhanglei/Desktop/Print/imgs/img0"
    strcat(newAdd, extention);  //  newAdd = "/Users/zhanglei/Desktop/Print/imgs/img0.ppm"
    
    // 从新路径打开图片文件(如果不存在系统会创建。写入方式：覆写)
    fp = fopen(newAdd, "wb");
    
    
    /*  这里要存储的图片格式是.ppm
        PPM格式图片文件说明：
     netpbm的几种图片格式是通过其表示的颜色类型来区别的，PBM是位图，只有黑色和白色，PGM是灰度图片，PPM是代表完整的RGB颜色的图片。
     (1) 文件头
     文件头由三个部分（或者认为是四个部分）组成：这几个部分之间用回车或换行分隔（但是PPM标准中要求是空格）
     第一部分是文件magic number：
     每一个netpbm图片由两个字节的magic number (ASCII)组成，来标识文件的类型(PMB/PGM/PPM)以及文件的编码(ASCII或binary).
     
     所以PPM格式的起始两个字节为P3或者P6.
     关于编码(ASCII或binary): 其区别是ASCII编码的文件是对于阅读友好的，可以字节用文本编辑器打开，并读取其对应的图片的数据（比如RGB的值），然后中间会有空格回车等隔开。binary就是按照二进制的形式，顺序存储图片信息，没有空格回车分隔。所以很显然，binary格式的图片处理起来更快（不需要判断空格回车），而且图片会更小，但是ASCII阅读调试更为直接。
     第二部分是图像宽度和高度（空格隔开），用ASCII表示。
     第三部分是描述像素的最大颜色组成，允许描述超过一个字节（0-255）的颜色值。
     另外，在上面的三个部分里面，都可以使用"#"插入注释，注释是#到行尾(回车或换行)部分。
     
     (2) 图像数据部分
     对于ASCII格式，就是按照RGB的顺序排列，以ASCII存储，并且，RGB中间用空格隔开，图片每一行用回车隔开。
     对于binary格式，就是每一个像素点的RGB值分别顺序存储并且按二进制写入文件(fwrite)，没有任何分隔。
     比如下面这个图片 (一共六个像素点，图片宽度为3，高度为2）:
     */
    
    // 写入文件头：
    // 遵循PPM格式 4项内容(P6, 长, 宽, 颜色值)
    fprintf(fp, "P6\n%d %d\n255\n", DIM, DIM);
    
    // 写入文件体(图像数据部分):
    for(int j=0;j<DIM;j++)
        
        for(int i=0;i<DIM;i++)
            
            pixel_write(i,j);
    
    fclose(fp); // 关闭文件，会释放打开文件占用的内存空间
    
    return 0;
}
