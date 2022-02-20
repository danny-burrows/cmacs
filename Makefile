target = Cmacs

srcs = main.c label.c
libs = -lSDL2 -lSDL2_ttf -lSDL2_image

$(target): $(srcs)
	gcc -o $@ $^ $(libs) 

.PHONY: clean
clean:
	rm $(target)
