#include "log.h"

int
main (int argc, char *argv[])
{
  QApplication app (argc, argv);
  Log log;
  log.show ();
  return app.exec ();
}
