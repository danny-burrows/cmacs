target = cmacs

include = -I src
srcs := $(shell find ./src -name '*.c')
libs = -lSDL2 -lSDL2_ttf -lSDL2_image

# make config=debug for debug build...
ifeq ($(config), debug)
    build_flags = -g -D DEBUG_CONFIG_SET -fdiagnostics-color=always -Wall -Wextra -Wpedantic
else
	build_flags = -O3
endif

$(target): $(srcs)
	gcc -o $@ $^ $(libs) $(include) $(build_flags)

.PHONY: clean
clean:
	rm $(target)
