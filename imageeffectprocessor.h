#ifndef IMAGEEFFECTPROCESSOR_H
#define IMAGEEFFECTPROCESSOR_H

#include <QImage>

class ImageEffectProcessor
{
public:
    ImageEffectProcessor();

    static QImage applyAdditionEffect(QImage oldImage, int value = 0);
};

#endif // IMAGEEFFECTPROCESSOR_H
