#include "apiwrapper.hpp"

#include <iostream>
#include <cpr/cpr.h>
#include <json/json.h>
#include <boost/algorithm/string/predicate.hpp>


void APIWrapper::AddLocalCinema(const std::string& name)
{
  m_local_cinemas.insert(name);
}

void APIWrapper::ClearLocalCinemas()
{
  m_local_cinemas.clear();
}

bool APIWrapper::Fetch()
{
  m_cinemas.clear();
  m_films.clear();
  m_performances.clear();
  m_performances_by_cinema.clear();
  m_performances_by_film.clear();

  //First fetch list of cinemas
  if(!FetchCinemas())
    return false;

  //Next fetch performances
  for(auto it : m_cinemas)
  {
    FetchPerformances(it.first);
  }
  
  return true;
}

bool APIWrapper::FetchCinemas()
{
  auto r = cpr::Get(cpr::Url{"http://www.odeon.co.uk/api/uk/v2/cinemas.json?lite=1"});
  if(r.status_code != 200)
    return false;

  Json::Value cinemas;
  Json::Reader reader;

  if(!reader.parse(r.text.c_str(), cinemas))
    return false;

  for(auto cinema : cinemas)
  {
    //If we have local cinemas configured, filter to just these
    if(!m_local_cinemas.empty() && m_local_cinemas.find(cinema["name"].asString()) == m_local_cinemas.end())
      continue;

    const int id = cinema["id"].asInt();

    m_cinemas[id].name = cinema["name"].asString();
  }

  return true;
}

bool APIWrapper::FetchPerformances(int cinema_id)
{
  std::stringstream url_ss;
  url_ss << "http://www.odeon.co.uk/api/uk/v2/performances/cinema/" << cinema_id << ".json";

  auto r = cpr::Get(cpr::Url{url_ss.str()});
  if(r.status_code != 200)
    return false;

  Json::Value films;
  Json::Reader reader;

  if(!reader.parse(r.text.c_str(), films))
    return false;

  for(auto film : films)
  {
    const int film_id = film["film"]["masterId"].asInt();

    if(m_films.find(film_id) == m_films.end())
      AddFilm(film["film"]);

    for(auto performance : film["performances"])
    {
      const std::string perf_id  = performance["id"].asString();
      const std::string date_str = performance["date"].asString();
      const std::string time_str = performance["showtime"].asString();

      Performance p;
      p.film = film_id;
      p.cinema = cinema_id;

      //YYYY-MM-DD
      p.year  = std::stoi(date_str.substr(0, 4));
      p.month = std::stoi(date_str.substr(5, 2));
      p.day   = std::stoi(date_str.substr(8, 2));

      //HH:MM:SS
      p.hour   = std::stoi(time_str.substr(0, 2));
      p.minute = std::stoi(time_str.substr(3, 2));

      p.is3D        = performance["filmAttributes"]["is3D"].asBool();
      p.isIMAX      = performance["filmAttributes"]["isImax"].asBool();
      p.isSubtitled = performance["filmAttributes"]["isSubtitledForHardOfHearing"].asBool();

      m_performances[perf_id] = p;
      m_performances_by_cinema[cinema_id].insert(perf_id);
      m_performances_by_film[film_id].insert(perf_id);
    }
  }

  return true;
}

bool APIWrapper::AddFilm(const Json::Value &film)
{
  const int id = film["masterId"].asInt();

  Film f;
  f.title = film["title"].asString();
  f.certificate = film["certificate"].asString();
  f.duration = std::stoi(film["runningTime"].asString());

  m_films[id] = f;
  return true;
}

const Cinema& APIWrapper::GetCinema(int cinema_id) const
{
  auto it = m_cinemas.find(cinema_id);
  if(it != m_cinemas.end())
    return it->second;

  return m_null_cinema;
}

const Film& APIWrapper::GetFilm(int film_id) const
{
  auto it = m_films.find(film_id);
  if(it != m_films.end())
    return it->second;

  return m_null_film;
}

const Performance& APIWrapper::GetPerformance(const std::string& performance_id) const
{
  auto it = m_performances.find(performance_id);
  if(it != m_performances.end())
    return it->second;

  return m_null_performance;
}

std::set<int> APIWrapper::GetCinemas() const
{
  std::set<int> ret;
  for(auto it : m_cinemas)
    ret.insert(it.first);
  return ret;
}

std::set<int> APIWrapper::GetFilms() const
{
  std::set<int> ret;
  for(auto it : m_films)
    ret.insert(it.first);
  return ret;
}

std::set<std::string> APIWrapper::GetCinemaPerformances(int cinema_id) const
{
  auto it = m_performances_by_cinema.find(cinema_id);
  if(it != m_performances_by_cinema.end())
    return it->second;

  return std::set<std::string>();
}

std::set<std::string> APIWrapper::GetFilmPerformances(int film_id) const
{
  auto it = m_performances_by_film.find(film_id);
  if(it != m_performances_by_film.end())
    return it->second;

  return std::set<std::string>();
}

std::set<int> APIWrapper::GrepFilms(const std::string& str) const
{
  std::set<int> ret;
  for(auto it = m_films.begin(); it != m_films.end(); ++it)
  {
    if(boost::algorithm::icontains(it->second.title, str))
      ret.insert(it->first);
  }

  return ret;
}

std::set<std::string> APIWrapper::GetAllPerformances() const
{
  std::set<std::string> perfs;
  for(auto p : m_performances)
    perfs.insert(p.first);
  return perfs;
}
