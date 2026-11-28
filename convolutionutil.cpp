#include "convolutionutil.h"

[[nodiscard]] ConvolutionUtil::RgbTuple ConvolutionUtil::convolution(
    std::vector<QRgb *> data, NormalizedMask mask, size_t N, size_t M, TransformPixels transform)
{
    return transform(data, N, M, mask);
}

[[nodiscard]] ConvolutionUtil::RgbTuple ConvolutionUtil::convolutionByValue(
    std::vector<std::vector<QRgb>> data, NormalizedMask mask, TransformPixels transform)
{
    const size_t M = data.size();
    const size_t N = data[0].size();

    std::vector<QRgb *> data_adapter(M, nullptr);
    for (int i = 0; i < M; ++i)
        data_adapter[i] = data[i].data();

    return transform(data_adapter, M, N, mask);
}