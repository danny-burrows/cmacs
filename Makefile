target = Cmacs

include = -I src
srcs := $(shell find ./src -name '*.c')
libs = -lSDL2 -lSDL2_ttf -lSDL2_image

$(target): $(srcs)
	gcc -o $@ $^ $(libs) $(include)

.PHONY: clean
clean:
	rm $(target)
