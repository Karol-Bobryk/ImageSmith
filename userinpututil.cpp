#include "userinpututil.h"
#include <QDebug>

std::optional<float> UserInputUtil::deserialize(QString input, InputType type)
{
    bool ok;
    float ret = input.toFloat(&ok);

    if (!ok) {
        qInfo() << "invalid user input detected";
        return std::nullopt;
    }
    return clamp(ret, type);
}
float UserInputUtil::clamp(float value, InputType type)
{
    float lo = 0, hi = 0;

    switch (type) {
    case InputType::RGBA_UNIT: {
        lo = 0;
        hi = 255;
        break;
    }
    case InputType::PERCENT: {
        lo = 0;
        hi = 100;
        break;
    }

    case InputType::FLOAT:
        /* FALLTHROUGH */
    default:
        return value;
    }

    return std::clamp(value, lo, hi);
}