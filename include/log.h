#ifndef MAIN_H
#define MAIN_H

#include "ui_log.h"
#include "util.h"

#include <QButtonGroup>

class Reg;

class Log : public QMainWindow
{
  Q_OBJECT

  friend class Reg;

private:
  Ui::Log ui = {};
  QButtonGroup btns;
  Reg *page_reg = nullptr;

public:
  Log ();

public:
  type category ();

private slots:
  void on_pbtn1_clicked ();
  void on_pbtn2_clicked ();
};

#endif
