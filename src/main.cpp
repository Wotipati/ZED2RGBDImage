#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <iomanip>
#include <sstream>

// OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// ZED
#include <sl_zed/Camera.hpp>

// Sample includes
#include <SaveDepth.hpp>

cv::Mat slMat2cvMat(sl::Mat& input);
void printHelp();
void createDirectories();


std::string pathOutputDir;
std::string pathOutputLeftDir;
std::string pathOutputRightDir;
std::string pathOutputDepthDir;


int main(int argc, char **argv) {
    // Create a ZED camera object
    sl::Camera zed;

    // Set configuration parameters
    sl::InitParameters initParams;
    initParams.camera_resolution = sl::RESOLUTION_HD1080;
    initParams.depth_mode        = sl::DEPTH_MODE_PERFORMANCE;
    initParams.coordinate_units  = sl::UNIT_METER;
    if (argc > 1) initParams.svo_input_filename.set(argv[1]);

    // Open the camera
    sl::ERROR_CODE err = zed.open(initParams);
    if (err != sl::SUCCESS) {
        printf("%s\n", toString(err).c_str());
        zed.close();
        return 1; // Quit if an error occurred
    }

    // Prepare directories to save images
    createDirectories();

    // Display help in console
    printHelp();

    // Set runtime parameters after opening the camera
    sl::RuntimeParameters runtimeParameters;
    runtimeParameters.sensing_mode = sl::SENSING_MODE_STANDARD;

    // Prepare new image size to retrieve half-resolution images
    sl::Resolution imageSize = zed.getResolution();
    int inputWidth  = imageSize.width;
    int inputHeight = imageSize.height;

    // To share data between sl::Mat and cv::Mat, use slMat2cvMat()
    sl::Mat leftImageZED (inputWidth, inputHeight, sl::MAT_TYPE_8U_C4);
    sl::Mat rightImageZED(inputWidth, inputHeight, sl::MAT_TYPE_8U_C4);
    sl::Mat depthImageZED(inputWidth, inputHeight, sl::MAT_TYPE_8U_C4);

    cv::Mat leftImageOCV  = slMat2cvMat(leftImageZED);
    cv::Mat rightImageOCV = slMat2cvMat(rightImageZED);
    cv::Mat depthImageOCV = slMat2cvMat(depthImageZED);

    int frameCounter = 0;

    // Loop until 'q' is pressed
    char key = ' ';
    while (key != 'q') {

        if (zed.grab(runtimeParameters) == sl::SUCCESS) {
            // Retrieve the left image, right image, and depth image
            zed.retrieveImage(leftImageZED , sl::VIEW_LEFT , sl::MEM_CPU, inputWidth, inputHeight);
            zed.retrieveImage(rightImageZED, sl::VIEW_RIGHT, sl::MEM_CPU, inputWidth, inputHeight);
            zed.retrieveImage(depthImageZED, sl::VIEW_DEPTH, sl::MEM_CPU, inputWidth, inputHeight);

            // Display image and depth using cv:Mat which share sl:Mat data
            cv::imshow("Left Image" , leftImageOCV);
            cv::imshow("Right Image", rightImageOCV);
            cv::imshow("Depth"      , depthImageOCV);

            // Save Images
            cv::imwrite(pathOutputLeftDir  + "/" + std::to_string(frameCounter) + ".png", leftImageOCV );
            cv::imwrite(pathOutputRightDir + "/" + std::to_string(frameCounter) + ".png", rightImageOCV);
            cv::imwrite(pathOutputDepthDir + "/" + std::to_string(frameCounter) + ".png", depthImageOCV);

            frameCounter += 1;
            // Handle key event
            key = cv::waitKey(10);
            processKeyEvent(zed, key);
        }
    }
    zed.close();
    return 0;
}

cv::Mat slMat2cvMat(sl::Mat& input) {
    // Mapping between MAT_TYPE and CV_TYPE
    int cv_type = -1;
    switch (input.getDataType()) {
        case sl::MAT_TYPE_32F_C1: cv_type = CV_32FC1; break;
        case sl::MAT_TYPE_32F_C2: cv_type = CV_32FC2; break;
        case sl::MAT_TYPE_32F_C3: cv_type = CV_32FC3; break;
        case sl::MAT_TYPE_32F_C4: cv_type = CV_32FC4; break;
        case sl::MAT_TYPE_8U_C1: cv_type = CV_8UC1; break;
        case sl::MAT_TYPE_8U_C2: cv_type = CV_8UC2; break;
        case sl::MAT_TYPE_8U_C3: cv_type = CV_8UC3; break;
        case sl::MAT_TYPE_8U_C4: cv_type = CV_8UC4; break;
        default: break;
    }

    // Since cv::Mat data requires a uchar* pointer, we get the uchar1 pointer from sl::Mat (getPtr<T>())
    // cv::Mat and sl::Mat will share a single memory structure
    return cv::Mat(input.getHeight(), input.getWidth(), cv_type, input.getPtr<sl::uchar1>(sl::MEM_CPU));
}


void printHelp() {
    std::cout << " Press 'q' to quite the process" << std::endl;
//    std::cout << " Press 's' to save Side by side images" << std::endl;
//    std::cout << " Press 'p' to save Point Cloud" << std::endl;
//    std::cout << " Press 'd' to save Depth image" << std::endl;
//    std::cout << " Press 'm' to switch Point Cloud format" << std::endl;
//    std::cout << " Press 'n' to switch Depth format" << std::endl;
}

void createDirectories(){
    pathOutputDir = "./images";
    pathOutputLeftDir  = pathOutputDir +"/left";
    pathOutputRightDir = pathOutputDir +"/right";
    pathOutputDepthDir = pathOutputDir +"/depth";

    mkdir(pathOutputDir.c_str()     , 0755);
    mkdir(pathOutputLeftDir.c_str() , 0755);
    mkdir(pathOutputRightDir.c_str(), 0755);
    mkdir(pathOutputDepthDir.c_str(), 0755);
}