#ifndef CONVOLUTIONUTIL_H
#define CONVOLUTIONUTIL_H

#include <QRgb>
#include <cstddef>
#include <qdebug.h>
#include <vector>

namespace ConvolutionUtil {

// treat this bullshit as vectors of M rows with N elements inside ig
[[nodiscard]] std::tuple<float, float, float> convolution(std::vector<QRgb *> data,
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

}; // namespace ConvolutionUtil

#endif // CONVOLUTIONUTIL_H
