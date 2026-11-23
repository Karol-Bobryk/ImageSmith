#include "imageeffectprocessor.h"
#include "userinpututil.h"

#include <QRgbaFloat16>
#include <QtCore/qdebug.h>

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

QImage ImageEffectProcessor::applySubstractionEffect(QImage oldImage, QRgb value)
{
    QImage newImage = oldImage.copy();

    for (int y = 0; y < newImage.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb *>(newImage.scanLine(y));
        for (int x = 0; x < newImage.width(); ++x) {
            QRgb &rgb = line[x];
            line[x] = qRgba(std::max(qRed(rgb) - qRed(value), 0),
                            std::max(qGreen(rgb) - qGreen(value), 0),
                            std::max(qBlue(rgb) - qBlue(value), 0),
                            std::max(qAlpha(rgb) - qAlpha(value), 0));
        }
    }

    return newImage;
}

QImage ImageEffectProcessor::applyMultiplicationEffect(QImage oldImage, QRgbaFloat<float> value)
{
    QImage newImage = oldImage.copy();

    for (int y = 0; y < newImage.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb *>(newImage.scanLine(y));
        for (int x = 0; x < newImage.width(); ++x) {
            QRgb &rgb = line[x];
            line[x] = qRgba((int) UserInputUtil::clamp(qRed(rgb) * value.red(),
                                                       InputType::RGBA_UNIT),
                            (int) UserInputUtil::clamp(qGreen(rgb) * value.green(),
                                                       InputType::RGBA_UNIT),
                            (int) UserInputUtil::clamp(qBlue(rgb) * value.blue(),
                                                       InputType::RGBA_UNIT),
                            (int) UserInputUtil::clamp(qAlpha(rgb) * value.alpha(),
                                                       InputType::RGBA_UNIT));
        }
    }

    return newImage;
}

QImage ImageEffectProcessor::applyDivideEffect(QImage oldImage, QRgbaFloat<float> value)
{
    QImage newImage = oldImage.copy();

    float red = value.red();
    float green = value.green();
    float blue = value.blue();
    float alpha = value.alpha();

    if (red == 0 || green == 0 || blue == 0 || alpha == 0) {
        throw std::runtime_error("cannot divide by zero");
    }

    for (int y = 0; y < newImage.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb *>(newImage.scanLine(y));
        for (int x = 0; x < newImage.width(); ++x) {
            QRgb &rgb = line[x];
            line[x] = qRgba((int) UserInputUtil::clamp((float) qRed(rgb) / red,
                                                       InputType::RGBA_UNIT),
                            (int) UserInputUtil::clamp((float) qGreen(rgb) / green,
                                                       InputType::RGBA_UNIT),
                            (int) UserInputUtil::clamp((float) qBlue(rgb) / blue,
                                                       InputType::RGBA_UNIT),
                            (int) UserInputUtil::clamp((float) qAlpha(rgb) / alpha,
                                                       InputType::RGBA_UNIT));
        }
    }

    return newImage;
}

QImage ImageEffectProcessor::applyChangeBrightness(QImage oldImage, float ratio)
{
    QImage newImage = oldImage.copy();

    for (int y = 0; y < newImage.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb *>(newImage.scanLine(y));
        for (int x = 0; x < newImage.width(); ++x) {
            QRgb &rgb = line[x];
            line[x] = qRgba((int) std::min(255.0f, (float) qRed(rgb) * ratio),
                            (int) std::min(255.0f, (float) qGreen(rgb) * ratio),
                            (int) std::min(255.0f, (float) qBlue(rgb) * ratio),
                            qAlpha(rgb));
        }
    }

    return newImage;
}