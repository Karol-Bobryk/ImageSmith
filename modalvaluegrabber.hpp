#ifndef MODALVALUEGRABBER_H
#define MODALVALUEGRABBER_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QRgb>
#include <QRgbaFLoat>
#include <QWidget>
#include <QtGui/qrgb.h>
#include "userinpututil.h"

class ModalValueGrabber : public QDialog
{
    Q_OBJECT
public:
    explicit ModalValueGrabber(QWidget *parent = nullptr, size_t N = 1)
        : QDialog(parent)
        , N{N}
        , labels({})
        , edits{(qsizetype) N}
        , form_layout{new QFormLayout(this)}
    {}

    explicit ModalValueGrabber(QWidget *parent = nullptr,
                               size_t N = 1,
                               std::initializer_list<std::string_view> labels = {})
        : QDialog(parent)
        , N{N}
        , edits{(qsizetype) N}
        , form_layout{new QFormLayout(this)}
    {
        addLabels(labels);
        initializeLayout();
    }

    void addLabels(std::initializer_list<std::string_view> labels)
    {
        if (labels.size() != N)
            throw std::runtime_error("The amount of labels does not match the number dialog boxes");
        this->labels = labels;
    }

    void initializeLayout()
    {
        if (labels.empty())
            throw std::runtime_error(
                "Cannot initialize layout without specifying the labels first");

        for (int i = 0; i < N; ++i) {
            edits[i] = new QLineEdit(this);
            form_layout->addRow(QString::fromLatin1(labels[i].data(), labels[i].size()), edits[i]);
        }

        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                               | QDialogButtonBox::Cancel,
                                                           Qt::Horizontal,
                                                           this);
        form_layout->addWidget(buttonBox);

        bool conn = connect(buttonBox,
                            &QDialogButtonBox::accepted,
                            this,
                            &ModalValueGrabber::accept);
        Q_ASSERT(conn);

        conn = connect(buttonBox, &QDialogButtonBox::rejected, this, &ModalValueGrabber::reject);

        Q_ASSERT(conn);
    }

    [[nodiscard]] QList<QString> execModal()
    {
        if (labels.size() != edits.size() || labels.size() != N)
            throw std::runtime_error("Modal initialization was not performed correctly");

        const int ret = exec();

        QList<QString> return_values(N);

        if (ret == QDialog::Rejected)
            return return_values;

        for (int i = 0; i < N; ++i) {
            return_values[i] = edits[i]->text();
        }

        return return_values;
    }

    [[nodiscard]] static QRgb getRgbaFromUser(QWidget *parent)
    {
        ModalValueGrabber modal(parent, 4, {"r", "g", "b", "a"});
        QList user_vals = modal.execModal();

        float d_red = UserInputUtil::deserialize(user_vals[0], InputType::RGBA_UNIT).value_or(0);
        float d_green = UserInputUtil::deserialize(user_vals[1], InputType::RGBA_UNIT).value_or(0);
        float d_blue = UserInputUtil::deserialize(user_vals[2], InputType::RGBA_UNIT).value_or(0);
        float d_alpha = UserInputUtil::deserialize(user_vals[3], InputType::RGBA_UNIT).value_or(0);

        return qRgba(d_red, d_green, d_blue, d_alpha);
    }

    [[nodiscard]] static QRgbaFloat<float> getRgbaFloatFromUser(QWidget *parent)
    {
        ModalValueGrabber modal(parent, 4, {"r", "g", "b", "a"});
        QList user_vals = modal.execModal();

        float d_red = UserInputUtil::deserialize(user_vals[0], InputType::FLOAT).value_or(0);
        float d_green = UserInputUtil::deserialize(user_vals[1], InputType::FLOAT).value_or(0);
        float d_blue = UserInputUtil::deserialize(user_vals[2], InputType::FLOAT).value_or(0);
        float d_alpha = UserInputUtil::deserialize(user_vals[3], InputType::FLOAT).value_or(0);

        QRgbaFloat<float> ret;

        ret.setRed(d_red);
        ret.setGreen(d_green);
        ret.setBlue(d_blue);
        ret.setAlpha(d_alpha);

        return ret;
    }

private:
    size_t N;
    std::vector<std::string_view> labels = {};
    QList<QLineEdit *> edits;
    QFormLayout *form_layout;
signals:
};

#endif // MODALVALUEGRABBER_H
