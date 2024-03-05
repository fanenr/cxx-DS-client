#include "main.h"

int
main (int argc, char *argv[])
{
  QApplication app (argc, argv);
  Main main;
  main.show ();
  return app.exec ();
}
