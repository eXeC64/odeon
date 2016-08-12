#pragma once

#include <string>
#include <vector>


#include "odeon.hpp"

class CLI
{
public:
  CLI(Odeon &model);
  void Run();
  void PrintFilmPerformances(int film_id);
  void PrintTodaysPerformances();
  void PrintPerformances(const std::vector<Performance>& performances, bool show_dates = true);

private:
  void HandleCommand(const std::string& command, const std::vector<std::string> &args);

  Odeon &m_model;
};
