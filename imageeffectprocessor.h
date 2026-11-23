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

}; // namespace ImageEffectProcessor

#endif // IMAGEEFFECTPROCESSOR_H
