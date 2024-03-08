#ifndef REG_H
#define REG_H

#include "ui_reg.h"

class Log;
class QNetworkReply;

class Reg : public QMainWindow
{
  Q_OBJECT
  friend class Log;

private:
  int type;
  QString user;
  QString pass;
  Ui::Reg *ui = new Ui::Reg;

public:
  Reg (QWidget *parent, int category);
  ~Reg () { delete ui; }

private:
  void req_finished (QNetworkReply *reply);

private slots:
  void
  on_pbtn1_clicked ()
  {
    close ();
  }

  void on_pbtn2_clicked ();
};

#endif
