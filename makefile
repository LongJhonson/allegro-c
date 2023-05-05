CXX=g++
CFLAGS=
LDFLAGS=-L/usr/lib -lallegro -lallegro_main -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_image
INCLUDE=-I. -I/usr/include/allegro5 -I/usr/include/allegro5/allegro -I/usr/include/allegro5/allegro_font -I/usr/include/allegro5/allegro_ttf -I/usr/include/allegro5/allegro_image

OBJS=hello.o
OUT=hello

all: hello_rule

clean:
	        rm -rf *.o hello

hello_rule: $(OBJS)
	        $(CXX) $(OBJS) -o $(OUT) $(INCLUDE) $(CFLAGS) $(LDFLAGS)
