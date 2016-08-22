#include "cli.hpp"
#include "colors.hpp"

#include <iostream>
#include <iomanip>
#include <chrono>
#include <sstream>
#include <boost/algorithm/string.hpp>

CLI::CLI(APIWrapper &api)
  : m_api(api)
{
}

void CLI::Run()
{
  while(!std::cin.eof())
  {
    std::string line;
    std::cout << "> " << std::flush;
    std::getline(std::cin, line);

    if(line == "exit")
      break;

    if(line.empty())
      continue;

    std::vector<std::string> parts;
    boost::split(parts, line, boost::is_any_of(" \t"));
    HandleCommand(parts[0], std::vector<std::string>(parts.begin() + 1, parts.end()));
  }
}

void CLI::HandleCommand(const std::string& command, const std::vector<std::string> &args)
{
  if(command == "help")
  {
    std::cout << "Commands:\n"
              << "\t       help: Print this help\n"
              << "\t       list: Film List\n"
              << "\t      today: Today's Performances\n"
              << "\tshow <film>: Show performances of film\n"
              << std::flush;
  }
  else if(command == "list")
  {
    std::set<int> film_ids = m_api.GetFilms();
    std::map<std::string, Film> films;

    for(int id : film_ids)
    {
      const Film &film = m_api.GetFilm(id);
      films[film.title] = film;
    }

    for(auto it : films)
    {
      std::cout << " * " << it.second.title << " [" << it.second.certificate << "] " << it.second.duration << "m" << std::endl;
    }
  }
  else if(command == "show")
  {
    if(args.empty())
    {
      std::cout << "Error: film title not provided" << std::endl;
      return;
    }

    std::string title;
    for(const std::string& s : args)
      title += std::string{" "} + s;

    std::set<int> film_ids = m_api.GrepFilms(title);
    std::map<std::string, Film> films;

    std::map<std::string, int> title_to_id;
    for(int id : film_ids)
    {
      const Film &film = m_api.GetFilm(id);
      films[film.title] = film;
      title_to_id[film.title] = id;
    }

    for(auto it : films)
      PrintFilmPerformances(title_to_id[it.first]);
  }
  else if(command == "today")
  {
    PrintTodaysPerformances();
  }
  else
  {
    std::cout << "Unknown command. For help, type 'help'." << std::endl;
  }
}

bool by_time(const Performance& lhs, const Performance& rhs)
{
  return (lhs.hour * 60 + lhs.minute) < (rhs.hour * 60 + rhs.minute);
}

bool by_cinema(const Performance& lhs, const Performance& rhs)
{
  return lhs.cinema < rhs.cinema;
}

bool by_film(const Performance& lhs, const Performance& rhs)
{
  return lhs.film < rhs.film;
}

bool by_date(const Performance& lhs, const Performance& rhs)
{
  return (lhs.year * 1000 + lhs.month * 50 + lhs.day) < (rhs.year * 1000 + rhs.month * 50 + rhs.day);
}

std::string format_date(int year, int month, int day)
{
  std::stringstream ss;
  ss << year << "/" << month << "/" << day;
  return ss.str();
}

void CLI::PrintFilmPerformances(int film_id)
{

  std::set<std::string> performance_ids = m_api.GetFilmPerformances(film_id);
  std::vector<Performance> performances;
  for(auto id : performance_ids)
    performances.push_back(m_api.GetPerformance(id));

  PrintPerformances(performances);
}

void CLI::PrintTodaysPerformances()
{

  std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
  time_t tt = std::chrono::system_clock::to_time_t(now);
  tm time = *localtime(&tt);

  std::set<std::string> performance_ids = m_api.GetAllPerformances();
  std::vector<Performance> performances;
  for(auto id : performance_ids)
  {
    const Performance& p = m_api.GetPerformance(id);
    if(p.year != time.tm_year + 1900 || p.month != time.tm_mon + 1 || p.day != time.tm_mday)
      continue;
    performances.push_back(p);
  }

  PrintPerformances(performances, false);
}

void CLI::PrintPerformances(std::vector<Performance> performances, bool show_dates)
{
  //Sort by time
  std::sort(performances.begin(), performances.end(), by_time);

  //Stable sort by cinema
  std::stable_sort(performances.begin(), performances.end(), by_cinema);

  //Stable sort by date
  std::stable_sort(performances.begin(), performances.end(), by_date);

  //Stable sort by film
  std::stable_sort(performances.begin(), performances.end(), by_film);


  std::string last_film;
  std::string last_date;
  std::string last_cinema;
  for(const Performance& p : performances)
  {
    const Film& film = m_api.GetFilm(p.film);
    const Cinema& cinema = m_api.GetCinema(p.cinema);
    const std::string cur_film = film.title;
    const std::string cur_date = format_date(p.year, p.month, p.day);
    const std::string cur_cinema = cinema.name;

    if(cur_film != last_film)
    {
      last_film = cur_film;
      last_date = "";
      std::cout << "\n\n" << Colors::Fg::White << film.title << ":" << Colors::Fg::Default;
    }

    if(cur_date != last_date)
    {
      last_date = cur_date;
      last_cinema = "";
      if(show_dates)
        std::cout << "\n" << Colors::Fg::Yellow << cur_date << ":" << Colors::Fg::Default;
    }
    if(cur_cinema != last_cinema)
    {
      last_cinema = cur_cinema;
      std::cout << "\n" << std::setw(30) << cur_cinema << ":";
    }

    const std::string left = (p.is3D ? Colors::Fg::White : Colors::Fg::Blue) + (p.isIMAX ? " <" : " [");
    const std::string right = (p.isIMAX ? ">" : "]") + Colors::Fg::Default;
    std::cout << std::setfill('0');
    std::cout << left << std::setw(2) << p.hour << ":" << std::setw(2) << p.minute << right << std::flush;
    std::cout << std::setfill(' ');
  }
  std::cout << std::endl;
}
