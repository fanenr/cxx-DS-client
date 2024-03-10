#ifndef MAIN_H
#define MAIN_H

#include "ui_log.h"
#include "util.h"

class Reg;
class QNetworkReply;

class Log : public QMainWindow
{
  Q_OBJECT

private:
  Ui::Log *ui = new Ui::Log;
  QButtonGroup *btns = nullptr;

public:
  explicit Log (QMainWindow *parent);
  ~Log () { delete ui; }

private:
  type category ();

private slots:
  void on_pbtn1_clicked ();
  void on_pbtn2_clicked ();
};

#endif
