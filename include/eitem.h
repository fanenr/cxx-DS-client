#ifndef EITEM_H
#define EITEM_H

#include "ui_eitem.h"

struct Eval
{
  int64_t id;
  double grade;
  QString user;
  QString uname;
  QString evaluation;
};

Q_DECLARE_METATYPE (Eval);

class Eitem : public QWidget
{
  Q_OBJECT

  friend class Home;

private:
  Ui::Eitem ui = {};

public:
  Eitem (QWidget *parent, Eval const &info);

public:
  void set_info (Eval const &info);
};

#endif
