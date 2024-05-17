#include "eitem.h"

Eitem::Eitem (QWidget *parent, Eval const &info) : QWidget (parent)
{
  ui.setupUi (this);
  set_info (info);
}

void
Eitem::set_info (Eval const &info)
{
  ui.label1->setText (info.uname);
  ui.label3->setText (info.evaluation);
  ui.label2->setText (QString::number (info.grade, 'f', 1) + tr (" 分"));
}
