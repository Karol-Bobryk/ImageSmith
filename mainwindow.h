#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QImage>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void setImage(QImage newImage);
    ~MainWindow() override;

private:
    void setupComboBox();
    Ui::MainWindow *ui;
    QImage image;
private slots:
    void loadImage(bool /* unused */);
};
#endif // MAINWINDOW_H
