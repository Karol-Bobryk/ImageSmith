#ifndef IMAGEEFFECTPROCESSOR_H
#define IMAGEEFFECTPROCESSOR_H

#include <QImage>

class ImageEffectProcessor
{
public:
    ImageEffectProcessor();

    static QImage applyAdditionEffect(QImage oldImage, QRgb value);
};

#endif // IMAGEEFFECTPROCESSOR_H
