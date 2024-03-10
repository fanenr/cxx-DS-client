#ifndef REG_H
#define REG_H

#include "ui_reg.h"
#include "util.h"

class Log;
class QNetworkReply;

class Reg : public QMainWindow
{
  Q_OBJECT
  friend class Log;

private:
  QString user;
  QString pass;
  type typ = type::NONE;
  Ui::Reg *ui = new Ui::Reg;

public:
  Reg (QMainWindow *parent, type typ);
  ~Reg () { delete ui; }

private slots:
  void on_pbtn1_clicked ();
  void on_pbtn2_clicked ();
};

#endif
