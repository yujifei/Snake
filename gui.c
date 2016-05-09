#include<gtk/gtk.h>
#include "gui.h"
#include "game_logical.h"
#include "timer.h"
#include "callback.h"

void global_init()
{
	snake_init();
	init_time = 1.0;
	init_sigaction(SIG_IGN);
	init_timer(init_time);
	cur_snake_orient.x = -1;
	cur_snake_orient.y = 0;
	orient_lock = true;
	
	bg_color[0] = 0.0, bg_color[1] = 0.0, bg_color[2] = 0.0;
	fg_color[0] = 1.0, fg_color[1] = 1.0, fg_color[2] = 1.0;
	snake_color[0] = 1.0, snake_color[1] = 0.0, snake_color[2] = 0.0;
	food_color[0] = 0.0, food_color[1] = 1.0, food_color[2] = 0.0;
}

GtkWidget * create_menu_file()
{
	GtkWidget * menu;
	GtkWidget * menu_item;
	GtkWidget * menu_item_head;
	
	menu = gtk_menu_new();
	menu_item_head = gtk_menu_item_new_with_mnemonic("文件(_F)");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_item_head), menu);
	
	menu_item = gtk_menu_item_new_with_mnemonic("开始(_S)");
	gtk_signal_connect(GTK_OBJECT(menu_item), "activate", G_CALLBACK(on_menu_file_start_clicked), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);
	
	menu_item = gtk_menu_item_new_with_mnemonic("暂停(_P)");
	gtk_signal_connect(GTK_OBJECT(menu_item), "activate", G_CALLBACK(on_menu_file_pause_clicked), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);
	
	menu_item = gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);
	
	menu_item = gtk_menu_item_new_with_mnemonic("退出(_Q)");
	gtk_signal_connect(GTK_OBJECT(menu_item), "activate", G_CALLBACK(on_program_exit), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);
	
	return menu_item_head;
}

GtkWidget * create_menu_setting()
{
	GtkWidget * menu;
	GtkWidget * menu_item;
	GtkWidget * menu_item_head;
	
	menu = gtk_menu_new();
	menu_item_head = gtk_menu_item_new_with_mnemonic("设置(_S)");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_item_head), menu);
	
	menu_item = gtk_menu_item_new_with_mnemonic("首选项");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);
	
	return menu_item_head;
}

GtkWidget * create_menu_help()
{
	GtkWidget * menu;
	GtkWidget * menu_item;
	GtkWidget * menu_item_head;
	
	menu = gtk_menu_new();
	menu_item_head = gtk_menu_item_new_with_mnemonic("帮助(_H)");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_item_head), menu);
	
	menu_item = gtk_menu_item_new_with_mnemonic("游戏简介(_I)");
	gtk_signal_connect(GTK_OBJECT(menu_item), "activate", G_CALLBACK(on_menu_help_intro_clicked), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);
	
	menu_item = gtk_menu_item_new_with_mnemonic("关于(_A)");
	gtk_signal_connect(GTK_OBJECT(menu_item), "activate", G_CALLBACK(on_menu_help_about_clicked), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);
	
	return menu_item_head;
}

GtkWidget * create_menu()
{
	GtkWidget * menubar;
	GtkWidget * menu;
	
	menubar = gtk_menu_bar_new();
	
	menu = create_menu_file();
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menu);
	
	menu = create_menu_setting();
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menu);
	
	menu = create_menu_help();
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menu);
	
	return menubar;
}


void main_window(int argc, char * argv[])
{
	GtkWidget * window;
	GtkWidget * vbox;
	GtkWidget * draw_area;
	GtkWidget * menu = NULL;
	
	gtk_init(&argc, &argv);
	
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(window), WIN_WIDTH, WIN_HEIGHT);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_title(GTK_WINDOW(window), "贪吃蛇");
	gtk_window_set_icon_from_file(GTK_WINDOW(window), "./icons/icon.png", NULL);
	
	vbox = gtk_vbox_new(FALSE, 0);
	draw_area = gtk_drawing_area_new();
	DrawArea = draw_area;
	
	menu = create_menu();
	gtk_container_add(GTK_CONTAINER(window), vbox);
	gtk_box_pack_start(GTK_BOX(vbox), menu, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), draw_area, TRUE, TRUE, 0);
	
	gtk_signal_connect(GTK_OBJECT(window), "destroy", G_CALLBACK(on_program_exit), NULL);
	gtk_signal_connect(GTK_OBJECT(draw_area), "expose-event", G_CALLBACK(on_expose), (gpointer)snake_zone_top_left);
	gtk_signal_connect(GTK_OBJECT(draw_area), "configure-event", G_CALLBACK(on_configure), NULL);
	gtk_signal_connect(GTK_OBJECT(window), "key-press-event", G_CALLBACK(on_key_pressed), NULL);
	
	gtk_widget_show_all(window);
	gtk_main();
}
