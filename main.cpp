#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <conio.h>
#include <ctime>

using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include<conio.h>
#include<time.h>



int A = 1;
int map_size = 1;    //地图尺寸
int mouse_x = 0, mouse_y = 0;   //老鼠坐标
int granary_x = 0, granary_y = 0;  //粮仓坐标
int maze[9][9];     //地图最大尺寸

struct best  //用于记录最短路径
{
	int min;
	int b[9][9];
}best_path;      

int route_number;  //正确路线个数
int g;


//打印地图
print_map(int a)
{
	for (int i = 0; i < A; i++)
	{
		for (int j = 0; j < A; j++)
		{
			if (maze[i][j] == 2)
				cout << "■" ;  //■代表墙体
			else if (maze[i][j] == 1)
				cout << "鼠" ;  //代表老鼠
			else if (maze[i][j] == 3)
				cout << "粮" ;  //代表粮食
			else
				cout << "  " ;
		}
		cout << endl ;
	}
}

//新建地图，并存入文件
void create_plat(int a)
{
	A = a;

	int i, j, s;
	int input_correct = 0; //不用goto语句实现用户输入错误时的返回

    while(! input_correct = 0)
	{
		//2代表墙体，0代表通路
		cout << "请输入0和2,0代表通路，2代表墙体(数字用空格隔开)，输入规格是1×1" << endl;
	    for (i = 1; i <= A; i++)
	    {
	      	cout << "第" << i << "行";
	    	for (j = 1; j <= A; j++)
		    {
	    		cin >> s ;
	    		if (s == 0 || s == 2)
				maze[i-1][j-1] = s;
		    	else
			    {
			    	input_correct = 0 ;
			    	system("cls");
			    	cout << "——————输入错误，请重新输入——————" << endl ;
			    }
		    }
			if (!input_correct) break;
	    }
  
	}
	
    while(! input_correct = 0)
	{
		//1代表老鼠
		cout << "请设置老鼠的初始位置(x, y),注意x,y取值范围为1~" << a << endl ;

	    //清除来自键盘的缓存字符，防止死循环bug
	    for (i = 0; i < 30; i++)
	    	fflush(stdin);			//flush一下

	    cin >> mouse_x >> mouse_y ;
	    if (mouse_x <= A && mouse_y <= A && mouse_x > 0 && mouse_y > 0)
	    	maze[mouse_x - 1][mouse_y - 1] = 1;
	    else
	    {
	    	input_correct = 0 ;
			system("cls");
	    	cout << "——————输入错误，请重新输入——————" << endl ;
	    }
	}

    while(! input_correct = 0)
	{
		//3代表粮食
		cout << "请设置粮仓的位置(x, y):" << endl ;

	    //清除来自键盘的缓存字符，防止死循环bug
	    for (i = 0; i < 30; i++)
	    	fflush(stdin);			//flush一下

	    cin >> granary_x >> granary_y ;
	    if (granary_x <= A && granary_y <= A && granary_x > 0 && granary_y > 0)
	    	maze[granary_x - 1][granary_y - 1] = 3;
	    else
	    {
	    	input_correct = 0 ;
			system("cls");
	    	cout << "——————输入错误，请重新输入——————" << endl ;
	    }
	}

	//保存用户创建地图到文件
	FILE *fp;
	fp = fopen("map.txt", "w");	   //w只写，会清除再写
	for (i = 0; i < A; i++)
	{
		for (j = 0; j < A; j++) 
		{
			fprintf << (fp, "%d\t", maze[i][j]);
		}
		fprintf << (fp, "%c", '\n');
	}
	fprintf << (fp, "%d\t", p);
	fprintf << (fp, "%d\t", q);
	fprintf << (fp, "%d\t", x);
	fprintf << (fp, "%d\t", y);
	fprintf << (fp, "%d\t", A);
	fclose(fp);

	cout << "——————新建地图完成,保存成功！—————" << endl ;
	system("pause");
}

//从文件中获取地图，读入程序
void get_map(int a)
{
	A = a;
	int i, j;
	FILE *fp;
	fp = fopen("map.txt", "r");
	if (!fp)
	{
		cout << "文件不存在，请重新打开" << endl ;
		system("pause");
	}
	else
	{
		for (i = 0; i < A; i++)
		{
			for (j = 0; j < A; j++)
			{
				fscanf(fp, "%d\t", &maze[i][j]);
			}
			fscanf(fp, "\n");
		}
		fscanf(fp, "%d\t", &p);
		fscanf(fp, "%d\t", &q);    //读取老鼠的位置
		fscanf(fp, "%d\t", &x);
		fscanf(fp, "%d\t", &y);    //读取粮仓的位置
		fscanf(fp, "%d\t", &A);
		cout << ("��ȡ�ɹ�����鿴�µ�ͼ��\n");
		print_map(A);
		system("pause");
	}
	fclose(fp);

}

//修改地图模块
void alter_map()
{
	int i, j, select, a, b;
	FILE *fp;
	while (1)
	{
		system("cls");

		print_map(A); //��ʾ��ͼ

		cout << " =============————修改地图————=============" << endl ;
		cout << " |请选择具体内容：                              |" << endl
		cout << " |              1.修改为墙体                    |" << endl ;
		cout << " |              2.修改为通路                    |" << endl ;
		cout << " |              3.保存修改地图                  |" << endl ;
		cout << " |              0.退出修改功能                  |" << endl ;
		cout << " =========（请输入相应数字执行其功能）========="<< endl ;
		fflush(stdin);  //清除键入的缓存
		cin >> select;

		if (select < 4 && select >= 0)
		{
			switch (select)
			{
			case 1:	cout << "围墙内为修改范围，注意范围为" << A << 'x' << A << endl ; 
				cout << "请输入要修改的地图坐标(x, y):" << endl ;
				//清除来自键盘的缓存字符，防止死循环bug
				for (i = 0; i < 30; i++)
					fflush(stdin);
				cin >> a >> b ;
				if (a <= A && b <= A && a > 0 && b > 0 && maze[a][b] != 1 && maze[a][b] != 3)
					maze[a-1][b-1] = 2;
				else
				{
					cout << "不能在围墙、粮仓和老鼠的位置修改！请重新输入：" << endl ;
					system("pause");
				}
				break;
			case 2:	cout << "围墙内为修改范围，注意范围为" << A << 'x' << A << endl ; 
				cout << "请输入要修改的地图坐标(x, y):" << endl ;
				//清除来自键盘的缓存字符，防止死循环bug
				for (i = 0; i < 30; i++)
					fflush(stdin);
				cin >> a >> b ;
				if (a <= A && b <= A && a > 0 && b > 0 && maze[a][b] != 1 && maze[a][b] != 3)
					maze[a-1][b-1] = 0;
				else
				{
					cout << "不能在围墙、粮仓和老鼠的位置修改！请重新输入：" << endl ;
					system("pause");
				}
				break;
			case 3:
			{	
				//修改后地图保存到文件
				fp = fopen("map.txt", "w");
				for (i = 0; i < A; i++)
				{
					for (j = 0; j < A; j++)
						fprintf << (fp, "%d\t", maze[i][j]);
					fprintf << (fp, "%c", '\n');
				}
				fprintf << (fp, "%d\t", p);
				fprintf << (fp, "%d\t", q);
				fprintf << (fp, "%d\t", x);
				fprintf << (fp, "%d\t", y);
				fclose(fp);

				cout << "——————地图修改完成,保存成功！—————" << endl ;
				system("pause");
			}
			break;
			case 0: break;
			}
		}
		else
		{
			cout << "——————请按规定输入！—————" << endl ;
			system("pause");
		}

		if (select == 0)
			break;
	}
}



