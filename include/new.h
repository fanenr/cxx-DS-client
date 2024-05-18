#ifndef NEW_H
#define NEW_H

#include "ui_new.h"
#include "util.h"

class Home;

class New : public QDialog
{
  Q_OBJECT

private:
  oper op;
  int64_t id;
  Home *parent;
  Ui::New ui = {};

public:
  New (Home *parent);

public:
  int exec (oper op);

private slots:
  void on_pbtn1_clicked ();
  void on_pbtn2_clicked ();
  void on_pbtn3_clicked ();
};

#endif
