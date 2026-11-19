#ifndef IMAGEEFFECTPROCESSOR_H
#define IMAGEEFFECTPROCESSOR_H

#include <QImage>
#include <QRgbaFloat>

namespace ImageEffectProcessor {

QImage applyAdditionEffect(QImage oldImage, QRgb value);
QImage applySubstractionEffect(QImage oldImage, QRgb value);
QImage applyMultiplicationEffect(QImage oldImage, QRgbaFloat<float> value);
QImage applyDivideEffect(QImage oldImage, QRgbaFloat<float> value);

}; // namespace ImageEffectProcessor

#endif // IMAGEEFFECTPROCESSOR_H
