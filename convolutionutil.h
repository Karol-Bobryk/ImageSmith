#ifndef CONVOLUTIONUTIL_H
#define CONVOLUTIONUTIL_H

#include <QRgb>
#include <cstddef>
#include <qdebug.h>
#include <vector>

namespace ConvolutionUtil {
using RgbTuple = std::tuple<float, float, float>;
using NormalizedMask = std::vector<std::vector<float>>;
using TransformPixels
    = std::function<RgbTuple(std::vector<QRgb *> &data, size_t N, size_t M, NormalizedMask mask)>;

// treat this bullshit as vectors of M rows with N elements inside ig
[[nodiscard]] RgbTuple convolution(
    std::vector<QRgb *> data, NormalizedMask mask, size_t N, size_t M, TransformPixels transform);
// less sophisticated version, pixel data passed by value designed rather for convoluting on the edges
// I suspect it could take a toll on performance so be aware
[[nodiscard]] RgbTuple convolutionByValue(std::vector<std::vector<QRgb>> data,
                                          NormalizedMask mask,
                                          TransformPixels transform);
}; // namespace ConvolutionUtil

using TransformFunction = ConvolutionUtil::TransformPixels;
#endif // CONVOLUTIONUTIL_H
