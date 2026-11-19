#include "imageeffectprocessor.h"

ImageEffectProcessor::ImageEffectProcessor() {}

QImage ImageEffectProcessor::applyAdditionEffect(QImage oldImage, QRgb value)
{
    QImage newImage = oldImage.copy();

    for (int y = 0; y < newImage.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb *>(newImage.scanLine(y));
        for (int x = 0; x < newImage.width(); ++x) {
            QRgb &rgb = line[x];
            line[x] = qRgba(std::min(qRed(rgb) + qRed(value), 255),
                            std::min(qGreen(rgb) + qGreen(value), 255),
                            std::min(qBlue(rgb) + qBlue(value), 255),
                            std::min(qAlpha(rgb) + qAlpha(value), 255));
        }
    }

    return newImage;
}