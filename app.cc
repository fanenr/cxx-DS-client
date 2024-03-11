#include "home.h"

int
main (int argc, char *argv[])
{
  QApplication app (argc, argv);
  Home home;
  return app.exec ();
}
