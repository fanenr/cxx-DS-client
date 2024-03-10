#ifndef NEW_H
#define NEW_H

#include "ui_new.h"

enum class opt
{
  NONE,
  NEW,
  MOD
};

class New : QMainWindow
{
  Q_OBJECT
  friend class Home;

private:
  opt op = opt::NONE;
  Ui::New *ui = new Ui::New;
  QMap<QString, QString> &info;

public:
  New (QMainWindow *parent, opt op, decltype (info) info);
  ~New () { delete ui; }

private slots:
  void on_pbtn1_clicked ();
  // void on_pbtn2_clicked ();
  void on_pbtn3_clicked ();
};

#endif
