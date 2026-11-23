#ifndef IMAGEEFFECTUTIL_H
#define IMAGEEFFECTUTIL_H

#include <stdexcept>

namespace ImageEffectUtil {
enum class ImageEffect {
    //per point effects
    ADD_VALUE,
    SUBSTRACT_VALUE,
    MULTIPLY_VALUE,
    DIVIDE_VALUE,
    CHANGE_BRIGHTNESS,
    CONVERT_TO_GRAYSCALE_1,
    CONVERT_TO_GRAYSCALE_2,
    // filters
    SMOOTHING_FILTER,
    MEDIAN_FILTER,
    EDGE_DETECT,
    HIGH_PASS_FILTER,
    GAUSSIAN_BLUR,
    CONVOLUTION,
};

int getEffectId(ImageEffect effect)
{
    switch (effect) {
    case ImageEffect::ADD_VALUE:
        return 0;
    case ImageEffect::SUBSTRACT_VALUE:
        return 1;
    case ImageEffect::MULTIPLY_VALUE:
        return 2;
    case ImageEffect::DIVIDE_VALUE:
        return 3;
    case ImageEffect::CHANGE_BRIGHTNESS:
        return 4;
    case ImageEffect::CONVERT_TO_GRAYSCALE_1:
        return 5;
    case ImageEffect::CONVERT_TO_GRAYSCALE_2:
        return 6;
    case ImageEffect::SMOOTHING_FILTER:
        return 7;
    case ImageEffect::MEDIAN_FILTER:
        return 8;
    case ImageEffect::EDGE_DETECT:
        return 9;
    case ImageEffect::HIGH_PASS_FILTER:
        return 10;
    case ImageEffect::GAUSSIAN_BLUR:
        return 11;
    case ImageEffect::CONVOLUTION:
        return 12;
    default:
        throw std::runtime_error("Invalid ImageEffect value");
    }
};

ImageEffect getEffectById(int id)
{
    switch (id) {
    case 0:
        return ImageEffect::ADD_VALUE;
    case 1:
        return ImageEffect::SUBSTRACT_VALUE;
    case 2:
        return ImageEffect::MULTIPLY_VALUE;
    case 3:
        return ImageEffect::DIVIDE_VALUE;
    case 4:
        return ImageEffect::CHANGE_BRIGHTNESS;
    case 5:
        return ImageEffect::CONVERT_TO_GRAYSCALE_1;
    case 6:
        return ImageEffect::CONVERT_TO_GRAYSCALE_2;
    case 7:
        return ImageEffect::SMOOTHING_FILTER;
    case 8:
        return ImageEffect::MEDIAN_FILTER;
    case 9:
        return ImageEffect::EDGE_DETECT;
    case 10:
        return ImageEffect::HIGH_PASS_FILTER;
    case 11:
        return ImageEffect::GAUSSIAN_BLUR;
    case 12:
        return ImageEffect::CONVOLUTION;
    default:
        throw std::runtime_error("Invalid ImageEffect id");
    }
};
}; // namespace ImageEffectUtil

using ImageEffect = ImageEffectUtil::ImageEffect;

#endif // IMAGEEFFECTUTIL_H
