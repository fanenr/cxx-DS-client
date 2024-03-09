#include "log.h"

int
main (int argc, char *argv[])
{
  QApplication app (argc, argv);
  auto log = new Log (nullptr);
  log->show ();
  return app.exec ();
}
