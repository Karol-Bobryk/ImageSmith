#include "imageeffectprocessor.h"
#include "convolutionutil.h"
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

QImage ImageEffectProcessor::applyGrayscaleConversionColorometric(QImage oldImage)
{
    QImage newImageGrayscale(oldImage.width(), oldImage.height(), QImage::Format::Format_Grayscale8);

    for (int y = 0; y < oldImage.height(); ++y) {
        const QRgb *line = reinterpret_cast<const QRgb *>(oldImage.scanLine(y));
        quint8 *new_line = reinterpret_cast<quint8 *>(newImageGrayscale.scanLine(y));

        for (int x = 0; x < oldImage.width(); ++x) {
            QRgb const &rgb = line[x];
            quint8 &gray = new_line[x];

            gray = (quint8) std::min(255,
                                     (int) (0.2126f * (float) qRed(rgb)
                                            + 0.7152f * (float) qGreen(rgb)
                                            + 0.0722f * (float) qBlue(rgb)));
        }
    }

    return newImageGrayscale;
}

QImage ImageEffectProcessor::applyGrayscaleConversionLinear(QImage oldImage)
{
    QImage newImageGrayscale(oldImage.width(), oldImage.height(), QImage::Format::Format_Grayscale8);

    for (int y = 0; y < oldImage.height(); ++y) {
        const QRgb *line = reinterpret_cast<const QRgb *>(oldImage.scanLine(y));
        quint8 *new_line = reinterpret_cast<quint8 *>(newImageGrayscale.scanLine(y));

        for (int x = 0; x < oldImage.width(); ++x) {
            QRgb const &rgb = line[x];
            quint8 &gray = new_line[x];

            gray = (quint8) std::min(255,
                                     (int) ((0.33f) * (float) qRed(rgb)
                                            + (0.33f) * (float) qGreen(rgb)
                                            + (0.33f) * (float) qBlue(rgb)));
        }
    }

    return newImageGrayscale;
}

QImage ImageEffectProcessor::applySmoothingFilter(QImage oldImage)
{
    QImage newImage(oldImage.width(), oldImage.height(), QImage::Format::Format_RGB32);

    std::vector<std::vector<float>> mask = {
        {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0},
        {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0},
        {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0},
    };
    // TODO: add dividing by sum of all elems

    int height_offset = mask.size() / 2;
    int width_offset = mask[0].size() / 2;

    for (int y = height_offset; y < oldImage.height() - height_offset; ++y) {
        std::vector<QRgb *> scanlines(mask.size());
        for (int h = -height_offset; h <= height_offset; ++h)
            scanlines[h + height_offset] = reinterpret_cast<QRgb *>(oldImage.scanLine(y + h));

        QRgb *new_line = reinterpret_cast<QRgb *>(newImage.scanLine(y));

        std::vector<QRgb *> matrix_start(mask.size());

        for (int x = width_offset; x < oldImage.width() - width_offset; ++x) {
            for (int h = -height_offset; h <= height_offset; ++h)
                matrix_start[h + height_offset] = scanlines[h + height_offset] + x - 1;

            QRgb &new_rgb = new_line[x];

            // TODO: add mask convolution
            auto [red_conv,
                  green_conv,
                  blue_conv] = ConvolutionUtil::convolution(matrix_start, mask, 3, 3);

            new_rgb = qRgb(qRed((int) red_conv), qGreen((int) green_conv), qBlue((int) blue_conv));
        }
    }

    return newImage;
}