#pragma once

namespace Colors
{
  const std::string Bold = "\033[1m";
  const std::string Underline = "\033[4m";
  namespace Fg
  {
    const std::string Default = "\033[39m";
    const std::string Black = "\033[30m";
    const std::string Red = "\033[31m";
    const std::string Green = "\033[32m";
    const std::string Yellow = "\033[33m";
    const std::string Blue = "\033[34m";
    const std::string Magenta = "\033[35m";
    const std::string Cyan = "\033[36m";
    const std::string White = "\033[37m";
  }
  namespace Bg
  {
    const std::string Default = "\033[49m";
    const std::string Red = "\033[41m";
    const std::string Green = "\033[42m";
    const std::string Yellow = "\033[43m";
    const std::string Blue = "\033[44m";
    const std::string Magenta = "\033[45m";
    const std::string Cyan = "\033[46m";
    const std::string White = "\033[47m";
  }
}
