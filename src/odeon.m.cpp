#include <iostream>
#include <iomanip>

#include "apiwrapper.hpp"
#include "cli.hpp"

int main()
{
  APIWrapper api;

  api.AddLocalCinema("Camden");
  api.AddLocalCinema("Greenwich");
  api.AddLocalCinema("London Covent Garden");
  api.AddLocalCinema("London Leicester Square");
  api.AddLocalCinema("London Panton Street");
  api.AddLocalCinema("London Tottenham Court Road");

  std::cout << "Fetching data..." << std::flush;
  api.Fetch();
  std::cout << " done." << std::endl;

  CLI ui(api);

  ui.Run();

  return 0;
}
