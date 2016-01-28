#include "TestBase.h"
#include "check.h"
#include <iostream>
#include <stdio.h>
#include <boost/filesystem/fstream.hpp>

boost::filesystem::path TestBase::getBoostPath(std::string path) {
    boost::filesystem::path boostPath(boost::filesystem::current_path());
    boostPath = boost::filesystem::system_complete(path);
    return boostPath;
}

int TestBase::getFileCount() {
    return fileCount;
}

void TestBase::findFile(const boost::filesystem::path &path) {
    if (boost::filesystem::is_directory(path)) {
        boost::filesystem::directory_iterator endIter;
        for (boost::filesystem::directory_iterator dirItr(path); dirItr != endIter; ++dirItr) {
            try {
                if (boost::filesystem::is_directory(dirItr->status())) {
                    ++dirCount;
                    findFile(dirItr->path());
                } else if (boost::filesystem::is_regular_file(dirItr->status())) {
                    addFileToPairs(dirItr->path());
                }
            }
            catch (const std::exception & ex) {
                throw ex;
            }
        }
    } else {
        throw std::string("Itsn't directoty: " + path.relative_path().string());
    }
}

void TestBase::addFileToPairs(const boost::filesystem::path &path) {
    if (boost::filesystem::is_regular_file(path)) {
        if (isImageFile(path)) {
            std::string imgFileName = getFileName(path);
            std::string txtFileName = getFileNameWithoutSuffix(path) + ".txt";
            //if (isExistFile(txtFileName)) {
                if (!alreadyAdded(imgFileName)) {
                    ++fileCount;
                    pairs.push_back(PairImgTxt(imgFileName, txtFileName));
                } else {
                    throw std::string("File already added: " + imgFileName);
                }
            /*} else {
                throw std::string("txt-file doesn't exist: " + txtFileName);
            }*/
        }

    } else {
        throw std::string("Itsn't file: " +  path.relative_path().string());
    }
}

bool TestBase::isExistFile(std::string filePath) {
    return boost::filesystem::exists(getBoostPath(filePath));
}

std::string TestBase::getFileName(const boost::filesystem::path &path) {
    std::string fullFileName = "/";
    fullFileName += path.relative_path().string();
    return fullFileName;
}

std::string TestBase::getFileNameWithoutSuffix(const boost::filesystem::path &path) {
    std::string fullFileName = "/";
    fullFileName += path.relative_path().string();
    return fullFileName.substr(0, fullFileName.size() - 4);
}

bool TestBase::isImageFile(const boost::filesystem::path &path) {
    std::string fullFileName = path.relative_path().string();
    std::string fileName = fullFileName.substr(fullFileName.size() - 4, 4);
    if (fileName == ".JPG" || fileName == ".jpg" || fileName == ".png" || fileName == ".bmp"  || fileName == ".PNG") {
        return true;
    }
    return false;
}

bool TestBase::alreadyAdded(std::string fullImageFileName) {
    for (int i = 0; i < pairs.size(); i++) {
        if (pairs[i].img.compare(fullImageFileName) == 0) {
            return true;
        }
    }
    return false;
}

void TestBase::clear() {
    pairs.clear();
    dirCount = 0;
    fileCount = 0;
    idx = 0;
}

TestBase::TestBase(const std::string& pathToBase, bool isWithTxt) {
    clear();
    findFile(getBoostPath(pathToBase));
}

int TestBase::size() {
	return pairs.size();
}

bool TestBase::isEnd() {
	return (idx >= pairs.size());
}

void TestBase::toBegin() {
    idx = 0;
}

void TestBase::next() {
	if (idx < pairs.size()) {
		idx++;
    } else {
        throw std::string("Bad index of vector");
    }
}

std::string& TestBase::getImgFullFileName() {
	if (idx < pairs.size()) {
		return pairs[idx].img;
	} else {
		throw std::string("Bad index of vector");
	}
}

std::string& TestBase::getTxtFullFileName() {
	if (idx < pairs.size()) {
		return pairs[idx].txt;
	} else {
		throw std::string("Bad index of vector");
	}
}

std::string TestBase::getImgFileName() {
	if (idx < pairs.size()) {
		return getLastPartOfFullFileName(pairs[idx].img);
	} else {
		throw std::string("Bad index of vector");
	}
}

std::string TestBase::getTxtFileName() {
	if (idx < pairs.size()) {
		return getLastPartOfFullFileName(pairs[idx].txt);
	} else {
		throw std::string("Bad index of vector");
	}
}

PairImgTxt& TestBase::getPair() {
	if (idx < pairs.size()) {
		return pairs[idx];
	} else {
		throw std::string("Bad index of vector");
	}
}

std::string TestBase::getLastPartOfFullFileName(const std::string& fullFileName) {
	int last_pos = 0;
	for (int i = 0; i < fullFileName.length(); i++) {
		if (fullFileName[i] == '/' || fullFileName[i] == '\\') {
			last_pos = i;
		}
	}
	return fullFileName.substr(last_pos+1);
}

cv::Point TestBase::getCoord(const std::string& coordLine) {
	double x = 0;
	double y = 0;
	for (int i = 0; i < coordLine.length(); i++) {	
		if (coordLine[i] == ' ') {
			std::string tmpStr = coordLine.substr(0, i);
			x = checked_atof(tmpStr);
			tmpStr = coordLine.substr(i+1, coordLine.length() - 2 - i);
			y = checked_atof(tmpStr);
		}	
	}
	return cv::Point(x, y);
}

cv::Point TestBase::getObjectCoord(int position) {
    cv::Point point(-1, -1);
    std::string fullTxtFileName = pairs[idx].txt;
    boost::filesystem::ifstream inFile(fullTxtFileName);

    int currentPosition = 1;
    while ( inFile ) {
        std::string coordLine = "";
        std::getline(inFile, coordLine);
        if (currentPosition == position) {
            point = getCoord(coordLine);
        }
        currentPosition++;
    }
    inFile.close();

    return point;
}

std::string TestBase::getImgFileNameWithoutExtention() {
    std::string str = getImgFileName();
    return str.substr(0, str.find_last_of("."));
}

std::string TestBase::getTxtFileNameWithoutExtention() {
    std::string str = getTxtFileName();
    return str.substr(0, str.find_last_of("."));
}

std::string TestBase::getImgFullFileNameWithoutExtention() {
    std::string str = getImgFullFileName();
    return str.substr(0, str.find_last_of("."));
}

/*Eyes TestBase::getEyes() {
	std::string fullTxtFileName = pairs[idx].txt;
	FILE* txtFile;
	Eye leftEye;
	Eye rightEye;
	char buffer[1024];
	txtFile = fopen(fullTxtFileName.c_str(), "r");
	if (txtFile == 0) {
		return Eyes(Eye(0, 0), Eye(0, 0));
	}
	int currentPosition = 1;
	while (!feof(txtFile)) {
		fgets(buffer, 1024, txtFile);
		std::string coordLine = buffer;
		if (currentPosition == 13) {
			leftEye = getEye(coordLine);
		}
		if (currentPosition == 16) {
			rightEye = getEye(coordLine);
		}
		currentPosition++;
	}
	fclose(txtFile);
	return Eyes(rightEye, leftEye);
}

Eye TestBase::getNose() {
	std::string fullTxtFileName = pairs[idx].txt;
	FILE* txtFile;
	Eye nose;
	Eye rightEye;
	char buffer[1024];
	txtFile = fopen(fullTxtFileName.c_str(), "r");
	if (txtFile == 0) {
		return Eye(0, 0);
	}
	int currentPosition = 1;
	while (!feof(txtFile)) {
		fgets(buffer, 1024, txtFile);
		std::string coordLine = buffer;
		if (currentPosition == 18) {
			nose = getEye(coordLine);
		}
		currentPosition++;
	}
	fclose(txtFile);
	return nose;
}

Eyes TestBase::getEyesCenters() {
	std::string fullTxtFileName = pairs[idx].txt;
	FILE* txtFile;
	Eye leftEye;
	Eye rightEye;
	char buffer[1024];
	txtFile = fopen(fullTxtFileName.c_str(), "r");
	int currentPosition = 1;
	while (!feof(txtFile)) {
		fgets(buffer, 1024, txtFile);
		std::string coordLine = buffer;
		if (currentPosition == 4) {
			leftEye = getEye(coordLine);
		}
		if (currentPosition == 5) {
			rightEye = getEye(coordLine);
		}
		currentPosition++;
	}
	fclose(txtFile);
	return Eyes(rightEye, leftEye);
}*/
