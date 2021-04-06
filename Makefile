CC=g++
CFLAGS=-std=c++0x -lpthread

no-defaults:
	echo "You can make sa_test, cli_app, or clean"

sa_test: common/pomodoro_cpp_linux.cpp
	$(CC) $(CFLAGS) -DSTANDALONE_TEST $^ -o exe.out

cli_app: pomodoro_cli_app.cpp common/pomodoro_cpp_linux.cpp
	$(CC) $(CFLAGS) $^ -o exe.out

clean:
	rm *.out