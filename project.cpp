#include <stdio.h>
#include <stdlib.h>
#include<conio.h>
#include<time.h>

int map_size = 1;
int mouse_x = 0, mouse_y = 0; // 老鼠坐标
int granary_x = 0, granary_y = 0;		//粮仓坐标

int maze[9][9];           // 地图最大尺寸

struct best               // 用于记录最短路径的结构体
{
    int min;
    int b[9][9];
} best_path;

int route_number;         // 正确路线个数

// 开始游戏
void game()
{
    mouse_x = granary_x - 1;
    mouse_y = granary_y - 1;
    int i, j, valid_move = 0, elapsed_time = 0;          // elapsed_time为已用时间
    char input_char;
    time_t start_time, end_time;                // 时间类型
    start_time = time(NULL);

    print_map(map_size);
    printf("（按w↑s↓a← d→移动）\n----请在15秒内通关----\n");

    while (1)
    {
        printf("时间：%d\r", elapsed_time);

        if (_kbhit()) // 输入控制
        {
            input_char = _getch();
            if (input_char == 'w' && maze[mouse_x - 1][mouse_y] != 1) // 上
            {
                if (maze[mouse_x - 1][mouse_y] == 0)
                {
                    maze[mouse_x - 1][mouse_y] = 1;
                    maze[mouse_x][mouse_y] = 0;
                    mouse_x -= 1;
                }
                else if (maze[mouse_x - 1][mouse_y] == 3)
                {
                    maze[mouse_x][mouse_y] = 0;
                    valid_move = 1;
                }
            }
            else if (input_char == 's' && maze[mouse_x + 1][mouse_y] != 1) // 下
            {
                if (maze[mouse_x + 1][mouse_y] == 0)
                {
                    maze[mouse_x + 1][mouse_y] = 1;
                    maze[mouse_x][mouse_y] = 0;
                    mouse_x += 1;
                }
                else if (maze[mouse_x + 1][mouse_y] == 3)
                {
                    maze[mouse_x][mouse_y] = 0;
                    valid_move = 1;
                }
            }
            else if (input_char == 'a' && maze[mouse_x][mouse_y - 1] != 1) // 左
            {
                if (maze[mouse_x][mouse_y - 1] == 0)
                {
                    maze[mouse_x][mouse_y - 1] = 1;
                    maze[mouse_x][mouse_y] = 0;
                    mouse_y -= 1;
                }
                else if (maze[mouse_x][mouse_y - 1] == 3)
                {
                    maze[mouse_x][mouse_y] = 0;
                    valid_move = 1;
                }
            }
            else if (input_char == 'd' && maze[mouse_x][mouse_y + 1] != 1) // 右
            {
                if (maze[mouse_x][mouse_y + 1] == 0)
                {
                    maze[mouse_x][mouse_y + 1] = 1;
                    maze[mouse_x][mouse_y] = 0;
                    mouse_y += 1;
                }
                else if (maze[mouse_x][mouse_y + 1] == 3)
                {
                    maze[mouse_x][mouse_y] = 0;
                    valid_move = 1;
                }
            }

            system("cls");
            printf("显示迷宫：\n");  // 显示游戏地图
            print_map(map_size);
            printf("（按w↑s↓a← d→移动）\n请在15秒内通关☆(－ｏ⌒) \n");
        }

        if (valid_move == 1) // 判断是否通关
        {
            printf("\n----恭喜通关----\n");
            system("pause");
            break;
        }

        if (elapsed_time > 15) // 规定时间到后结束游戏
        {
            printf("\n----未在规定时间内通关，游戏失败----\n");
            maze[mouse_x][mouse_y] = 0;   // 清除最后所在位置
            system("pause");
            break;
        }

        end_time = time(NULL);
        elapsed_time = difftime(end_time, start_time);
    }

}

void main()
{
    int select, k;

    while (1)
    {
        system("cls");
        printf("  ================== 老鼠走迷宫游戏 ==================\n");
        printf(" | 请选择：                                          |\n");
        printf(" |              1. 获取地图；                        |\n");
        printf(" |              2. 新建地图；                        |\n");
        printf(" |              3. 查看地图；                        |\n");
        printf(" |              4. 修改地图；                        |\n");
        printf(" |              5. 显示通关路径数量；                |\n");
        printf(" |              6. 显示最优路径；                    |\n");
        printf(" |              7. 显示所有路径；                    |\n");
        printf(" |              8. 开始游戏；                        |\n");
        printf(" |              0. 退出系统；                        |\n");
        printf("  =========== （请输入相应数字执行其功能）===========\n");

        for (k = 0; k < 30; k++) fflush(stdin);  // 清除键盘输入的scanf缓存，防止死循环

        scanf_s("%d", &select);
        if (select >= 0 && select <= 8)    // 键盘输入检错
        {
            switch (select)
            {
            case 0:
                exit(0);
            case 1:
                system("cls");
                get_map(9); // 从文件获取地图到程序
                break;
            case 2:
                system("cls");
                printf("新的地图大小(小于9)：");
                scanf_s("%d", &map_size);
                create_plat(map_size);  // 新建地图
                break;
            case 3:
                system("cls");
                print_map(map_size);    // 显示地图
                system("pause");
                break;
            case 4:
                alter_map(); // 修改地图
                break;
            case 5:
                system("cls");
                path_result(mouse_x - 1, mouse_y - 1);
                printf("有%d条通路\n", route_number);
                route_number = 0;
                system("pause");
                break;
            case 6:
                system("cls");
                best_path.min = 999;
                path_result(mouse_x - 1, mouse_y - 1); // 统计地图所有解的个数
                path_best(mouse_x - 1, mouse_y - 1, 1); // 寻找最短的路径
                system("pause");
                break;
            case 7:
                system("cls");
                path_find(mouse_x - 1, mouse_y - 1);  // 寻找所有通关路径
                printf("----已显示所有路径!----\n");
                system("pause");
                break;
            case 8:
                system("cls");
                game(); // 开始游戏
                break;
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
}


