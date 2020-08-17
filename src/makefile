CC = g++
INCLUDE = main.cpp modules/frameQueue/frameQueue.cpp modules/decoder/decoder.cpp modules/display/display.cpp modules/streamReceiver/streamReceiver.cpp modules/packetQueue/packetQueue.cpp modules/videoStream/videoStream.cpp
INCLUDE_TEST = test.cpp modules/frameQueue/frameQueue.cpp modules/decoder/decoder.cpp modules/display/display.cpp modules/streamReceiver/streamReceiver.cpp modules/packetQueue/packetQueue.cpp modules/videoStream/videoStream.cpp
PKG = `pkg-config --cflags --libs libavformat libswscale libavcodec libavutil opencv4`
CFLAGS = -lboost_log -lpthread -std=c++0x -pthread -lX11

main: $(INCLUDE)
	$(CC) -w -DBOOST_LOG_DYN_LINK $(INCLUDE) $(CFLAGS) -o $@ $(PKG)

test: $(INCLUDE_TEST)
	$(CC) -w -DBOOST_LOG_DYN_LINK $(INCLUDE_TEST) -lboost_unit_test_framework $(CFLAGS) -o $@ $(PKG)

coverage: $(INCLUDE_TEST)
	$(CC) -fprofile-arcs -ftest-coverage -w -DBOOST_LOG_DYN_LINK $(INCLUDE_TEST) -lboost_unit_test_framework $(CFLAGS) -o $@ $(PKG)
	lcov --capture -i --directory . --output-file base_coverage.info --no-external
	./coverage
	gcov test.cpp
	lcov --capture --directory . --output-file main_coverage.info --no-external
	lcov -a base_coverage.info -a main_coverage.info -o full_coverage.info
	genhtml full_coverage.info --output-directory out

clean:
	rm -f -R *.info *.gcda *.gcno out test coverage

