#include <iostream>
#include <iomanip>

#include "odeon.hpp"
#include "cli.hpp"

int main()
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

  CLI ui(db);

  ui.Run();

  return 0;
}
