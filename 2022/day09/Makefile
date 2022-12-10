all:
	gcc -DVIDEO -O3 day09ab.c -o day09ab

	./day09ab < input | \
		ffmpeg -v 0 -y -r 40 -f rawvideo -pixel_format gray -video_size 200x420 \
			-i - -f mp4 -pix_fmt yuv420p -b:v 5000k -s 200x420 \
			video.mp4
