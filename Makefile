CC=g++
CFLAGS=-std=c++0x -lpthread

sa_test:
	$(CC) $(CFLAGS) -DSTANDALONE_TEST pomodoro_cpp_linux.cpp -o exe.out

cli_app:
	$(CC) $(CFLAGS) pomodoro_cli_app.cpp pomodoro_cpp_linux.cpp -o exe.out

clean:
	rm *.out