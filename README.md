# Real-Time Scalable Intelligent Surveillance System


## Requirements

- OpenCV
- FFMPEG


## Usage

```
cd src
```
```
g++ -w -DBOOST_LOG_DYN_LINK main.cpp modules/frameManager/frameManager.cpp modules/decoder/decoder.cpp modules/display/display.cpp modules/streamReceiver/streamReceiver.cpp modules/packetManager/packetManager.cpp -lboost_log -lpthread -o main -std=c++0x -pthread -lX11 $(pkg-config --cflags --libs libavformat libswscale libavcodec libavutil opencv4)
```



## License
[Apache-2.0](https://choosealicense.com/licenses/apache-2.0/)
