#ifndef FILE_GUI_H
#define FILE_GUI_H

#include<gtk/gtk.h>

GtkWidget * DrawArea;

#define WIN_WIDTH  700
#define WIN_HEIGHT 500

float bg_color[3];
float fg_color[3];

float snake_color[3];
float food_color[3];

void global_init();

GtkWidget * create_menu_file(); //创建"文件"菜单
GtkWidget * create_menu_setting(); //创建"设置"菜单
GtkWidget * create_menu_help(); //创建"帮助"菜单
GtkWidget * create_menu();

void main_window(int argc, char * argv[]); //创建整个窗口

#endif
