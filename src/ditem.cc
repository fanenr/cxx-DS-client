#include "ditem.h"

Ditem::Ditem (QWidget *parent, Dish const &info) : QWidget (parent)
{
  ui->setupUi (this);
  set_info (info);
}

void
Ditem::set_info (Dish const &info)
{
  ui->label1->setText (info.name);
  ui->label2->setText (QString::number (info.price) + tr (" 元"));
  ui->label3->setText (info.owner);
  ui->label4->setText (info.position);
}
