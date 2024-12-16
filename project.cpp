#define _CRT_SECURE_NO_WARNINGS
#define TIME 300
#define MAX_SIZE 9

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>
using namespace std;

int map_size = 1;
int mouse_x = 0, mouse_y = 0;                 // 老鼠移动中的坐标
int mouse_initial_x = 0, mouse_initial_y = 0; // 老鼠初始位置(逻辑)
int granary_x = 0, granary_y = 0;             // 粮仓初始位置

int map[MAX_SIZE][MAX_SIZE]; // 地图最大尺寸

int path_number; // 正确路线个数

pair<int, int> temp_pair;
vector<pair<int, int>> best_path;
vector<pair<int, int>> path;
int shortest_distance = 1e9; // vector容器用于求最短路径的函数

// 打印地图
void print_map(int a, int x = mouse_x, int y = mouse_y) // int mouse_x,int mouse_y用于防止显示bug
{
    for (int i = 0; i < map_size; i++)
    {
        for (int j = 0; j < map_size; j++)
        {
            if (map[i][j] == 2)
            {
                printf("墙");
            }

            else if (map[i][j] == 1 && i == x && j == y)
            {
                printf("鼠");
            }
            else if (map[i][j] == 3)
            {
                printf("仓");
            }
            else
            {
                printf("  ");
            }
        }
        printf("\n");
    }
}

// 新建地图，并存入txt文件
void create_map(int a)
{
    map_size = a;

    int i, j, s;
error_1:
    printf("请输入0和2,0代表通路，2代表墙体(数字用空格隔开)，输入规格是%dx%d\n", map_size, map_size);
    for (i = 1; i <= map_size; i++)
    {
        printf("第%d行:", i);
        for (j = 1; j <= map_size; j++)
        {
            scanf_s("%d", &s);
            if (s == 0 || s == 2)
                map[i - 1][j - 1] = s;
            else
            {
                system("cls");
                printf("----输入错误请重新输入----\n");
                goto error_1;
            }
        }
    }

error_2:
    printf("请设置老鼠的初始位置(x, y)（1-%d,1-%d）:\n", map_size, map_size);

    for (i = 0; i < 30; i++) // 清除缓存字符
        fflush(stdin);

    scanf_s("%d,%d", &mouse_initial_x, &mouse_initial_y);
    if (mouse_initial_x <= map_size && mouse_initial_y <= map_size && mouse_initial_x > 0 && mouse_initial_y > 0)
        map[mouse_initial_x - 1][mouse_initial_y - 1] = 1;
    else
    {
        system("cls");
        printf("----输入错误，请重新输入,在%dx%d的范围内----\n", map_size, map_size);
        goto error_2;
    }

error_3:
    printf("请设置粮仓的位置(x, y):\n");
    for (i = 0; i < 30; i++) // 清除缓存字符
        fflush(stdin);

    scanf_s("%d,%d", &granary_x, &granary_y);
    if (granary_x <= map_size && granary_y <= map_size && granary_x > 0 && granary_y > 0 && (granary_x != mouse_initial_x || granary_y != mouse_initial_y))
        map[granary_x - 1][granary_y - 1] = 3;
    else
    {
        system("cls");
        printf("----输入错误，请重新输入,在%dx%d的范围内----\n", map_size, map_size);
        goto error_3;
    }

    // 文件保存地图
    FILE* fp;
    fp = fopen("map.txt", "w");
    for (i = 0; i < map_size; i++)
    {
        for (j = 0; j < map_size; j++)
        {
            fprintf(fp, "%d\t", map[i][j]);
        }
        fprintf(fp, "%c", '\n');
    }
    fprintf(fp, "%d\t", mouse_initial_x);
    fprintf(fp, "%d\t", mouse_initial_y);
    fprintf(fp, "%d\t", granary_x);
    fprintf(fp, "%d\t", granary_y);
    fprintf(fp, "%d\t", map_size);
    fclose(fp);

    printf("----地图新建完成,并保存在同目录下的txt文件中----\n");
    system("pause");
}

// 从txt获取地图，读入程序中
void get_map(int a)
{
    map_size = a;
    int i, j;
    FILE* fp;
    fp = fopen("map.txt", "r");
    if (!fp)
    {
        printf("文件不存在，请重新打开\n");
        system("pause");
    }
    else
    {
        for (i = 0; i < map_size; i++)
        {
            for (j = 0; j < map_size; j++)
            {
                fscanf(fp, "%d\t", &map[i][j]);
            }
            fscanf(fp, "\n");
        }
        fscanf(fp, "%d\t", &mouse_initial_x);
        fscanf(fp, "%d\t", &mouse_initial_y); // 读取老鼠的位置
        fscanf(fp, "%d\t", &granary_x);
        fscanf(fp, "%d\t", &granary_y); // 读取粮仓的位置
        fscanf(fp, "%d\t", &map_size);  // 读取地图大小
        printf("读取成功，请查看新地图！\n");
        print_map(map_size);
        system("pause");
    }
    fclose(fp);
}

// 修改地图函数
void alter_map()
{
    int i, j, select, a, b;
    FILE* fp;
    while (1)
    {
        system("cls");

        print_map(map_size); // 显示地图

        printf("  =============----修改地图------===================\n");
        printf(" |请选择：                                          |\n");
        printf(" |              1.修改为墙体；                      |\n");
        printf(" |              2.修改为通路；                      |\n");
        printf(" |              3.保存修改地图；                    |\n");
        printf(" |              0.退出修改功能；                    |\n");
        printf("  ===========（请输入相应数字执行其功能）===========\n");
        fflush(stdin); // 清除键入的缓存
        scanf("%d", &select);
        for (i = 0; i < 30; i++)
            fflush(stdin);
        if (select < 4 && select > 0)
        {
            switch (select)
            {
            case 1:
                printf("围墙内为修改范围，范围是%dx%d\n", map_size, map_size);
                printf("请输入坐标 x, y 修改地图:\n");

                // 清除缓存字符
                for (i = 0; i < 30; i++)
                    fflush(stdin);

                cin >> a >> b;
                if (a < map_size && b < map_size && a > 1 && b > 1 && map[a - 1][b - 1] != 1 && map[a - 1][b - 1] != 3)
                    map[a - 1][b - 1] = 2;
                else
                {
                    printf("输入错误，请重新输入，不能在围墙、粮仓和老鼠的位置修改哦\n");
                    system("pause");
                }
                break;
            case 2:
                printf("围墙内为修改范围，范围是%dx%d\n", map_size, map_size);
                printf("请输入坐标 x, y 修改地图:\n");
                // 清除缓存字符
                for (i = 0; i < 30; i++)
                    fflush(stdin);

                cin >> a >> b;
                if (a < map_size && b < map_size && a > 1 && b > 1 && map[a - 1][b - 1] != 1 && map[a - 1][b - 1] != 3)
                    map[a - 1][b - 1] = 0;
                else
                {
                    printf("输入错误，请重新输入，不能在围墙、粮仓和老鼠的位置修改哦\n");
                    system("pause");
                }
                break;
            case 3: // 文件形式保存修改后地图
                fp = fopen("map.txt", "w");
                for (i = 0; i < map_size; i++)
                {
                    for (j = 0; j < map_size; j++)
                        fprintf(fp, "%d\t", map[i][j]);
                    fprintf(fp, "%c", '\n');
                }
                fprintf(fp, "%d\t", mouse_initial_x);
                fprintf(fp, "%d\t", mouse_initial_y);
                fprintf(fp, "%d\t", granary_x);
                fprintf(fp, "%d\t", granary_y);
                fclose(fp);

                printf("----地图修改完成,并保存在同目录下的txt文件中----\n");
                system("pause");
                break;
            case 0:
                break;
            }
        }
        else
        {
            printf("----请按规定输入----\n");
            system("pause");
        }

        if (select == 0)
            break;
    }
}

// 开始游戏
void game()
{
    mouse_x = mouse_initial_x - 1;
    mouse_y = mouse_initial_y - 1;
    int i, j, v = 0, t = 0; // t为初始时间
    char str;
    time_t start, end; // 定义时间类型变量
    start = time(NULL);
    print_map(map_size, mouse_x, mouse_y);
    printf("（按w a s d移动）\n----请在30秒内通关----\n"); // 初始化地图

    while (1)
    {
        printf("时间：%d\r", t);
        if (_kbhit()) // 输入控制,检查键盘有无输入，有非0，无1
        {
            str = _getch();
            if (str == 'w') // 上
            {
                if (map[mouse_x - 1][mouse_y] == 0)
                {
                    map[mouse_x - 1][mouse_y] = 1;
                    map[mouse_x][mouse_y] = 0;
                    mouse_x = mouse_x - 1;
                }
                else if (map[mouse_x - 1][mouse_y] == 3)
                {
                    map[mouse_x][mouse_y] = 0;
                    v = 1;
                }
            }
            else if (str == 's') // 下
            {
                if (map[mouse_x + 1][mouse_y] == 0)
                {
                    map[mouse_x + 1][mouse_y] = 1;
                    map[mouse_x][mouse_y] = 0;
                    mouse_x = mouse_x + 1;
                }
                else if (map[mouse_x + 1][mouse_y] == 3)
                {
                    map[mouse_x][mouse_y] = 0;
                    v = 1;
                }
            }
            else if (str == 'a') // 左
            {
                if (map[mouse_x][mouse_y - 1] == 0)
                {
                    map[mouse_x][mouse_y - 1] = 1;
                    map[mouse_x][mouse_y] = 0;
                    mouse_y = mouse_y - 1;
                }
                else if (map[mouse_x][mouse_y - 1] == 3)
                {
                    map[mouse_x][mouse_y] = 0;
                    v = 1;
                }
            }
            else if (str == 'd') // 右
            {
                if (map[mouse_x][mouse_y + 1] == 0)
                {
                    map[mouse_x][mouse_y + 1] = 1;
                    map[mouse_x][mouse_y] = 0;
                    mouse_y = mouse_y + 1;
                }
                else if (map[mouse_x][mouse_y + 1] == 3)
                {
                    map[mouse_x][mouse_y] = 0;
                    v = 1;
                }
            }
            else
                ;

            system("cls");
            print_map(map_size);
            printf("（按w a s d移动）\n请在30秒内通关 \n");
        }
        else
            ;

        if (v == 1) // 判断是否通关
        {
            printf("\n----恭喜通关----\n");
            system("pause");
            break;
        }

        if (t > 29) // 规定时间到后结束游戏
        {
            printf("\n----未在规定时间内通关，游戏失败----\n");
            map[mouse_x][mouse_y] = 0; // 清除最后所在位置
            system("pause");
            break;
        }

        end = time(NULL);
        t = difftime(end, start);
    }
}

// 计算通路数函数
void path_count_number(int m, int n, int distance)
{

    map[m][n] = 1;
    temp_pair = pair<int, int>(m, n);
    path.push_back(temp_pair);
    if (map[m][n + 1] == 3 || map[m][n - 1] == 3 || map[m + 1][n] == 3 || map[m - 1][n] == 3) // 判断当前位置上下左右是否有粮仓
    {
        ++path_number; // 统计一个地图的所有可能性的次数
        if (distance < shortest_distance)
        {
            shortest_distance = distance;
            best_path = path;
        }
    }
    // 递归
    if (map[m][n + 1] == 0)
        path_count_number(m, n + 1, distance + 1); // 右
    if (map[m + 1][n] == 0)
        path_count_number(m + 1, n, distance + 1); // 下
    if (map[m][n - 1] == 0)
        path_count_number(m, n - 1, distance + 1); // 左
    if (map[m - 1][n] == 0)
        path_count_number(m - 1, n, distance + 1); // 上

    map[m][n] = 0; // 回溯
    path.pop_back();
}

// 寻找通关路径函数
void show_all_path(int m, int n)
{
    map[m][n] = 1;
    temp_pair = pair<int, int>(m, n);
    path.push_back(temp_pair);

    if (map[m][n + 1] == 3 || map[m][n - 1] == 3 || map[m + 1][n] == 3 || map[m - 1][n] == 3) // 判断当前位置上下左右是否有粮仓
    {
        for (int i = 0; i < path.size(); i++)
        {
            map[path[i].first][path[i].second] = 1;
            print_map(map_size, path[i].first, path[i].second);
            std::this_thread::sleep_for(std::chrono::milliseconds(TIME)); // 控制每个字符的输出间隔
            map[path[i].first][path[i].second] = 0;
            system("cls");
        }
    }
    // 递归
    if (map[m][n + 1] == 0)
        show_all_path(m, n + 1); // 右
    if (map[m + 1][n] == 0)
        show_all_path(m + 1, n); // 下
    if (map[m][n - 1] == 0)
        show_all_path(m, n - 1); // 左
    if (map[m - 1][n] == 0)
        show_all_path(m - 1, n); // 上

    map[m][n] = 0; // 回溯
    path.pop_back();
}

// 显示最优路径函数
void path_best(int p, int q, int l)
{
    path_count_number(p, q, 0);
    while (!best_path.empty())
    {
        map[best_path[0].first][best_path[0].second] = 1;
        print_map(map_size, best_path[0].first, best_path[0].second);
        std::this_thread::sleep_for(std::chrono::milliseconds(TIME)); // 控制每个字符的输出间隔
        map[best_path[0].first][best_path[0].second] = 0;
        best_path.erase(best_path.begin());
        system("cls");
    }
}

int main()
{
    int select, k;

    while (1)
    {
        system("cls");
        printf("  =============老鼠走迷宫游戏 n.0===================\n");
        printf(" |请选择：                                          |\n");
        printf(" |              1.获取地图；                        |\n");
        printf(" |              2.新建地图；                        |\n");
        printf(" |              3.查看地图；                        |\n");
        printf(" |              4.修改地图；                        |\n");
        printf(" |              5.显示有几条通关路径；              |\n");
        printf(" |              6.显示最优路径；                    |\n");
        printf(" |              7.显示通关所有路径；                |\n");
        printf(" |              8.开始游戏；                        |\n");
        printf(" |              0.退出系统；                        |\n");
        printf("  ===========（请输入相应数字执行其功能）===========\n");
        for (k = 0; k < 30; k++)
            fflush(stdin); // 清除缓存

        scanf_s("%d", &select);
        if (select >= 0 && select <= 8) // 键盘输入检错
        {
            switch (select)
            {
            case 0:
                exit(0);
            case 1:
                system("cls"); // 清除菜单
                get_map(9);    // 从文件获取地图到程序
                break;
            case 2:
                system("cls");
                printf("新的地图大小(小于9)：");
                scanf_s("%d", &map_size);
                create_map(map_size); // 新建地图
                break;
            case 3:
                system("cls");
                print_map(map_size, mouse_initial_x - 1, mouse_initial_y - 1); // 显示地图
                system("pause");
                break;
            case 4:
                alter_map(); // 修改地图
                break;
            case 5:
                system("cls");
                path_count_number(mouse_initial_x - 1, mouse_initial_y - 1, 0);
                printf("有%d条通路\n", path_number);
                path_number = 0;
                system("pause");
                break;
            case 6:
                system("cls");
                path_best(mouse_initial_x - 1, mouse_initial_y - 1, 1); // 寻找最短的路径
                system("pause");
                path_number = 0;
                break;
            case 7:
                system("cls");
                show_all_path(mouse_initial_x - 1, mouse_initial_y - 1); // 寻找所有通关路径
                printf("----已显示所有路径!----\n");
                system("pause");
                break;
            case 8:
                system("cls");
                game();
            default:
                break;
            }
        }
        else
        {
            system("cls");
            printf("----请按规定输入----\n");
            system("pause");
        }
    }
    return 0;
}
