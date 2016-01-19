#ifndef FACE_RECOG_
#define FACE_RECOG_

#include <opencv2/core/core.hpp>
#include <opencv2/>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

static cv::Mat norm_0_255(cv::InputArray _src) {
    cv::Mat src = _src.getMat();
    // Create and return normalized image:
    cv::Mat dst;
    switch(src.channels()) {
    case 1:
        cv::normalize(_src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC1);
        break;
    case 3:
        cv::normalize(_src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC3);
        break;
    default:
        src.copyTo(dst);
        break;
    }
    return dst;
}

static void read_csv(const std::string& filename, std::vector<Mat>& images, std::vector<int>& labels, char separator = ';') {
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        std::string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }

    std::string line, path, classlabel;
    while (getline(file, line)) {
        std::stringstream liness(line);
        std::getline(liness, path, separator);
        std::getline(liness, classlabel);
        if(!path.empty() && !classlabel.empty()) {
            images.push_back(imread(path, 0));
            labels.push_back(atoi(classlabel.c_str()));
        }
    }
}

int main(int argc, const char *argv[]) {
    // Check for valid command line arguments, print usage
    // if no arguments were given.
    if (argc < 2) {
        std::cout << "usage: " << argv[0] << " <csv.ext> <output_folder> " << endl;
        exit(1);
    }

    std::string output_folder = ".";
    if (argc == 3) {
        output_folder = string(argv[2]);
    }
    // Get the path to your CSV.
    std::string fn_csv = std::string(argv[1]);
    // These vectors hold the images and corresponding labels.
    std::vector<cv::Mat> images;
    std::vector<int> labels;
    // Read in the data. This can fail if no valid
    // input filename is given.
    try {
        read_csv(fn_csv, images, labels);
    } catch (cv::Exception& e) {
        cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
        // nothing more we can do
        exit(1);
    }
    // Quit if there are not enough images for this demo.
    if(images.size() <= 1) {
        std::string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
        CV_Error(CV_StsError, error_message);
    }
    // Get the height from the first image. We'll need this
    // later in code to reshape the images to their original
    // size:
    int height = images[0].rows;
    // The following lines simply get the last images from
    // your dataset and remove it from the vector. This is
    // done, so that the training data (which we learn the
    // cv::FaceRecognizer on) and the test data we test
    // the model with, do not overlap.
    cv::Mat testSample = images[images.size() - 1];
    int testLabel = labels[labels.size() - 1];
    images.pop_back();
    labels.pop_back();
    // The following lines create an Fisherfaces model for
    // face recognition and train it with the images and
    // labels read from the given CSV file.
    // If you just want to keep 10 Fisherfaces, then call
    // the factory method like this:
    //
    //      cv::createFisherFaceRecognizer(10);
    //
    // However it is not useful to discard Fisherfaces! Please
    // always try to use _all_ available Fisherfaces for
    // classification.
    //
    // If you want to create a FaceRecognizer with a
    // confidence threshold (e.g. 123.0) and use _all_
    // Fisherfaces, then call it with:
    //
    //      cv::createFisherFaceRecognizer(0, 123.0);
    //
    cv::Ptr<cv::FaceRecognizer> model = cv::createFisherFaceRecognizer();
    model->train(images, labels);
    // The following line predicts the label of a given
    // test image:
    int predictedLabel = model->predict(testSample);
    //
    // To get the confidence of a prediction call the model with:
    //
    //      int predictedLabel = -1;
    //      double confidence = 0.0;
    //      model->predict(testSample, predictedLabel, confidence);
    //
    std::string result_message = format("Predicted class = %d / Actual class = %d.", predictedLabel, testLabel);
    std::cout << result_message << endl;
    // Here is how to get the eigenvalues of this Eigenfaces model:
    cv::Mat eigenvalues = model->getMat("eigenvalues");
    // And we can do the same to display the Eigenvectors (read Eigenfaces):
    cv::Mat W = model->getMat("eigenvectors");
    // Get the sample mean from the training data
    cv::Mat mean = model->getMat("mean");
    // Display or save:
    if(argc == 2) {
        cv::imshow("mean", norm_0_255(mean.reshape(1, images[0].rows)));
    } else {
        cv::imwrite(std::format("%s/mean.png", output_folder.c_str()), norm_0_255(mean.reshape(1, images[0].rows)));
    }
    // Display or save the first, at most 16 Fisherfaces:
    for (int i = 0; i < min(16, W.cols); i++) {
        std::string msg = format("Eigenvalue #%d = %.5f", i, eigenvalues.at<double>(i));
        std::cout << msg << endl;
        // get eigenvector #i
        cv::Mat ev = W.col(i).clone();
        // Reshape to original size & normalize to [0...255] for imshow.
        cv::Mat grayscale = norm_0_255(ev.reshape(1, height));
        // Show the image & apply a Bone colormap for better sensing.
        cv::Mat cgrayscale;
        cv::applyColorMap(grayscale, cgrayscale, COLORMAP_BONE);
        // Display or save:
        if(argc == 2) {
            cv::imshow(format("fisherface_%d", i), cgrayscale);
        } else {
            cv::imwrite(format("%s/fisherface_%d.png", output_folder.c_str(), i), norm_0_255(cgrayscale));
        }
    }
    // Display or save the image reconstruction at some predefined steps:
    for(int num_component = 0; num_component < min(16, W.cols); num_component++) {
        // Slice the Fisherface from the model:
        cv::Mat ev = W.col(num_component);
        cv::Mat projection = subspaceProject(ev, mean, images[0].reshape(1,1));
        cv::Mat reconstruction = subspaceReconstruct(ev, mean, projection);
        // Normalize the result:
        reconstruction = norm_0_255(reconstruction.reshape(1, images[0].rows));
        // Display or save:
        if(argc == 2) {
            cv::imshow(format("fisherface_reconstruction_%d", num_component), reconstruction);
        } else {
            cv::imwrite(format("%s/fisherface_reconstruction_%d.png", output_folder.c_str(), num_component), reconstruction);
        }
    }
    // Display if we are not writing to an output folder:
    if(argc == 2) {
        waitKey(0);
    }
    return 0;
}

#endif // FACE_RECOG
