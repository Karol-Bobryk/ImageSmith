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

    const size_t mask_height = mask.size();
    const size_t mask_width = mask[0].size();

    const int height_offset = mask.size() / 2;
    const int width_offset = mask[0].size() / 2;

    // middle
    for (int y = height_offset; y < oldImage.height() - height_offset; ++y) {
        std::vector<QRgb *> scanlines = getScanlineRange(oldImage,
                                                         y - height_offset,
                                                         y + height_offset);

        QRgb *new_line = reinterpret_cast<QRgb *>(newImage.scanLine(y));

        convoluteScanline(scanlines, mask, new_line, oldImage.width(), width_offset);
    }

    const size_t fake_scanline_size = std::max(oldImage.width(), oldImage.height());
    constexpr unsigned int fake_scanline_default_value = 0x00000000;
    std::vector<QRgb> fake_scanline(fake_scanline_size, fake_scanline_default_value);

    // top border no corners
    for (int y = 0; y < height_offset; ++y) {
        std::vector<QRgb *> scanlines = getScanlineRange(oldImage,
                                                         y - height_offset,
                                                         y + height_offset);

        QRgb *new_line = reinterpret_cast<QRgb *>(newImage.scanLine(y));

        convoluteScanline(scanlines, mask, new_line, oldImage.width(), width_offset);
    }

    // bottom border no corners
    for (int y = oldImage.height() - height_offset; y < oldImage.height(); ++y) {
        std::vector<QRgb *> scanlines = getScanlineRange(oldImage,
                                                         y - height_offset,
                                                         y + height_offset);

        QRgb *new_line = reinterpret_cast<QRgb *>(newImage.scanLine(y));

        convoluteScanline(scanlines, mask, new_line, oldImage.width(), width_offset);
    }

    // side border with corners
    for (int y = 0; y < oldImage.height(); ++y) {
        QRgb *new_line = reinterpret_cast<QRgb *>(newImage.scanLine(y));
        // left
        for (int x = 0; x < width_offset; ++x) {
            std::vector<std::vector<QRgb>> matrix = createRgbMatrixFromImage(oldImage,
                                                                             x,
                                                                             y,
                                                                             mask_width,
                                                                             mask_height);

            auto [red_conv, green_conv, blue_conv] = ConvolutionUtil::convolutionByValue(matrix,
                                                                                         mask);

            new_line[x] = qRgb((int) red_conv, (int) green_conv, (int) blue_conv);
        }

        // right
        for (int x = oldImage.width() - width_offset; x < oldImage.width(); ++x) {
            std::vector<std::vector<QRgb>> matrix = createRgbMatrixFromImage(oldImage,
                                                                             x,
                                                                             y,
                                                                             mask_width,
                                                                             mask_height);

            auto [red_conv, green_conv, blue_conv] = ConvolutionUtil::convolutionByValue(matrix,
                                                                                         mask);

            new_line[x] = qRgb((int) red_conv, (int) green_conv, (int) blue_conv);
        }
    }

    return newImage;
}

void ImageEffectProcessor::convoluteScanline(std::vector<QRgb *> scanlines,
                                             std::vector<std::vector<float>> mask,
                                             QRgb *out,
                                             size_t len,
                                             size_t x_offset)
{
    assert(x_offset < len);

    const size_t mask_height = mask.size();
    const size_t mask_width = mask[0].size();

    std::vector<QRgb *> matrix_start(mask_height);

    for (int x = x_offset; x < len - x_offset; ++x) {
        for (int h = 0; h < mask_height; ++h)
            matrix_start[h] = scanlines[h] + x - x_offset;

        QRgb &rgb = out[x];
        auto [red_conv, green_conv, blue_conv] = ConvolutionUtil::convolution(matrix_start,
                                                                              mask,
                                                                              mask_height,
                                                                              mask_width);

        rgb = qRgb((int) red_conv, (int) green_conv, (int) blue_conv);
    }
}

std::vector<QRgb *> ImageEffectProcessor::getScanlineRange(QImage image,
                                                           int range_start,
                                                           int range_end,
                                                           QRgb *fake_scanline)
{
    assert(range_end > range_start);

    size_t ret_size = range_end - range_start + 1;
    std::vector<QRgb *> ret(ret_size, nullptr);

    for (size_t i = 0; i < ret_size; ++i)
        if (i < 0 || i > image.height())
            ret[i] = fake_scanline;
        else
            ret[i] = reinterpret_cast<QRgb *>(image.scanLine(range_start + i));

    return ret;
}

std::vector<std::vector<QRgb>> ImageEffectProcessor::createRgbMatrixFromImage(
    QImage image, int center_x, int center_y, size_t width, size_t height, QRgb default_value)
{
    using Matrix = std::vector<std::vector<QRgb>>;

    int x_offset = width / 2;
    int y_offset = height / 2;

    Matrix matrix(height, std::vector<QRgb>(width, default_value));

    std::vector<QRgb *> scanlines = getScanlineRange(image,
                                                     (int) center_y - y_offset,
                                                     (int) center_y + y_offset);

    for (int y = center_y - y_offset, m_y = 0; y <= center_y + y_offset; ++y, ++m_y)
        for (int x = center_x - x_offset, m_x = 0; x <= center_x + x_offset; ++x, ++m_x)
            if (y < 0 || x < 0 || y >= image.height() || x >= image.width())
                continue;
            else
                matrix[m_y][m_x] = reinterpret_cast<const QRgb *>(image.constScanLine(y))[x];

    return matrix;
}