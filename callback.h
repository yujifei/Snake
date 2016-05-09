#ifndef FILE_CALLBACK_H
#define FILE_CALLBACK_H

#include<gtk/gtk.h>
#include<cairo.h>

float snake_zone_top_left[2];

void do_you_dead(); //任务失败，显示一个"你挂了"的对话框

void draw_frame(GtkWidget * draw_area, cairo_t * cr); // 绘制背景以及蛇的活动区
void draw_food(GtkWidget * draw_area, cairo_t * cr); //绘制食物
void draw_snake(GtkWidget * draw_area, cairo_t * cr); //绘制蛇

gboolean on_expose(GtkWidget * widget, GdkEventExpose * event, gpointer data);
gboolean on_configure(GtkWidget * widget, GdkEventConfigure * event, gpointer data);
gboolean on_program_exit(GtkWidget * widget, gpointer data);

gboolean on_key_pressed(GtkWidget * widget, GdkEventKey * event, gpointer data);

gboolean on_menu_file_start_clicked(GtkWidget * item, gpointer data);
gboolean on_menu_file_pause_clicked(GtkWidget * item, gpointer data);
gboolean on_menu_help_intro_clicked(GtkWidget * item, gpointer data);
gboolean on_menu_help_about_clicked(GtkWidget * item, gpointer data);

#endif
