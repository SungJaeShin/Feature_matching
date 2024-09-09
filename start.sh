g++ ./src/matching.cpp -L/usr/local/include/opencv2 -I/usr/local/include/opencv2  -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_core -lopencv_stitching -lopencv_xfeatures2d -lopencv_features2d -lopencv_calib3d -lopencv_flann -o matching
./matching
