#ifndef FILE_GAME_LOGICAL_H
#define FILE_GAME_LOGICAL_H

#include <stdbool.h>

struct snake_node
{
	int pos_x;  //栅格坐标，从0开始
	int pos_y;
	struct snake_node * next_node;
};

struct snake_node * snake_body; //蛇的身体

struct orientation
{
	int x;  // x取 -1， 0， 1之一
	int y;  // y取 -1， 0， 1之一且x * y = 0
};

typedef struct orientation position_t; //若oritation::x, y 取值不作限制则可表示一个位置

struct orientation cur_snake_orient; //当前蛇前进方向
_Bool orient_lock;
position_t food_position; // 食物位置

void snake_init(); // 初始化蛇
void generate_food_pos(); // 生成食物位置  
void snake_go_ahead(); // 向前走一步
void snake_grow_up();  // 增加一个节点
int snake_update();   // 更新蛇
void snake_destroy();  //删除所有节点
void update_orient(struct orientation new_orient);
int point_in_valid_pos(position_t p); //判断一个点是否位合适区域，不合适区域指诸如活动区域外，特殊位置等等

#define SNAKE_INIT_LEN 4
static int snake_init_pos[][SNAKE_INIT_LEN] =
{{42, 41, 40, 39}, {20, 20, 20, 20}};

int snake_cur_len; 



#define ZONE_WIDTH  60   //蛇活动区域宽度(栅格数)
#define ZONE_HEIGHT 40   //蛇活动区域高度(栅格数)
#define ZONE_GRID_SIZE 10  //栅格大小(像素数), 这个数本来应该定义在gui.h中的，考虑到它与上面两个数关系密切所以就定义到这了


#endif
