#ifndef MOD_H
#define MOD_H

#include "ui_mod.h"

class Home;

class Mod : public QMainWindow
{
  Q_OBJECT
  friend class Home;

private:
  Home *prnt;
  Ui::Mod *ui = new Ui::Mod;

public:
  Mod (Home *parent);
  ~Mod () { delete ui; }

protected:
  void showEvent (QShowEvent *event) override;

private slots:
  void on_pbtn1_clicked ();
  void on_pbtn2_clicked ();
  void on_pbtn3_clicked ();
};

#endif
