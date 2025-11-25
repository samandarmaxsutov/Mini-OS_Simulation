all:build run

run:
	./build/src/mini-os

build:clean
	meson build
	ninja -C build
clean:
	sudo rm -rf build