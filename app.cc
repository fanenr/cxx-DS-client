#include "log.h"

int
main (int argc, char *argv[])
{
  QApplication app (argc, argv);
  Log log (nullptr);
  log.show ();
  return app.exec ();
}
