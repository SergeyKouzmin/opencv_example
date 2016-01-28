#ifndef FACE_RECOG_
#define FACE_RECOG_

#include <opencv2/core/core.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <boost/format.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <string>
#include "TestBase.h"

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

cv::String FaceCascadeName = "./cascades/haarcascade_frontalface_alt.xml";

cv::CascadeClassifier faceCascade;

void CascadeInit() {
    if (!faceCascade.load(FaceCascadeName)) {
        std::cout << "--(!)Error loading face cascade: " << FaceCascadeName << std::endl;
        exit;
    }
}

void MakeTrainBase(const std::string &basePath, const std::string &resultsPath) {
    TestBase base(basePath);
    std::vector<cv::Rect> faces;
    while (!base.isEnd()) {
        cv::Mat img = cv::imread(base.getImgFullFileName());
        cv::Mat frameGray;
        cv::cvtColor( img, frameGray, CV_BGR2GRAY );
        faceCascade.detectMultiScale(frameGray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
        for (size_t i = 0; i < faces.size(); i++) {
            cv::Mat face = img(faces[i]);
            std::stringstream imgSavePath;
            imgSavePath << resultsPath << "/" << i << "_" << base.getImgFileName();
            cv::imwrite(imgSavePath.str(), face);
        }

        base.next();
        faces.clear();
    }
}

void MakeTrainBase() {
    MakeTrainBase("./train_base/men", "./train_base/men_faces");
    MakeTrainBase("./train_base/women", "./train_base/women_faces");
}

void MakeTrainData(const std::string &basePath, std::vector<cv::Mat> &testImages, std::vector<int> &labels, int label) {
    TestBase base(basePath);
    while (!base.isEnd()) {
        cv::Mat img = cv::imread(base.getImgFullFileName(), 0);
        float scaleWidth = 128.0 / img.size().width;
        float scaleHeight = 128.0 / img.size().height;

        cv::Mat resizeImg;
        cv::resize(img, resizeImg, cv::Size(0,0), scaleWidth, scaleHeight);
        testImages.push_back(resizeImg);
        labels.push_back(label);
        base.next();
    }
}

void MakeTrainData(std::vector<cv::Mat> &testImages, std::vector<int> &labels) {
    MakeTrainData("./train_base/men_faces", testImages, labels, 0);
    MakeTrainData("./train_base/women_faces", testImages, labels, 1);
}

int FaceRecogDemo() {
    std::vector<cv::Mat> testImages;
    std::vector<int> labels;

    CascadeInit();

    //MakeTrainBase();

    MakeTrainData(testImages, labels);

    if(testImages.size() <= 1) {
        std::string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
        CV_Error(CV_StsError, error_message);
    }

    int height = testImages[0].rows;

    cv::Ptr<cv::FaceRecognizer> model = cv::createFisherFaceRecognizer();
    model->train(testImages, labels);
    //TestBase testBase("./train_base/men");
    TestBase testBase("./train_base/women");
    while (!testBase.isEnd()) {
        //cv::Mat testImg = cv::imread("./train_base/12.jpg");
        //cv::Mat testImg = cv::imread("./train_base/6.png");
        cv::Mat testImg = cv::imread(testBase.getImgFullFileName());

        cv::Mat testImgGray;
        cvtColor(testImg, testImgGray, CV_BGR2GRAY);
        std::vector<cv::Rect> faces;
        faceCascade.detectMultiScale(testImgGray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
        cv::Rect maxFace = cv::Rect();
        for (size_t i = 0; i < faces.size(); i++) {
            if (faces[i].area() > maxFace.area()) {
                maxFace = faces[i];
            }
        }
        testImgGray = testImgGray(maxFace);

        float scaleWidth = 128.0 / testImgGray.size().width;
        float scaleHeight = 128.0 / testImgGray.size().height;

        cv::Mat resizeImg;
        cv::resize(testImgGray, resizeImg, cv::Size(0,0), scaleWidth, scaleHeight);
        cv::imwrite("./result.jpg", resizeImg);

        int predict = model->predict(resizeImg);
        std::cout << "predict: " << predict << std::endl;

        testBase.next();
    }
    /*
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
    std::stringstream result_message;
    result_message << "Predicted class = " << predictedLabel << " / Actual class = " << testLabel << ".";

    std::cout << result_message.str() << std::endl;
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
        std::stringstream savePath;
        savePath << output_folder.c_str() << "/mean.png";
        cv::imwrite(format(savePath.str(), norm_0_255(mean.reshape(1, images[0].rows)));
    }
    // Display or save the first, at most 16 Fisherfaces:
    for (int i = 0; i < min(16, W.cols); i++) {
        std::cout << "Eigenvalue " << i << " = " << eigenvalues.at<double>(i) << std::endl;
        // get eigenvector #i
        cv::Mat ev = W.col(i).clone();
        // Reshape to original size & normalize to [0...255] for imshow.
        cv::Mat grayscale = norm_0_255(ev.reshape(1, height));
        // Show the image & apply a Bone colormap for better sensing.
        cv::Mat cgrayscale;
        cv::applyColorMap(grayscale, cgrayscale, COLORMAP_BONE);
        //save
        std::stringstream saveImgPath;
        saveImgPath << output_folder << "/fisherface_" << i << ".png";
        cv::imwrite(saveImgPath.str(), norm_0_255(cgrayscale));
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
    }*/
    return 0;
}

#endif // FACE_RECOG
