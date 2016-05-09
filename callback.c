#include<gtk/gtk.h>
#include<gdk/gdkkeysyms.h>
#include<cairo.h>
#include<signal.h>
#include "gui.h"
#include "callback.h"
#include "game_logical.h"
#include "timer.h"

void do_you_dead()
{
	const gchar * msg = "你挂了！";
	GtkWidget * dialog;
	dialog = gtk_message_dialog_new(NULL,
			 GTK_DIALOG_DESTROY_WITH_PARENT,
			 GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
			 msg, "title");

	gtk_window_set_icon_from_file(GTK_WINDOW(dialog), "./icons/icon.png", NULL);
	gtk_window_set_title(GTK_WINDOW(dialog), "贪吃蛇");
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

void draw_frame(GtkWidget * draw_area, cairo_t * cr)
{
	float width = draw_area->allocation.width;
	float height = draw_area->allocation.height;
	
	//绘制背景
	cairo_set_source_rgb(cr, bg_color[0], bg_color[1], bg_color[2]);
	cairo_rectangle(cr, 1, 1, width, height);
	cairo_stroke_preserve(cr);
	cairo_fill(cr);
	
	//绘制活动区域
	float snake_zone_width = ZONE_WIDTH * ZONE_GRID_SIZE;
	float snake_zone_height = ZONE_HEIGHT * ZONE_GRID_SIZE;
	
	cairo_set_source_rgb(cr, fg_color[0], fg_color[1], fg_color[2]);
	cairo_rectangle(cr, snake_zone_top_left[0], snake_zone_top_left[1], snake_zone_width, snake_zone_height);
	cairo_fill(cr);
}

void draw_food(GtkWidget * draw_area, cairo_t * cr)
{
	float food_left = snake_zone_top_left[0] + food_position.x * ZONE_GRID_SIZE;
	float food_top  = snake_zone_top_left[1] + food_position.y * ZONE_GRID_SIZE;
	
	cairo_set_source_rgb(cr, food_color[0], food_color[1], food_color[2]);
	cairo_rectangle(cr, food_left, food_top, ZONE_GRID_SIZE, ZONE_GRID_SIZE);
	cairo_fill(cr);
}

void draw_snake(GtkWidget * draw_area, cairo_t * cr)
{
	float left, top;
	struct snake_node * ptr = snake_body;
	
	cairo_set_source_rgb(cr, snake_color[0], snake_color[1], snake_color[2]);
	
	while(ptr != NULL)
	{
		left = snake_zone_top_left[0] + ptr->pos_x * ZONE_GRID_SIZE;
		top  = snake_zone_top_left[1] + ptr->pos_y * ZONE_GRID_SIZE;
		
		cairo_rectangle(cr, left, top, ZONE_GRID_SIZE, ZONE_GRID_SIZE);
		
		ptr = ptr->next_node;
	}
	
	cairo_fill(cr);
}

gboolean on_expose(GtkWidget * widget, GdkEventExpose * event, gpointer data)
{
	float width = widget->allocation.width;
	float height = widget->allocation.height;
	
	float zone_width = ZONE_WIDTH * ZONE_GRID_SIZE;
	float zone_height = ZONE_HEIGHT * ZONE_GRID_SIZE;
	
	float * _data = (float *)data;
	_data[0] = (float)(int)((width - zone_width) / 2);
	_data[1] = (float)(int)((height - zone_height) / 2);
	
	cairo_t * cr;
	cr = gdk_cairo_create(widget->window);
	
	draw_frame(widget, cr);
	draw_food(widget, cr);
	draw_snake(widget, cr);
	
	cairo_destroy(cr);
	
	return TRUE;
}

gboolean on_configure(GtkWidget * widget, GdkEventConfigure * event, gpointer data)
{
	gtk_widget_queue_draw(widget);
	return TRUE;
}

gboolean on_program_exit(GtkWidget * widget, gpointer data)
{
	snake_destroy();
	gtk_main_quit();
	
	return TRUE;
}

gboolean on_key_pressed(GtkWidget * widget, GdkEventKey * event, gpointer data)
{
#ifdef DEBUG
	g_print("key pressed!\n");
#endif
	struct orientation new_orient = cur_snake_orient;
	switch(event->keyval)
	{
		case GDK_KEY_A:
		case GDK_KEY_a:
		case GDK_KEY_Left:
		
			new_orient.x = -1;
			new_orient.y = 0;
			break;
		
		case GDK_KEY_D:
		case GDK_KEY_d:
		case GDK_KEY_Right:
			
			new_orient.x = 1;
			new_orient.y = 0;
			break;
			
		case GDK_KEY_W:
		case GDK_KEY_w:
		case GDK_KEY_Up:
		
			new_orient.x = 0;
			new_orient.y = -1;
			break;
			
		case GDK_KEY_S:
		case GDK_KEY_s:
		case GDK_KEY_Down:
		
			new_orient.x = 0;
			new_orient.y = 1;
			break;

		case GDK_KEY_P:
		case GDK_KEY_p:
			on_menu_file_pause_clicked(NULL, NULL);
			break;

		case GDK_KEY_space:
			on_menu_file_start_clicked(NULL, NULL);
			break; 
			
		default:
#ifdef DEBUG
			g_print("0x%x\n", event->keyval);
#endif
			break;
	}
	if(!orient_lock)
	{
		update_orient(new_orient);
		orient_lock = true;
	}
	
	return TRUE;
}

gboolean on_menu_file_start_clicked(GtkWidget * item, gpointer data)
{
	g_print("start\n");
	init_sigaction(timer_handler);
	init_timer(init_time);
	return TRUE;
}

gboolean on_menu_file_pause_clicked(GtkWidget * item, gpointer data)
{
	init_sigaction(SIG_IGN);
	init_timer(0);
	return TRUE;
}

gboolean on_menu_help_intro_clicked(GtkWidget * item, gpointer data)
{
	GtkWidget * dialog;
	char * intro = "使用W/S/A/D或者Up/Down/Left/Right键来控制蛇的前进方向,\n"
					"吃到食物时蛇会长大。记住，千万不要撞到活动区边沿，也不\n"
					"要撞到自己，否则会挂的!";
	
	dialog = gtk_message_dialog_new(data,
			 GTK_DIALOG_DESTROY_WITH_PARENT,
			 GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
			 intro, "title");
	
	gtk_window_set_icon_from_file(GTK_WINDOW(dialog), "./icons/icon.png", NULL);
	gtk_window_set_title(GTK_WINDOW(dialog), "贪吃蛇");		 
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	
	return TRUE;
}

gboolean on_menu_help_about_clicked(GtkWidget * item, gpointer data)
{
	GtkWidget * dialog = gtk_about_dialog_new();
	
	GdkPixbuf * logo_buf = gdk_pixbuf_new_from_file("./icons/logo.png", NULL);
	
	gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(dialog), "贪吃蛇");
	gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), "1.0");
	gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), "Copyright (C) 2007 - 2011 庾吉飞编写");
	gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog), logo_buf);
	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), "经常玩\"贪吃蛇\"有利于提高您的反应速度，预防痴呆！");
	gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), "blog.163.com/yujihui_xidian");
	gtk_window_set_icon_from_file(GTK_WINDOW(dialog), "./icons/icon.png", NULL);
	
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	
	return TRUE;
} 
