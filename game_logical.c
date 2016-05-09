#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "game_logical.h"

void snake_init()
{
	snake_destroy();

	snake_body = NULL;
	struct snake_node * ptr;
	int i;
	for(i = 0; i < SNAKE_INIT_LEN; ++i)
	{
		ptr = (struct snake_node *)malloc(sizeof(struct snake_node));
		if(NULL == ptr)
		{
			perror("Fatal error in snake_init");
			snake_destroy(); //销毁已分配的节点
			exit(1);
		}
		
		ptr->pos_x = snake_init_pos[0][i];
		ptr->pos_y = snake_init_pos[1][i];
		ptr->next_node = snake_body;
		
		snake_body = ptr;
	}
	
	generate_food_pos();
	snake_cur_len = 4;
}

/* 判断一个给定的点是否位于合适的位置，若是返回1否则返回0*/
int point_in_valid_pos(position_t pos)
{
	//越界
	if(pos.x < 0 || pos.x >= ZONE_WIDTH)
	{
		return 0;
	}
	if(pos.y < 0 || pos.y >= ZONE_HEIGHT)
	{
		return 0;
	}
	
	// 在蛇身上
	struct snake_node * ptr = snake_body;
	while(ptr != NULL)
	{
		if(ptr->pos_x == pos.x && ptr->pos_y == pos.y)
		{
			return 0;
		}
		ptr = ptr->next_node;
	}
	
	//其他
	// some code
	
	return 1;
}

void generate_food_pos()
{
	srand(time(0));
	do
	{
		food_position.x = rand() % ZONE_WIDTH;
		food_position.y = rand() % ZONE_HEIGHT;
	}
	while(!point_in_valid_pos(food_position));
}

/* 蛇向前走一步，第一个节点根据前进方向修改，其余节点
 * 跟新为前一个节点的值******************************
 * **************************************************/

void snake_go_ahead()
{
	if(NULL == snake_body)
	{
		snake_init();
	}
	
	struct snake_node tmp_node;
	struct snake_node * tmp_ptr;
	
	tmp_node.pos_x = snake_body->pos_x;
	tmp_node.pos_y = snake_body->pos_y;
	tmp_ptr = snake_body->next_node;
	
	snake_body->pos_x += cur_snake_orient.x;
	snake_body->pos_y += cur_snake_orient.y;
	
	
	while(tmp_ptr != NULL)
	{
		//交换tmp_node与ptr所指节点的值
		tmp_ptr->pos_x += tmp_node.pos_x;
		tmp_node.pos_x = tmp_ptr->pos_x - tmp_node.pos_x;
		tmp_ptr->pos_x -= tmp_node.pos_x;
		
		tmp_ptr->pos_y += tmp_node.pos_y;
		tmp_node.pos_y = tmp_ptr->pos_y - tmp_node.pos_y;
		tmp_ptr->pos_y -= tmp_node.pos_y;
		
		tmp_ptr = tmp_ptr->next_node;		
	}
}

void snake_grow_up()
{
	struct snake_node * new_node;
	new_node = (struct snake_node *)malloc(sizeof(struct snake_node));
	if(NULL == new_node)
	{
		perror("Fatal error in snake_grow_up");
		snake_destroy();
		exit(1);
	}
	
	if(NULL == snake_body)
	{
		snake_init();
	}
	
	new_node->pos_x = snake_body->pos_x + cur_snake_orient.x;
	new_node->pos_y = snake_body->pos_y + cur_snake_orient.y;
	new_node->next_node = snake_body;
	
	snake_body = new_node;
	++snake_cur_len;
}

//根据前进方向下一个节点来判断蛇前进还是长大或者任务失败
//任务失败返回-1否则返回1

int snake_update()
{
	position_t next_pos;
	next_pos.x = snake_body->pos_x + cur_snake_orient.x;
	next_pos.y = snake_body->pos_y + cur_snake_orient.y;
	if(!point_in_valid_pos(next_pos)) // 蛇越界或者撞到自己 任务失败
	{
		return -1;
	}
	
	else if(next_pos.x == food_position.x && next_pos.y == food_position.y)
	{
		snake_grow_up();
		generate_food_pos();
		return 0;
	}
	
	else
	{
		snake_go_ahead();
		return 0;
	}
	 
}

void snake_destroy()
{
	struct snake_node * ptr;
	
	while(snake_body != NULL)
	{
		ptr = snake_body;
		snake_body = snake_body->next_node;
		free(ptr);
	}
}

void update_orient(struct orientation new_orient)
{
	/* 蛇只能向左或者右转，不能向后转，因此新的方向与当前
	 * 前进方向正交才予以跟新，否则忽略新的方向**********/
	 
	if(cur_snake_orient.x * new_orient.x +
	    cur_snake_orient.y * new_orient.y == 0)
	{
		cur_snake_orient.x = new_orient.x;
		cur_snake_orient.y = new_orient.y; 	
	}
}


