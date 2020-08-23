# Real-Time Scalable Intelligent Surveillance System


## Requirements

- OpenCV
- FFMPEG
- Boost
- nginx


## Usage

In develop branch

```
cd src
```
```
g++ -w -DBOOST_LOG_DYN_LINK main.cpp modules/frameQueue/frameQueue.cpp modules/decoder/decoder.cpp modules/display/display.cpp modules/streamReceiver/streamReceiver.cpp modules/packetQueue/packetQueue.cpp modules/videoStream/videoStream.cpp  modules/outputStreamer/outputStreamer.cpp modules/httpServer/httpServer.cpp modules/httpServer/httpService.cpp modules/httpServer/httpAcceptor.cpp -lboost_log -lpthread -o main -std=c++0x -pthread -lX11 $(pkg-config --cflags --libs libavformat libswscale libavcodec libavutil opencv4)

```

Install nginx with rtmp support 

Add below part to support streaming in nginx.conf

```
rtmp {
        server {
                listen 1935;
                chunk_size 4096;

                application live {
                        live on;
                        record off;
						allow publish all;
						allow play all;
                        #push rtmp://localhost/live/stream;
                }
        }
}
```

## License
[Apache-2.0](https://choosealicense.com/licenses/apache-2.0/)
