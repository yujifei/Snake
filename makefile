cc = gcc
cflag = `pkg-config --cflags gtk+-2.0`
lflag = `pkg-config --libs gtk+-2.0` -lm
preflag = -g -Wall

game_logical_dep = game_logical.h game_logical.c
gui_dep = gui.h game_logical.h callback.h timer.h gui.c
timer_dep = timer.h game_logical.h gui.h callback.h timer.c
callback_dep = gui.h game_logical.h callback.h timer.h callback.c
main_dep =  gui.h game_logical.h callback.h timer.h main.c
BUILD_MODE = -DDEBUG
target = 贪吃蛇
target_dep = main.o callback.o gui.o timer.o game_logical.o

$(target):$(target_dep)
	$(cc) $(preflag) $(target_dep) -o $(target) $(lflag) 


main.o:$(main_dep)
	$(cc) $(BUILD_MODE) -c $(preflag) $(main_dep) $(cflag)
callback.o:$(callback_dep)
	$(cc) $(BUILD_MODE) -c $(preflag) $(callback_dep) $(cflag)
gui.o:$(gui_dep)
	$(cc) $(BUILD_MODE) -c $(preflag) $(gui_dep) $(cflag)
timer.o:$(timer_dep)
	$(cc) $(BUILD_MODE) -c $(preflag) $(timer_dep) $(cflag)
game_logical.o:$(game_logical_dep)
	$(cc) $(BUILD_MODE) -c $(preflag) $(game_logical_dep) $(cflag)
	
	
clean:
	rm -f *.gch
	
