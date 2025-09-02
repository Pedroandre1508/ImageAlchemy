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
    int rows = std::min(img1.rows, img2.rows);
    int cols = std::min(img1.cols, img2.cols);
    int channels = img1.channels(); // Assumindo que ambas têm o mesmo número de canais
    cv::Mat result(rows, cols, img1.type());
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            for (int c = 0; c < channels; c++) {
                int sum = img1.at<cv::Vec3b>(y, x)[c] + img2.at<cv::Vec3b>(y, x)[c];
                result.at<cv::Vec3b>(y, x)[c] = std::min(sum, 255);
            }
        }
    }
    return result;
}

cv::Mat Operation::subtractImages(const cv::Mat& img1, const cv::Mat& img2) {
    int rows = std::min(img1.rows, img2.rows);
    int cols = std::min(img1.cols, img2.cols);
    int channels = img1.channels();
    cv::Mat result(rows, cols, img1.type());
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            for (int c = 0; c < channels; c++) {
                int diff = img1.at<cv::Vec3b>(y, x)[c] - img2.at<cv::Vec3b>(y, x)[c];
                result.at<cv::Vec3b>(y, x)[c] = std::max(diff, 0);
            }
        }
    }
    return result;
}

cv::Mat Operation::multiplyImages(const cv::Mat& img1, const cv::Mat& img2) {
    int rows = std::min(img1.rows, img2.rows);
    int cols = std::min(img1.cols, img2.cols);
    int channels = img1.channels();
    cv::Mat result(rows, cols, img1.type());
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            for (int c = 0; c < channels; c++) {
                int prod = img1.at<cv::Vec3b>(y, x)[c] * img2.at<cv::Vec3b>(y, x)[c] / 255;
                result.at<cv::Vec3b>(y, x)[c] = std::min(prod, 255);
            }
        }
    }
    return result;
}

cv::Mat Operation::divideImages(const cv::Mat& img1, const cv::Mat& img2) {
    int rows = std::min(img1.rows, img2.rows);
    int cols = std::min(img1.cols, img2.cols);
    int channels = img1.channels();
    cv::Mat result(rows, cols, img1.type());
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            for (int c = 0; c < channels; c++) {
                uchar denominator = img2.at<cv::Vec3b>(y, x)[c];
                uchar numerator = img1.at<cv::Vec3b>(y, x)[c];
                uchar value = denominator == 0 ? 255 : std::min((numerator * 255) / denominator, 255);
                result.at<cv::Vec3b>(y, x)[c] = value;
            }
        }
    }
    return result;
}

cv::Mat Operation::addScalar(const cv::Mat& input, double scalar) {
    cv::Mat result = input.clone();
    int rows = input.rows;
    int cols = input.cols;
    int channels = input.channels();

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            if (channels == 1) {
                int sum = input.at<uchar>(y, x) + scalar;
                result.at<uchar>(y, x) = std::min(sum, 255);
            } else if (channels == 3) {
                cv::Vec3b pixel = input.at<cv::Vec3b>(y, x);
                for (int c = 0; c < 3; c++) {
                    int sum = pixel[c] + scalar;
                    result.at<cv::Vec3b>(y, x)[c] = std::min(sum, 255);
                }
            }
        }
    }
    return result;
}

cv::Mat Operation::subtractScalar(const cv::Mat& input, double scalar) {
    cv::Mat result = input.clone();
    int rows = input.rows;
    int cols = input.cols;
    int channels = input.channels();

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            if (channels == 1) {
                int diff = input.at<uchar>(y, x) - scalar;
                result.at<uchar>(y, x) = std::max(diff, 0);
            } else if (channels == 3) {
                cv::Vec3b pixel = input.at<cv::Vec3b>(y, x);
                for (int c = 0; c < 3; c++) {
                    int diff = pixel[c] - scalar;
                    result.at<cv::Vec3b>(y, x)[c] = std::max(diff, 0);
                }
            }
        }
    }
    return result;
}

cv::Mat Operation::multiplyScalar(const cv::Mat& input, double scalar) {
    cv::Mat result = input.clone();
    int rows = input.rows;
    int cols = input.cols;
    int channels = input.channels();

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            if (channels == 1) {
                int prod = static_cast<int>(input.at<uchar>(y, x) * scalar);
                result.at<uchar>(y, x) = std::min(prod, 255);
            } else if (channels == 3) {
                cv::Vec3b pixel = input.at<cv::Vec3b>(y, x);
                for (int c = 0; c < 3; c++) {
                    int prod = static_cast<int>(pixel[c] * scalar);
                    result.at<cv::Vec3b>(y, x)[c] = std::min(prod, 255);
                }
            }
        }
    }
    return result;
}

cv::Mat Operation::divideScalar(const cv::Mat& input, double scalar) {
    cv::Mat result = input.clone();
    int rows = input.rows;
    int cols = input.cols;
    int channels = input.channels();

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            if (channels == 1) {
                int value = scalar == 0 ? 255 : static_cast<int>(input.at<uchar>(y, x) / scalar);
                result.at<uchar>(y, x) = std::min(value, 255);
            } else if (channels == 3) {
                cv::Vec3b pixel = input.at<cv::Vec3b>(y, x);
                for (int c = 0; c < 3; c++) {
                    int value = scalar == 0 ? 255 : static_cast<int>(pixel[c] / scalar);
                    result.at<cv::Vec3b>(y, x)[c] = std::min(value, 255);
                }
            }
        }
    }
    return result;
}

cv::Mat Operation::threshold(const cv::Mat& input, double thresh, double maxval) {
    cv::Mat result = input.clone();
    int rows = input.rows;
    int cols = input.cols;
    int channels = input.channels();

    if (channels == 1) {
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < cols; x++) {
                uchar pixel = input.at<uchar>(y, x);
                result.at<uchar>(y, x) = (pixel > thresh) ? static_cast<uchar>(maxval) : 0;
            }
        }
    } else if (channels == 3) {
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < cols; x++) {
                cv::Vec3b pixel = input.at<cv::Vec3b>(y, x);
                // Converte para cinza usando média ponderada
                uchar gray = static_cast<uchar>(0.114 * pixel[0] + 0.587 * pixel[1] + 0.299 * pixel[2]);
                uchar value = (gray > thresh) ? static_cast<uchar>(maxval) : 0;
                result.at<cv::Vec3b>(y, x) = cv::Vec3b(value, value, value);
            }
        }
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
    } else if (input.channels() == 1 && channel == 0) {
        // Para imagens de 1 canal, apenas copia
        for (int y = 0; y < input.rows; y++) {
            for (int x = 0; x < input.cols; x++) {
                result.at<uchar>(y, x) = input.at<uchar>(y, x);
            }
        }
    }
    return result;
}

std::vector<cv::Mat> Operation::computeHistogram(const cv::Mat& input) {
    std::vector<cv::Mat> histograms;
    int histSize = 256;

    if (input.channels() == 1) {
        cv::Mat hist = cv::Mat::zeros(1, histSize, CV_32S);
        for (int y = 0; y < input.rows; y++) {
            for (int x = 0; x < input.cols; x++) {
                int val = input.at<uchar>(y, x);
                hist.at<int>(val)++;
            }
        }
        histograms.push_back(hist);
    } else if (input.channels() == 3) {
        for (int c = 0; c < 3; c++) {
            cv::Mat hist = cv::Mat::zeros(1, histSize, CV_32S);
            for (int y = 0; y < input.rows; y++) {
                for (int x = 0; x < input.cols; x++) {
                    int val = input.at<cv::Vec3b>(y, x)[c];
                    hist.at<int>(val)++;
                }
            }
            histograms.push_back(hist);
        }
    }
    return histograms;
}

cv::Mat Operation::visualizeHistogram(const std::vector<cv::Mat>& histograms) {
    int hist_w = 512, hist_h = 400;
    int bin_w = hist_w / 256;
    cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));

    // Encontrar o valor máximo para normalizar
    int maxVal = 0;
    for (const auto& hist : histograms) {
        for (int i = 0; i < 256; i++) {
            maxVal = std::max(maxVal, hist.at<int>(i));
        }
    }
    if (maxVal == 0) maxVal = 1;

    for (size_t c = 0; c < histograms.size(); c++) {
        cv::Scalar color;
        if (histograms.size() == 1) color = cv::Scalar(255, 255, 255);
        else if (c == 0) color = cv::Scalar(255, 0, 0);
        else if (c == 1) color = cv::Scalar(0, 255, 0);
        else color = cv::Scalar(0, 0, 255);

        for (int i = 1; i < 256; i++) {
            int h1 = cvRound((double)histograms[c].at<int>(i - 1) * hist_h / maxVal);
            int h2 = cvRound((double)histograms[c].at<int>(i) * hist_h / maxVal);
            cv::line(histImage,
                cv::Point(bin_w * (i - 1), hist_h - h1),
                cv::Point(bin_w * i, hist_h - h2),
                color, 2, 8, 0);
        }
    }
    return histImage;
}

cv::Mat Operation::invert(const cv::Mat& input) {
    cv::Mat result = input.clone();
    int rows = input.rows;
    int cols = input.cols;
    int channels = input.channels();

    if (channels == 1) {
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < cols; x++) {
                result.at<uchar>(y, x) = 255 - input.at<uchar>(y, x);
            }
        }
    } else if (channels == 3) {
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < cols; x++) {
                cv::Vec3b pixel = input.at<cv::Vec3b>(y, x);
                result.at<cv::Vec3b>(y, x) = cv::Vec3b(255 - pixel[0], 255 - pixel[1], 255 - pixel[2]);
            }
        }
    }
    return result;
}

cv::Mat Operation::equalizeHistogram(const cv::Mat& input) {
    cv::Mat result = input.clone();
    int rows = input.rows;
    int cols = input.cols;

    if (input.channels() == 1) {
        // Calcular histograma
        int hist[256] = {0};
        for (int y = 0; y < rows; y++)
            for (int x = 0; x < cols; x++)
                hist[input.at<uchar>(y, x)]++;

        // Calcular CDF
        int cdf[256] = {0};
        cdf[0] = hist[0];
        for (int i = 1; i < 256; i++)
            cdf[i] = cdf[i - 1] + hist[i];

        // Normalizar CDF
        int cdf_min = 0;
        for (int i = 0; i < 256; i++) {
            if (cdf[i] != 0) {
                cdf_min = cdf[i];
                break;
            }
        }
        int total = rows * cols;
        uchar lut[256];
        for (int i = 0; i < 256; i++) {
            lut[i] = static_cast<uchar>(std::round((cdf[i] - cdf_min) * 255.0 / (total - cdf_min)));
        }

        // Aplicar LUT
        for (int y = 0; y < rows; y++)
            for (int x = 0; x < cols; x++)
                result.at<uchar>(y, x) = lut[input.at<uchar>(y, x)];
    } else if (input.channels() == 3) {
        // Equalizar cada canal separadamente
        for (int c = 0; c < 3; c++) {
            int hist[256] = {0};
            for (int y = 0; y < rows; y++)
                for (int x = 0; x < cols; x++)
                    hist[input.at<cv::Vec3b>(y, x)[c]]++;

            int cdf[256] = {0};
            cdf[0] = hist[0];
            for (int i = 1; i < 256; i++)
                cdf[i] = cdf[i - 1] + hist[i];

            int cdf_min = 0;
            for (int i = 0; i < 256; i++) {
                if (cdf[i] != 0) {
                    cdf_min = cdf[i];
                    break;
                }
            }
            int total = rows * cols;
            uchar lut[256];
            for (int i = 0; i < 256; i++) {
                lut[i] = static_cast<uchar>(std::round((cdf[i] - cdf_min) * 255.0 / (total - cdf_min)));
            }

            for (int y = 0; y < rows; y++)
                for (int x = 0; x < cols; x++)
                    result.at<cv::Vec3b>(y, x)[c] = lut[input.at<cv::Vec3b>(y, x)[c]];
        }
    }
    return result;
}