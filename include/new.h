#ifndef NEW_H
#define NEW_H

#include "ui_new.h"

enum class oper
{
  NEW,
  MOD,
};

class Home;

class New : public QMainWindow
{
  Q_OBJECT

private:
  oper op;
  Home *prnt;
  Ui::New *ui = new Ui::New;

public:
  New (Home *parent, oper op);
  ~New () { delete ui; }

private slots:
  void on_pbtn1_clicked ();
  void on_pbtn2_clicked ();
  void on_pbtn3_clicked ();
};

#endif
