
编译命令:
环境需要依赖opencv库

gcc -o test opencv_imgproc.cpp -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_videoio -lstdc++

gcc -o test1 opencv_imgproc1.cpp -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_videoio -lstdc++

gcc -o test2 opencv_imgproc2.cpp -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_videoio -lstdc++

gcc -o test3 opencv_imgproc3.cpp -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_videoio -lstdc++

gcc -o test4 opencv_imgproc4.cpp -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_videoio -lstdc++

gcc -o test_face opencv_face.cpp -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_videoio -lstdc++ -lopencv_objdetect -lm
