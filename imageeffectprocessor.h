#ifndef IMAGEEFFECTPROCESSOR_H
#define IMAGEEFFECTPROCESSOR_H

#include <QImage>
#include <QRgbaFloat>

namespace ImageEffectProcessor {

QImage applyAdditionEffect(QImage oldImage, QRgb value);
QImage applySubstractionEffect(QImage oldImage, QRgb value);
QImage applyMultiplicationEffect(QImage oldImage, QRgbaFloat<float> value);
QImage applyDivideEffect(QImage oldImage, QRgbaFloat<float> value);
QImage applyChangeBrightness(QImage oldImage, float ratio);
QImage applyGrayscaleConversionColorometric(QImage oldImage);
QImage applyGrayscaleConversionLinear(QImage oldImage);
// filters
QImage applySmoothingFilter(QImage oldImage);
QImage applyConvolution(QImage oldImage);

void convoluteScanline(
    std::vector<QRgb *> scanlines,
    std::vector<std::vector<float>> mask,
    QRgb *out,
    size_t len,
    size_t x_offset = 0 // this is the offset taken from both the left and right of the scanline
);

std::vector<QRgb *> getScanlineRange(
    QImage image,
    int range_start,
    int range_end,
    QRgb *fake_scanline = nullptr // used for out of bounds scanline requests,
                                  // has to be allocated by the user
);
// if data is taken out of range this function will compensate  by default value
std::vector<std::vector<QRgb>> createRgbMatrixFromImage(QImage image,
                                                        int center_x,
                                                        int center_y,
                                                        size_t width,
                                                        size_t height,
                                                        QRgb default_value = 0x00000000);
}; // namespace ImageEffectProcessor

#endif // IMAGEEFFECTPROCESSOR_H
