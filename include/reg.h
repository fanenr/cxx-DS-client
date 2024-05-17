#ifndef REG_H
#define REG_H

#include "ui_reg.h"

class Log;

class Reg : public QMainWindow
{
  Q_OBJECT

private:
  Log *prnt;
  Ui::Reg ui = {};

public:
  Reg (Log *parent);

public:
  void show ();

private slots:
  void on_pbtn1_clicked ();
  void on_pbtn2_clicked ();
};

#endif
