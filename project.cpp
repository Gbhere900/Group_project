#include <stdio.h>
#include <stdlib.h>
#include<conio.h>
#include<time.h>

int A = 1;
int x = 0, y = 0;
int mouse_x = 0, mouse_y = 0;		//老鼠坐标
int granary_x = 0, granary_y = 0;		//粮仓坐标


int maze[9][9];    //地图最大尺寸

struct best                  //用于记录最短路径的结构体
{
	int min;
	int b[9][9];
}Best;

int route_number;		//正确路线个数

