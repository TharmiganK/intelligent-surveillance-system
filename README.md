# Real-Time Scalable Intelligent Surveillance System


## Requirements

- OpenCV
- FFMPEG
- Boost
- TBB
- nginx


## Usage

```
cd src
```
```
make
```
```
./main

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
