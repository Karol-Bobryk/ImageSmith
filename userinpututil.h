#ifndef USERINPUTUTIL_H
#define USERINPUTUTIL_H

#include <QString>

namespace UserInputUtil {

enum class InputType { RGBA_UNIT, PERCENT, FLOAT };

std::optional<float> deserialize(QString input, InputType type = InputType::FLOAT);
float clamp(float value, InputType type = InputType::FLOAT);

}; // namespace UserInputUtil

using InputType = UserInputUtil::InputType;
#endif // USERINPUTUTIL_H
