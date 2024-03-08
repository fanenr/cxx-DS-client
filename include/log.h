#ifndef MAIN_H
#define MAIN_H

#include "ui_log.h"

class Reg;
class QNetworkReply;

class Log : public QMainWindow
{
  Q_OBJECT

private:
  Reg *sreg = nullptr;
  Reg *mreg = nullptr;
  Ui::Log *ui = new Ui::Log;
  QButtonGroup *btns = nullptr;

public:
  explicit Log (QWidget *parent = nullptr);
  ~Log () { delete ui; }

private:
  int category ();
  void req_finished (QNetworkReply *reply);

private slots:
  void on_pbtn1_clicked ();
  void on_pbtn2_clicked ();
};

#endif
