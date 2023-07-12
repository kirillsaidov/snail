FILE=main
INC_DIR=../third_party

all:
	mkdir -p bin && gcc -o bin/$(FILE) $(FILE).c -I$(INC_DIR) -L../lib -lsnail -L./lib -lvita -g
run:
	./bin/$(FILE)
clean:
	rm -rf bin


