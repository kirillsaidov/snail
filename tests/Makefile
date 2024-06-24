FILE=main
INC_DIR_VITA=../third_party/vita/inc
INC_DIR_SNAIL=../inc

all:
	mkdir -p bin && gcc -o bin/$(FILE) -g $(FILE).c -I$(INC_DIR_VITA) -I$(INC_DIR_SNAIL) -L../lib -lsnail -L../third_party/vita/lib -lvita -g
run:
	./bin/$(FILE)
clean:
	rm -rf bin


