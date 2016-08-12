#include <iostream>
#include <iomanip>

#include "odeon.hpp"
#include "ui.hpp"
#include "cli.hpp"

int main(int argc, char **argv)
{
  Odeon db;

  db.AddLocalCinema("Camden");
  db.AddLocalCinema("Greenwich");
  db.AddLocalCinema("London Covent Garden");
  db.AddLocalCinema("London Leicester Square");
  db.AddLocalCinema("London Panton Street");
  db.AddLocalCinema("London Tottenham Court Road");

  std::cout << "Fetching data..." << std::flush;
  db.Fetch();
  std::cout << " done." << std::endl;

  /* UI ui(db); */
  CLI ui(db);

  ui.Run();


  /* for(auto cinema_id : db.GetCinemas()) */
  /* { */
  /*   auto cinema = db.GetCinema(cinema_id); */
  /*   for(auto perf_id : db.GetCinemaPerformances(cinema_id)) */
  /*   { */
  /*     auto perf = db.GetPerformance(perf_id); */
  /*     auto film = db.GetFilm(perf.film); */
  /*     std::cout << cinema.name << " is showing " << film.title << " on "; */
  /*     std::cout << perf.day << "/" << perf.month << "/" << perf.year; */
  /*     std::cout << " at "; */
  /*     std::cout << std::setw(2) << std::setfill('0') << perf.hour; */
  /*     std::cout << ":"; */
  /*     std::cout << std::setw(2) << std::setfill('0') << perf.minute; */
  /*     if(perf.is3D || perf.isIMAX) */
  /*     { */
  /*       std::cout << " in"; */
  /*       if(perf.isIMAX) */
  /*         std::cout << " IMAX"; */
  /*       if(perf.is3D) */
  /*         std::cout << " 3D"; */
  /*     } */
  /*     std::cout << std::endl; */
  /*   } */
  /* } */

  return 0;
}
