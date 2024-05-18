#ifndef EVA_H
#define EVA_H

#include "ui_eva.h"
#include "util.h"

class Home;

class Eva : public QDialog
{
  Q_OBJECT

private:
  oper op;
  int64_t id;
  Home *parent;
  Ui::Eva ui = {};

public:
  Eva (Home *parent);

public:
  int exec (oper op);

private slots:
  void on_pbtn1_clicked ();
  void on_pbtn2_clicked ();
  void on_pbtn3_clicked ();
};

#endif