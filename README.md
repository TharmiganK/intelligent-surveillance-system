# Real-Time Scalable Intelligent Surveillance System


## Requirements

- OpenCV
- FFMPEG


## Usage

```
cd App
```
```
g++ -w main.cpp modules/frameManager/frameManager.cpp modules/decoder/decoder.cpp modules/display/display.cpp -o main -std=c++0x -pthread -lX11 $(pkg-config --cflags --libs libavformat libswscale libavcodec libavutil opencv4)
```



## License
[Apache-2.0](https://choosealicense.com/licenses/apache-2.0/)
