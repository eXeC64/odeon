#pragma once

#include <map>
#include <set>
#include <string>

namespace Json
{
  class Value;
}

struct Cinema
{
  std::string name = "Invalid Cinema";
};

struct Film
{
  std::string title = "Invalid Film";
  std::string certificate = "?";
  int duration = 0;
};

struct Performance
{
  int film = -1;
  int cinema = -1;
  int year = 1970;
  int month = 1;
  int day = 1;
  int hour = 0;
  int minute = 1;
  bool is3D = false;
  bool isIMAX = false;
  bool isSubtitled = false;
};

class APIWrapper
{
public:
  void AddLocalCinema(const std::string& name);
  void ClearLocalCinemas();

  const Cinema& GetCinema(int cinema_id) const;
  const Film& GetFilm(int film_id) const;
  const Performance& GetPerformance(const std::string& id) const;

  std::set<int> GetCinemas() const;
  std::set<int> GetFilms() const;

  std::set<std::string> GetCinemaPerformances(int cinema_id) const;
  std::set<std::string> GetFilmPerformances(int film_id) const;

  std::set<int> GrepFilms(const std::string& str) const;
  std::set<std::string> GetAllPerformances() const;

  bool Fetch(); //Fetch latest data

private:

  bool FetchCinemas();
  bool FetchPerformances(int cinema_id);
  bool AddFilm(const Json::Value &film);

  std::set<std::string> m_local_cinemas;
  std::map<int,Cinema> m_cinemas;
  std::map<int,Film> m_films;
  std::map<std::string,Performance> m_performances;

  //Indexes of performance ids by cinema/film
  std::map<int, std::set<std::string>> m_performances_by_cinema;
  std::map<int, std::set<std::string>> m_performances_by_film;

  Film m_null_film;
  Cinema m_null_cinema;
  Performance m_null_performance;
};
