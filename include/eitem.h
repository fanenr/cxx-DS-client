#ifndef EITEM_H
#define EITEM_H

#include "ui_eitem.h"

struct Eval
{
  qint64 id;
  QString user;
  double grade;
  QString uname;
  QString evaluation;
};

Q_DECLARE_METATYPE (Eval);

class Eitem : public QWidget
{
  Q_OBJECT
  friend class Home;

private:
  Ui::Eitem *ui = new Ui::Eitem;

public:
  Eitem (QWidget *parent, Eval const &info);
  ~Eitem () { delete ui; }

public:
  void set_info (Eval const &info);
};

#endif
