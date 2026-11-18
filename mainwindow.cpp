#include "mainwindow.h"
#include <QFileDialog>
#include <QImageReader>
#include <QMessageBox>
#include "./ui_mainwindow.h"
#include "ImageEffectUtil.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->load_image_button, &QPushButton::clicked, this, &MainWindow::loadImage);
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

MainWindow::~MainWindow()
{
    delete ui;
}
