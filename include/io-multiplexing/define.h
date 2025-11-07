#pragma once

#include <string>
#include <functional>

using Buffer = std::string;

using ProcessRequestFunction = std::function<Buffer(const Buffer&)>;
using ProcessResponseFunction = std::function<Buffer(const Buffer&)> ;

