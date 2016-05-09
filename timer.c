#include<sys/time.h>
#include<gtk/gtk.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>
#include<math.h>
#include "timer.h"
#include "game_logical.h"
#include "gui.h"
#include "callback.h"

void timer_handler(int sig)
{
#ifdef DEBUG
	g_print("timer handler\n");
#endif
	int state = snake_update();
	if(state == -1)
	{
		do_you_dead();
		global_init();
	}
	
	float cur_time = init_time * pow(time_shrink_rate, snake_cur_len / SNAKE_INIT_LEN - 1);
	init_timer(cur_time);
	
	gtk_widget_queue_draw(DrawArea);
	orient_lock = false;
}

void init_sigaction(void (*handler)(int))
{
	struct sigaction act;
	act.sa_handler = handler;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
//	sigaction(SIGVTALRM, &act, NULL);
	signal(SIGALRM, handler);
}

void init_timer(float t)
{
	struct itimerval value;
	value.it_value.tv_sec = (int)t;
	value.it_value.tv_usec = (t - (float)(int)t) * 1000000;
	value.it_interval = value.it_value;

//	alarm(1);	
	setitimer(ITIMER_REAL, &value, NULL);
}
