#ifndef	_TEST_BASE_H_
#define _TEST_BASE_H_

#include <vector>
#include <string>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "boost/filesystem/operations.hpp"
#include <boost/filesystem/path.hpp>


struct PairImgTxt {

    std::string img;

    std::string txt;

    PairImgTxt() {}
	
	PairImgTxt(std::string img, std::string txt) {
		this->img = img;
		this->txt = txt;
	}

};

class TestBase {

private:

    std::string pathToBase;

    std::vector<PairImgTxt> pairs;
	
    int dirCount;

    int fileCount;
	
	cv::Point getCoord(const std::string& coordLine);

    void findFile(const boost::filesystem::path &p);

    void addFileToPairs(const boost::filesystem::path &path);

    bool isImageFile(const boost::filesystem::path &path);

    std::string getFileName(const boost::filesystem::path &path);

    std::string getFileNameWithoutSuffix(const boost::filesystem::path &path);

    bool isExistFile(std::string filePath);

    bool alreadyAdded(std::string fullImageFileName);

    boost::filesystem::path getBoostPath(std::string path);

    void clear();

public:

    int idx;

    TestBase(const std::string& pathToBase, bool isWithTxt = true);

    int size();

    void next();

    bool isEnd();

    void toBegin();

    std::string& getImgFullFileName();

    std::string getImgFileNameWithoutExtention();

    std::string getImgFullFileNameWithoutExtention();

    std::string getTxtFileNameWithoutExtention();

    std::string getLastPartOfFullFileName(const std::string& fullFileName);

    std::string& getTxtFullFileName();

    std::string getImgFileName();

    std::string getTxtFileName();

    PairImgTxt& getPair();

    cv::Point getObjectCoord(int position);

    int getFileCount();

};

#endif //_TEST_BASE_H_
