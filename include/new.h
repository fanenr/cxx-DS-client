#ifndef NEW_H
#define NEW_H

#include "ui_new.h"
#include "util.h"

class Home;

class New : public QMainWindow
{
  Q_OBJECT

private:
  oper op;
  int64_t id;
  Home *prnt;
  Ui::New ui = {};

public:
  New (Home *parent);

public:
  void show (oper op);

private slots:
  void on_pbtn1_clicked ();
  void on_pbtn2_clicked ();
  void on_pbtn3_clicked ();
};

#endif
