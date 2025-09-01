#include "Operation.hpp"
#include <algorithm>

Operation::Operation() {}

cv::Mat Operation::toGrayscaleAverage(const cv::Mat& input) {
    cv::Mat output = input.clone();
    
    for (int y = 0; y < input.rows; y++) {
        for (int x = 0; x < input.cols; x++) {
            if (input.channels() == 3) {
                cv::Vec3b pixel = input.at<cv::Vec3b>(y, x);
                uchar media = (pixel[0] + pixel[1] + pixel[2]) / 3;
                output.at<cv::Vec3b>(y, x) = cv::Vec3b(media, media, media);
            }
        }
    }
    return output;
}

cv::Mat Operation::toGrayscaleWeighted(const cv::Mat& input) {
    cv::Mat output = input.clone();
    
    for (int y = 0; y < input.rows; y++) {
        for (int x = 0; x < input.cols; x++) {
            if (input.channels() == 3) {
                cv::Vec3b pixel = input.at<cv::Vec3b>(y, x);
                // Média ponderada padrão ITU-R BT.709: 0.299*R + 0.587*G + 0.114*B
                uchar weighted = static_cast<uchar>(0.114 * pixel[0] + 0.587 * pixel[1] + 0.299 * pixel[2]);
                output.at<cv::Vec3b>(y, x) = cv::Vec3b(weighted, weighted, weighted);
            }
        }
    }
    return output;
}

cv::Mat Operation::addImages(const cv::Mat& img1, const cv::Mat& img2) {
    cv::Mat result;
    cv::add(img1, img2, result);
    return result;
}

cv::Mat Operation::subtractImages(const cv::Mat& img1, const cv::Mat& img2) {
    cv::Mat result;
    cv::subtract(img1, img2, result);
    return result;
}

cv::Mat Operation::multiplyImages(const cv::Mat& img1, const cv::Mat& img2) {
    cv::Mat result;
    cv::multiply(img1, img2, result, 1.0/255.0);
    return result;
}

cv::Mat Operation::divideImages(const cv::Mat& img1, const cv::Mat& img2) {
    cv::Mat result;
    cv::divide(img1, img2, result, 255.0);
    return result;
}

cv::Mat Operation::addScalar(const cv::Mat& input, double scalar) {
    cv::Mat result;
    cv::add(input, cv::Scalar::all(scalar), result);
    return result;
}

cv::Mat Operation::subtractScalar(const cv::Mat& input, double scalar) {
    cv::Mat result;
    cv::subtract(input, cv::Scalar::all(scalar), result);
    return result;
}

cv::Mat Operation::multiplyScalar(const cv::Mat& input, double scalar) {
    cv::Mat result;
    cv::multiply(input, cv::Scalar::all(scalar), result);
    return result;
}

cv::Mat Operation::divideScalar(const cv::Mat& input, double scalar) {
    cv::Mat result;
    cv::divide(input, cv::Scalar::all(scalar), result);
    return result;
}

cv::Mat Operation::threshold(const cv::Mat& input, double thresh, double maxval) {
    cv::Mat result;
    if (input.channels() == 1) {
        cv::threshold(input, result, thresh, maxval, cv::THRESH_BINARY);
    } else {
        cv::Mat gray;
        cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
        cv::threshold(gray, result, thresh, maxval, cv::THRESH_BINARY);
        cv::cvtColor(result, result, cv::COLOR_GRAY2BGR);
    }
    return result;
}

cv::Mat Operation::isolateChannel(const cv::Mat& input, int channel) {
    cv::Mat result = cv::Mat::zeros(input.size(), input.type());
    
    if (input.channels() == 3 && channel >= 0 && channel <= 2) {
        for (int y = 0; y < input.rows; y++) {
            for (int x = 0; x < input.cols; x++) {
                cv::Vec3b pixel = input.at<cv::Vec3b>(y, x);
                cv::Vec3b newPixel(0, 0, 0);
                newPixel[channel] = pixel[channel];
                result.at<cv::Vec3b>(y, x) = newPixel;
            }
        }
    }
    return result;
}

std::vector<cv::Mat> Operation::computeHistogram(const cv::Mat& input) {
    std::vector<cv::Mat> histograms;
    
    if (input.channels() == 1) {
        cv::Mat hist;
        int histSize = 256;
        float range[] = {0, 256};
        const float* histRange = {range};
        cv::calcHist(&input, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange);
        histograms.push_back(hist);
    } else {
        std::vector<cv::Mat> bgr_planes;
        cv::split(input, bgr_planes);
        
        int histSize = 256;
        float range[] = {0, 256};
        const float* histRange = {range};
        
        for (int i = 0; i < 3; i++) {
            cv::Mat hist;
            cv::calcHist(&bgr_planes[i], 1, 0, cv::Mat(), hist, 1, &histSize, &histRange);
            histograms.push_back(hist);
        }
    }
    
    return histograms;
}

cv::Mat Operation::visualizeHistogram(const std::vector<cv::Mat>& histograms) {
    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound((double)hist_w / 256);
    
    cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));
    
    for (size_t i = 0; i < histograms.size(); i++) {
        cv::Mat hist = histograms[i];
        cv::normalize(hist, hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
        
        cv::Scalar color;
        if (histograms.size() == 1) {
            color = cv::Scalar(255, 255, 255); // Branco para escala de cinza
        } else {
            if (i == 0) color = cv::Scalar(255, 0, 0);      // Azul
            else if (i == 1) color = cv::Scalar(0, 255, 0); // Verde
            else color = cv::Scalar(0, 0, 255);             // Vermelho
        }
        
        for (int j = 1; j < 256; j++) {
            cv::line(histImage,
                cv::Point(bin_w * (j - 1), hist_h - cvRound(hist.at<float>(j - 1))),
                cv::Point(bin_w * j, hist_h - cvRound(hist.at<float>(j))),
                color, 2, 8, 0);
        }
    }
    
    return histImage;
}

cv::Mat Operation::invert(const cv::Mat& input) {
    cv::Mat result;
    cv::bitwise_not(input, result);
    return result;
}

cv::Mat Operation::equalizeHistogram(const cv::Mat& input) {
    cv::Mat result;
    
    if (input.channels() == 1) {
        // Equalização para imagem em escala de cinza
        cv::equalizeHist(input, result);
    } else {
        // Equalização para imagem colorida (por canal)
        std::vector<cv::Mat> channels;
        cv::split(input, channels);
        
        for (int i = 0; i < 3; i++) {
            cv::equalizeHist(channels[i], channels[i]);
        }
        
        cv::merge(channels, result);
    }
    
    return result;
}
