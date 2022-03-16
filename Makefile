include   = -I src
srcs     := $(shell find ./src -name '*.c')
inclds   := $(shell find ./src -name '*.h')
src_dirs := $(shell find ./src -type d)

objs      = $(srcs:./src/%.c=./obj/%.o)
obj_dirs  = $(src_dirs:./src%=./obj%)

libs 	  = -lSDL2 -lSDL2_ttf -lSDL2_image

# make config=debug for debug build...
ifeq ($(config), debug)
    build_flags = -g -D DEBUG_CONFIG_SET -fdiagnostics-color=always -Wall -Wextra -Wpedantic
else
	build_flags = -O3
endif

cmacs: $(objs)
	gcc -o $@ $^ $(libs)

obj/%.o: src/%.c $(inclds) | $(obj_dirs)
	gcc -c -o $@ $< $(build_flags) $(include)

$(obj_dirs):
	mkdir $@

.PHONY: clean
clean:
	rm -rf cmacs obj
