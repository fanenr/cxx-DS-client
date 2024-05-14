#ifndef DITEM_H
#define DITEM_H

#include "ui_ditem.h"

struct Dish
{
  int64_t id;
  double price;
  QString name;
  QString user;
  QString uname;
  QString position;
};

Q_DECLARE_METATYPE (Dish);

class Ditem : public QWidget
{
  Q_OBJECT
  friend class Home;

private:
  Ui::Ditem *ui = new Ui::Ditem;

public:
  Ditem (QWidget *parent, Dish const &info);
  ~Ditem () { delete ui; }

public:
  void set_info (Dish const &info);
};

#endif
