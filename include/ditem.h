#ifndef DITEM_H
#define DITEM_H

#include "ui_ditem.h"

struct Dish
{
  quint32 id;
  double price;
  QString user;
  QString name;
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
