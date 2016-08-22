#pragma once

#include <string>
#include <vector>


#include "apiwrapper.hpp"

class CLI
{
public:
  CLI(APIWrapper &api);
  void Run();
  void PrintFilmPerformances(int film_id);
  void PrintTodaysPerformances();
  void PrintPerformances(std::vector<Performance> performances, bool show_dates = true);

private:
  void HandleCommand(const std::string& command, const std::vector<std::string> &args);

  APIWrapper &m_api;
};
