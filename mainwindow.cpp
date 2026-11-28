#include "mainwindow.h"
#include <QFileDialog>
#include <QImageReader>
#include <QMessageBox>
#include <QRgb>
#include <QRgbaFloat>
#include "./ui_mainwindow.h"
#include "ImageEffectUtil.h"
#include "imageeffectprocessor.h"
#include "modalvaluegrabber.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->load_image_button, &QPushButton::clicked, this, &MainWindow::loadImage);
    connect(ui->smith_button, &QPushButton::clicked, this, &MainWindow::applyEffect);
}

void MainWindow::setupComboBox()
{
    ui->comboBox->setItemText(ImageEffectUtil::getEffectId(ImageEffect::ADD_VALUE), //
                              "Add value");
    ui->comboBox->setItemText(ImageEffectUtil::getEffectId(ImageEffect::SUBSTRACT_VALUE),
                              "Substract value");
    ui->comboBox->setItemText(ImageEffectUtil::getEffectId(ImageEffect::MULTIPLY_VALUE),
                              "Multiply value");
    ui->comboBox->setItemText(ImageEffectUtil::getEffectId(ImageEffect::DIVIDE_VALUE),
                              "Divide value");
    ui->comboBox->setItemText(ImageEffectUtil::getEffectId(ImageEffect::CHANGE_BRIGHTNESS),
                              "Change brightness");
    ui->comboBox->setItemText(ImageEffectUtil::getEffectId(ImageEffect::CONVERT_TO_GRAYSCALE_1),
                              "Convert to grayscale 1");
    ui->comboBox->setItemText(ImageEffectUtil::getEffectId(ImageEffect::CONVERT_TO_GRAYSCALE_2),
                              "Convert to grayscale 2");

    ui->comboBox->setItemText(ImageEffectUtil::getEffectId(ImageEffect::SMOOTHING_FILTER),
                              "Smoothing filter");
    ui->comboBox->setItemText(ImageEffectUtil::getEffectId(ImageEffect::MEDIAN_FILTER),
                              "Median filter");
    ui->comboBox->setItemText(ImageEffectUtil::getEffectId(ImageEffect::EDGE_DETECT),
                              "Edge detect (sobel)");
    ui->comboBox->setItemText(ImageEffectUtil::getEffectId(ImageEffect::HIGH_PASS_FILTER),
                              "High pass filter");
    ui->comboBox->setItemText(ImageEffectUtil::getEffectId(ImageEffect::GAUSSIAN_BLUR),
                              "Gaussian blur");
    ui->comboBox->setItemText(ImageEffectUtil::getEffectId(ImageEffect::CONVOLUTION), //
                              "Convolution");
}

void MainWindow::loadImage(bool /* unused */)
{
    QUrl file_url = QFileDialog::getOpenFileUrl(this,
                                                "Pick an image to load into the program",
                                                QUrl(),
                                                "Images (*.png *.xpm *.jpg)");

    if (file_url.isEmpty()) {
        return;
    }

    if (!image.load(file_url.toLocalFile())) {
        QMessageBox::warning(this, "Error", "Failed to get the image file");
        return;
    }

    ui->image_label->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::applyEffect(bool /* unused */)
{
    // TODO: check for image existing
    if (image.isNull()) {
        QMessageBox::information(this, "Failed to apply a filter", "You have to load an image");
        return;
    }
    ImageEffect effect = ImageEffectUtil::getEffectById(ui->comboBox->currentIndex());

    switch (effect) {
    case ImageEffect::ADD_VALUE: {
        QRgb d_rgba = ModalValueGrabber::getRgbaFromUser(this);

        image = ImageEffectProcessor::applyAdditionEffect(image, d_rgba);

        ui->image_label->setPixmap(QPixmap::fromImage(image));
        break;
    }
    case ImageEffect::SUBSTRACT_VALUE: {
        QRgb d_rgba = ModalValueGrabber::getRgbaFromUser(this);

        image = ImageEffectProcessor::applySubstractionEffect(image, d_rgba);

        ui->image_label->setPixmap(QPixmap::fromImage(image));
        break;
    }
    case ImageEffect::MULTIPLY_VALUE: {
        QRgbaFloat<float> d_rgba = ModalValueGrabber::getRgbaFloatFromUser(this);

        image = ImageEffectProcessor::applyMultiplicationEffect(image, d_rgba);

        ui->image_label->setPixmap(QPixmap::fromImage(image));
        break;
    }

    case ImageEffect::DIVIDE_VALUE: {
        QRgbaFloat<float> d_rgba = ModalValueGrabber::getRgbaFloatFromUser(this);

        float red = d_rgba.red() == 0.0 ? 1 : d_rgba.red();
        float green = d_rgba.green() == 0.0 ? 1 : d_rgba.green();
        float blue = d_rgba.blue() == 0.0 ? 1 : d_rgba.blue();
        float alpha = d_rgba.alpha() == 0.0 ? 1 : d_rgba.alpha();

        d_rgba.setRed(red);
        d_rgba.setGreen(green);
        d_rgba.setBlue(blue);
        d_rgba.setAlpha(alpha);

        image = ImageEffectProcessor::applyDivideEffect(image, d_rgba);

        ui->image_label->setPixmap(QPixmap::fromImage(image));
        break;
    }

    case ImageEffect::CHANGE_BRIGHTNESS: {
        float ratio = ModalValueGrabber::getFloatFromUser(this);

        image = ImageEffectProcessor::applyChangeBrightness(image, ratio);

        ui->image_label->setPixmap(QPixmap::fromImage(image));
        break;
    }
    case ImageEffect::CONVERT_TO_GRAYSCALE_1: {
        image = ImageEffectProcessor::applyGrayscaleConversionColorometric(image);

        ui->image_label->setPixmap(QPixmap::fromImage(image));

        break;
    }

    case ImageEffect::CONVERT_TO_GRAYSCALE_2: {
        image = ImageEffectProcessor::applyGrayscaleConversionLinear(image);

        ui->image_label->setPixmap(QPixmap::fromImage(image));

        break;
    }
    case ImageEffect::SMOOTHING_FILTER: {
        std::vector<std::vector<float>> mask = {{1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f},
                                                {1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f},
                                                {1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f}};
        image
            = ImageEffectProcessor::applyConvolution(image,
                                                     mask,
                                                     ImageEffectProcessor::transformSmoothingFilter);

        ui->image_label->setPixmap(QPixmap::fromImage(image));

        break;
    }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
