#ifndef EVA_H
#define EVA_H

#include "ui_eva.h"
#include "util.h"

class Home;

class Eva : public QMainWindow
{
  Q_OBJECT

private:
  oper op;
  qint64 id;
  Home *prnt;
  Ui::Eva *ui = new Ui::Eva;

public:
  Eva (Home *parent);
  ~Eva () { delete ui; }

public:
  void show (oper op);

private slots:
  void on_pbtn1_clicked ();
  void on_pbtn2_clicked ();
  void on_pbtn3_clicked ();
};

#endif