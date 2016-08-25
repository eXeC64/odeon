#pragma once

#include <chrono>
#include <string>
#include <vector>


#include "apiwrapper.hpp"

class CLI
{
public:
  CLI(APIWrapper &api);
  void Run();
  void PrintFilmPerformances(int film_id);
  void PrintPerformancesOnDate(std::chrono::time_point<std::chrono::system_clock> date);
  void PrintPerformances(std::vector<Performance> performances, bool show_dates = true);

private:
  void HandleCommand(const std::string& command, const std::vector<std::string> &args);

  APIWrapper &m_api;
};
