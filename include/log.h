#ifndef MAIN_H
#define MAIN_H

#include "ui_log.h"
#include "util.h"

#include <QButtonGroup>

class Home;

class Log : public QMainWindow
{
  Q_OBJECT
  friend class Reg;

private:
  Home *prnt;
  QButtonGroup btns;
  Ui::Log *ui = new Ui::Log;

public:
  explicit Log (Home *parent);
  ~Log () { delete ui; }

private:
  type category ();

private slots:
  void on_pbtn1_clicked ();
  void on_pbtn2_clicked ();
};

#endif
