
#include "opencv2/opencv.hpp"
#include <sstream>
#include <fstream>
#include <dirent.h>
#include <algorithm>

using namespace std;
using namespace cv;
 
void ReadIntrinsics(Mat &cameraMatrix, Mat &distCoeffs, Size &imageSize, char *IntrinsicsPath)
{
	bool FSflag = false;
	FileStorage readfs;
 
	FSflag = readfs.open(IntrinsicsPath, FileStorage::READ);
	
	if (FSflag == false) cout << "Cannot open the file" << endl;
	readfs["Camera_Matrix"] >> cameraMatrix;
	readfs["Distortion_Coefficients"] >> distCoeffs;
	readfs["image_Width"] >> imageSize.width;
	readfs["image_Height"] >> imageSize.height;
 
	cout << cameraMatrix << endl << distCoeffs << endl << imageSize << endl;
 
	readfs.release();
}
 
void Undistort_img(Mat map1, Mat map2, string input_path, string out_path)
{

	Mat img1, img2;
	img1 = imread(input_path);
	if (img1.empty()) cout << "Cannot open the image" << endl;
	remap(img1, img2, map1, map2, INTER_LINEAR);
	imwrite(out_path, img2);
	// imshow("src img", img1);
	// imshow("dst img", img2);
	// waitKey();
}
 
// int example()
// {
// 	Mat	cameraMatrix, distCoeffs, map1, map2;
// 	Size imageSize;
// 	char * IntrinsicsPath = "/media/wq/JackieData/data/leopard/camera.yml";
// 	std::cout << "before read intrinsics" << std::endl;
// 	ReadIntrinsics(cameraMatrix, distCoeffs, imageSize, IntrinsicsPath);
 
// 	// 去畸变并保留最大图
// 	initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(),
// 		getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, imageSize, 1, imageSize, 0),
// 		imageSize, CV_16SC2, map1, map2);
 
// 	Undistort_img(map1, map2, "/media/wq/JackieData/data/leopard/images/1586941796698182522.jpg");
 
// 	// 去畸变至全图
// 	initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(), Mat(),
// 		imageSize, CV_16SC2, map1, map2);
 
//     Undistort_img(map1, map2, "/media/wq/JackieData/data/leopard/images/1586941796698182522.jpg");
//     return 0;
// }

void output_undistorted_img(string img_path, string out_path) {
	static bool mapping_flag{true};
	static Mat	cameraMatrix, distCoeffs, map1, map2;
	static Size imageSize;
	if(mapping_flag) {
		mapping_flag = false;
		char * IntrinsicsPath = "/media/wq/JackieData/data/leopard/camera.yml";
		std::cout << "before read intrinsics" << std::endl;
		ReadIntrinsics(cameraMatrix, distCoeffs, imageSize, IntrinsicsPath);
		// 去畸变至全图
		initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(), Mat(),
			imageSize, CV_16SC2, map1, map2);
	}
	
	Undistort_img(map1, map2, img_path, out_path);
}

int main(int argc, char** argv) {
	char* image_folder = argv[1];
	std::vector<std::string> files;
	DIR *dp;
	struct dirent *dirp;
    if((dp  = opendir(image_folder)) == NULL) {
        return -1;
    }
	while ((dirp = readdir(dp)) != NULL) {
    	std::string name = std::string(dirp->d_name);

    	if(name != "." && name != "..") {
    		files.push_back(name);
			std::cout << name << std::endl;
		}
    }
    closedir(dp);
	std::sort(files.begin(), files.end());
	for(int img_cnt = 0; img_cnt < files.size(); ++img_cnt) {
		string img_name = string(image_folder) + '/' + files[img_cnt];
		cv::Mat img1 = cv::imread(img_name);
		// imshow("src img", img1);
		// waitKey();
		string out_path = "/media/wq/JackieData/data/leopard/undistorted_img";
		out_path = out_path + '/' + files[img_cnt];
		output_undistorted_img(img_name, out_path);
	}
	return 0;

}