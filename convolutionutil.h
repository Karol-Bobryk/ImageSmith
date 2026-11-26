#ifndef CONVOLUTIONUTIL_H
#define CONVOLUTIONUTIL_H

#include <QRgb>
#include <cstddef>
#include <qdebug.h>
#include <vector>

namespace ConvolutionUtil {
using RgbTuple = std::tuple<float, float, float>;
// treat this bullshit as vectors of M rows with N elements inside ig
[[nodiscard]] RgbTuple convolution(std::vector<QRgb *> data,
                                   std::vector<std::vector<float>> mask,
                                   size_t N,
                                   size_t M)
{
    float red_sum = 0.0f;
    float green_sum = 0.0f;
    float blue_sum = 0.0f;

    for (int y = 0; y < M; ++y) {
        for (int x = 0; x < N; ++x) {
            red_sum += qRed(data[y][x]) * mask[y][x];
            green_sum += qGreen(data[y][x]) * mask[y][x];
            blue_sum += qBlue(data[y][x]) * mask[y][x];
        }
    }

    return std::make_tuple(red_sum, green_sum, blue_sum);
}

// less sophisticated version, pixel data passed by value designed rather for convoluting on the edges
// I suspect it could take a toll on performance so be aware
[[nodiscard]] RgbTuple convolutionByValue(std::vector<std::vector<QRgb>> data,
                                          std::vector<std::vector<float>> mask)
{
    float red_sum = 0.0f;
    float green_sum = 0.0f;
    float blue_sum = 0.0f;

    const size_t M = data.size();
    const size_t N = data[0].size();

    for (int y = 0; y < M; ++y) {
        for (int x = 0; x < N; ++x) {
            red_sum += qRed(data[y][x]) * mask[y][x];
            green_sum += qGreen(data[y][x]) * mask[y][x];
            blue_sum += qBlue(data[y][x]) * mask[y][x];
        }
    }

    return std::make_tuple(red_sum, green_sum, blue_sum);
}
}; // namespace ConvolutionUtil

#endif // CONVOLUTIONUTIL_H
