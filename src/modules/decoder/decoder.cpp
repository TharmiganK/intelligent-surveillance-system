#include "decoder.h"

void decoder::operator()(FrameManager *m1) {
            //imshow( "Display window", image );                   // Show our image inside it.
            //waitKey(0);

            // Open the initial context variables that are needed
            SwsContext *img_convert_ctx;
            AVFormatContext* format_ctx = avformat_alloc_context();
            AVCodecContext* codec_ctx = NULL;
            int video_stream_index;

            // Register everything
            av_register_all();
            avformat_network_init();

            //open RTSP
            std::cout << "CONNECTING" << std::endl;
            if (avformat_open_input(&format_ctx, "rtsp://admin:admin@192.168.1.4:8554/live",
                    NULL, NULL) != 0) {
                return;
            }
            if (avformat_find_stream_info(format_ctx, NULL) < 0) {
                return;
            }
            //search video stream
            for (int i = 0; i < format_ctx->nb_streams; i++) {
                if (format_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
                    video_stream_index = i;
            }

            AVPacket packet;
            av_init_packet(&packet);

            //open output file
            AVFormatContext* output_ctx = avformat_alloc_context();

            AVStream* stream = NULL;
            int cnt = 0;
            //start reading packets from stream and write them to file
            av_read_play(format_ctx);    //play RTSP

            // Get the codec
            AVCodec *codec = NULL;
            codec = avcodec_find_decoder(AV_CODEC_ID_H264);
            if (!codec) {
                exit(1);
            }

            // Add this to allocate the context by codec
            codec_ctx = avcodec_alloc_context3(codec);

            avcodec_get_context_defaults3(codec_ctx, codec);
            avcodec_copy_context(codec_ctx, format_ctx->streams[video_stream_index]->codec);
            std::ofstream output_file;

            if (avcodec_open2(codec_ctx, codec, NULL) < 0)
                exit(1);

            img_convert_ctx = sws_getContext(codec_ctx->width, codec_ctx->height,
                    codec_ctx->pix_fmt, codec_ctx->width, codec_ctx->height, AV_PIX_FMT_BGR24,
                    SWS_BICUBIC, NULL, NULL, NULL);
        
            // std::deque<cv::Mat> queue;
            int size = avpicture_get_size(AV_PIX_FMT_YUV420P, codec_ctx->width,
                    codec_ctx->height);
            uint8_t* picture_buffer = (uint8_t*) (av_malloc(size));
            AVFrame* picture = av_frame_alloc();
            AVFrame* picture_rgb = av_frame_alloc();
            int size2 = avpicture_get_size(AV_PIX_FMT_RGB24, codec_ctx->width,
                    codec_ctx->height);
            uint8_t* picture_buffer_2 = (uint8_t*) (av_malloc(size2));
            avpicture_fill((AVPicture *) picture, picture_buffer, AV_PIX_FMT_YUV420P,
                    codec_ctx->width, codec_ctx->height);
            avpicture_fill((AVPicture *) picture_rgb, picture_buffer_2, AV_PIX_FMT_RGB24,
                    codec_ctx->width, codec_ctx->height);
            std::cout << "STARTING TO LISTEN" << std::endl;

            AVFrame* picturex = av_frame_alloc();
            uint8_t* picture_buffer_x = (uint8_t*) (av_malloc(size2));
            avpicture_fill((AVPicture *) picturex, picture_buffer_x, AV_PIX_FMT_RGB24,
                    codec_ctx->width, codec_ctx->height);
            cv::Mat m2 = cv::Mat(1,1, CV_64F, cv::Scalar(0.));
            int cc = 0;
            while (av_read_frame(format_ctx, &packet) >= 0 && cnt < 10000) { //read ~ 1000 frames

                if (packet.stream_index == video_stream_index) {    //packet is video
                    if (stream == NULL) {    //create stream in file
                        std::cout << "Packet received" << std::endl;
                        stream = avformat_new_stream(output_ctx,
                                format_ctx->streams[video_stream_index]->codec->codec);
                        avcodec_copy_context(stream->codec,
                                format_ctx->streams[video_stream_index]->codec);
                        stream->sample_aspect_ratio =
                                format_ctx->streams[video_stream_index]->codec->sample_aspect_ratio;
                    }
                    int check = 0;
                    packet.stream_index = stream->id;
                    int result = avcodec_decode_video2(codec_ctx, picture, &check, &packet);
                    std::cout << "Decoded frame: " << cnt << std::endl;
                //     std::cout << "Bytes decoded " << result << " check " << check << std::endl;
                    sws_scale(img_convert_ctx, picture->data, picture->linesize, 0,
                                codec_ctx->height, picture_rgb->data, picture_rgb->linesize);
                    cv::Mat image(codec_ctx->height, codec_ctx->width, CV_8UC3, picture_rgb->data[0], picture_rgb->linesize[0]);
                    // queue.push_back(image.clone());
                    // imshow("frame", image);
                    // waitKey(10);
                    cc = cc + 1;
                    // std::lock_guard<std::mutex> guard(myMutex);
                    m1->enqueueFrame(image.clone());
                    // qq->push_back(image.clone());
                    if (cnt == 200)    //cnt < 0)
                            {
                        // picture_buffer_x = picture_buffer_2;
                        std::stringstream file_name;
                        file_name << "test" << cnt << ".ppm";
                        output_file.open(file_name.str().c_str());
                        output_file << "P3 " << codec_ctx->width << " " << codec_ctx->height
                                << " 255\n";
                        for (int y = 0; y < codec_ctx->height; y++) {
                            for (int x = 0; x < codec_ctx->width * 3; x++)
                                output_file
                                        << (int) (picture_rgb->data[0]
                                                + y * picture_rgb->linesize[0])[x] << " ";
                        }
                        output_file.close();
                    }
                    cnt++;
                }
                av_free_packet(&packet);
                av_init_packet(&packet);
            }
            av_free(picture);
            av_free(picture_rgb);
            av_free(picturex);
            av_free(picture_buffer);
            av_free(picture_buffer_2);
            av_free(picture_buffer_x);
            av_read_pause(format_ctx);
            avio_close(output_ctx->pb);
            avformat_free_context(output_ctx);

    }
