#ifndef MOD_H
#define MOD_H

#include "ui_mod.h"
#include "util.h"

class Home;

class Mod : public QMainWindow
{
  Q_OBJECT
  friend class Home;

private:
  type typ = type::NONE;
  Ui::Mod *ui = new Ui::Mod;
  QMap<QString, QString> &old;

public:
  Mod (QMainWindow *parent, type typ, decltype (old) old);
  ~Mod () { delete ui; }

private:
  void load_old ();

private slots:
  void on_pbtn1_clicked ();
  void on_pbtn2_clicked ();
  void on_pbtn3_clicked ();
};

#endif
