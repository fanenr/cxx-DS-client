#include "home.h"
#include "ditem.h"
#include "mod.h"
#include "util.h"

Home::Home (QWidget *parent, type typ) : QMainWindow (parent), typ (typ)
{
  ui->setupUi (this);
  setAttribute (Qt::WA_DeleteOnClose);

  switch (typ)
    {
    case type::STUDENT:
      ui->hint3->setText (tr ("学号: "));
      break;
    case type::MERCHANT:
      ui->hint3->setText (tr ("位置: "));
      ui->pbtn4->setText (tr ("修改菜品"));
      break;
    default:
      break;
    }
}

void
Home::load_info ()
{
  ui->info1->setText (info["name"]);
  ui->info2->setText (info["user"]);
  ui->info4->setText (info["number"]);

  switch (typ)
    {
    case type::STUDENT:
      ui->info3->setText (info["id"]);
      break;
    case type::MERCHANT:
      ui->info3->setText (info["position"]);
      break;
    default:
      break;
    }
}

void
Home::load_dish ()
{
  Dish temp = { .id = 0,
                .price = 10,
                .name = "测试",
                .owner = "包子铺",
                .position = "南餐厅 1 楼 1 号" };

  QVector<Dish> vec;
  for (auto i = 0; i < 16; i++)
    {
      temp.price = i + 1;
      vec.push_back (temp);
    }

  auto list = ui->list;
  list->clear ();

  for (auto i = 0; i < vec.size (); i++)
    {
      auto item = new QListWidgetItem (list);
      item->setData (Qt::UserRole, QVariant::fromValue (vec[i]));

      auto widget = new Ditem (list, vec[i]);
      item->setSizeHint (widget->sizeHint ());

      list->setItemWidget (item, widget);
    }
}

void
Home::on_pbtn1_clicked ()
{
  Mod *mod;
  static Mod *smod;
  static Mod *mmod;

  switch (typ)
    {
    case type::STUDENT:
      if (!smod)
        smod = new Mod (this, type::STUDENT, info);
      mod = smod;
      break;
    case type::MERCHANT:
      if (!mmod)
        mmod = new Mod (this, type::MERCHANT, info);
      mod = mmod;
      break;
    default:
      break;
    }

  if (mod->isVisible ())
    return;

  mod->load_old ();
  mod->show ();
}

void
Home::on_pbtn2_clicked ()
{
  close ();
}

void
Home::on_pbtn5_clicked ()
{
  load_dish ();
}
