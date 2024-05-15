#include "log.h"

int
main (int argc, char **argv)
{
  auto app = QApplication (argc, argv);
  auto log = Log ();
  log.show ();
  app.exec ();
}
