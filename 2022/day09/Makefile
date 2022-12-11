all:
	gcc -O3 day09ab_video.c -o day09ab_video

	./day09ab_video < input | \
		ffmpeg -v 0 -y -r 40 -f rawvideo -pixel_format gray -video_size 200x420 \
			-i - -f mp4 -pix_fmt yuv420p -b:v 5000k -s 200x420 \
			video.mp4
