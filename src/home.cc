#include "home.h"
#include "ditem.h"
#include "eitem.h"
#include "eva.h"
#include "http.h"
#include "log.h"
#include "mod.h"
#include "new.h"
#include "util.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <algorithm>

#include <QtNetwork/QNetworkReply>

Home::Home ()
{
  ui->setupUi (this);

  connect (ui->sort, &QCheckBox::toggled, this, &Home::sort_changed);
  connect (ui->list, &QListWidget::currentItemChanged, this,
           &Home::item_selected);

  page_log = new Log (this);
  page_log->show ();
}

void
Home::init_ui ()
{
  if (typ == type::MERCHANT)
    {
      ui->hint1->setText (tr ("店名: "));
      ui->hint3->setText (tr ("位置: "));
      ui->pbtn5->setText (tr ("添加菜品"));
      ui->pbtn6->setText (tr ("修改菜品"));
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
Home::load_eva ()
{
  auto item = ui->list->currentItem ();
  auto id = item->data (Qt::UserRole).value<Dish> ().id;

  sts = stat::EVA;
  ui->list->clear ();
  ui->list->clearSelection ();

  ui->pbtn4->setEnabled (false);
  ui->pbtn5->setEnabled (false);
  ui->pbtn6->setEnabled (false);

  static QNetworkAccessManager *nam;
  QJsonObject req_data;
  req_data["id"] = id;

  nam = Http::post (
      URL_EVA_LIST, req_data,
      [this] (QNetworkReply *reply) {
        auto res = Http::get_data (reply, this);
        if (!res.has_value ())
          return;

        QVector<Eval> vec;
        auto arr = res.value ()["data"].toArray ();
        for (auto const &item : arr)
          {
            auto const &obj = item.toObject ();
            vec.push_back ({ .id = obj["id"].toInteger (),
                             .user = obj["user"].toString (),
                             .grade = obj["grade"].toDouble (),
                             .uname = obj["uname"].toString (),
                             .evaluation = obj["evaluation"].toString () });
          }

        auto list = ui->list;
        for (auto &eval : vec)
          {
            auto widget = new Eitem (list, eval);
            auto item = new QListWidgetItem (list);

            item->setSizeHint (widget->sizeHint ());
            item->setData (Qt::UserRole,
                           QVariant::fromValue (std::move (eval)));

            list->setItemWidget (item, widget);
          }
      },
      nam);
}

void
Home::load_dish ()
{
  sts = stat::DISH;
  ui->list->clear ();
  ui->list->clearSelection ();

  ui->pbtn3->setEnabled (false);
  ui->pbtn4->setEnabled (false);
  ui->pbtn5->setEnabled (false);
  ui->pbtn6->setEnabled (false);

  static QNetworkAccessManager *nam;

  nam = Http::post (
      URL_MENU_LIST, {},
      [this] (QNetworkReply *reply) {
        ui->pbtn3->setEnabled (true);

        auto res = Http::get_data (reply, this);
        if (!res.has_value ())
          return;

        QVector<Dish> vec;
        auto arr = res.value ()["data"].toArray ();
        for (auto const &item : arr)
          {
            auto const &obj = item.toObject ();
            vec.push_back ({ .id = obj["id"].toInteger (),
                             .name = obj["name"].toString (),
                             .user = obj["user"].toString (),
                             .price = obj["price"].toDouble (),
                             .uname = obj["uname"].toString (),
                             .position = obj["position"].toString () });
          }

        auto list = ui->list;
        for (auto &dish : vec)
          {
            auto widget = new Ditem (list, dish);
            auto item = new QListWidgetItem (list);

            item->setSizeHint (widget->sizeHint ());
            item->setData (Qt::UserRole,
                           QVariant::fromValue (std::move (dish)));

            list->setItemWidget (item, widget);
          }

        if (typ == type::MERCHANT)
          ui->pbtn5->setEnabled (true);
      },
      nam);
}

void
Home::sort_changed (bool checked)
{
  static bool is_sorting;
  if (is_sorting)
    return;

  is_sorting = true;
  auto ui_list = ui->list;
  QList<QListWidgetItem *> list;
  auto size = ui_list->count ();

  if (size < 2)
    {
      is_sorting = false;
      return;
    }

  for (; size > 0;)
    list.push_back (ui_list->takeItem (--size));

  std::sort (list.begin (), list.end (),
             [this, checked] (QListWidgetItem *a, QListWidgetItem *b) {
               double num1, num2;

               if (sts == stat::DISH)
                 {
                   auto const &dish1 = a->data (Qt::UserRole).value<Dish> ();
                   auto const &dish2 = b->data (Qt::UserRole).value<Dish> ();
                   num1 = dish1.price;
                   num2 = dish2.price;
                 }

               if (sts == stat::EVA)
                 {
                   auto const &eval1 = a->data (Qt::UserRole).value<Eval> ();
                   auto const &eval2 = b->data (Qt::UserRole).value<Eval> ();
                   num1 = eval1.grade;
                   num2 = eval2.grade;
                 }

               return checked ? num1 > num2 : num1 < num2;
             });

  if (sts == stat::DISH)
    for (auto item : list)
      {
        auto const &dish = item->data (Qt::UserRole).value<Dish> ();
        auto widget = new Ditem (this, dish);

        ui_list->addItem (item);
        ui_list->setItemWidget (item, widget);
      }

  if (sts == stat::EVA)
    for (auto item : list)
      {
        auto const &eval = item->data (Qt::UserRole).value<Eval> ();
        auto widget = new Eitem (this, eval);

        ui_list->addItem (item);
        ui_list->setItemWidget (item, widget);
      }

  is_sorting = false;
}

void
Home::item_selected (QListWidgetItem *item, QListWidgetItem *prev)
{
  if (sts == stat::DISH)
    {
      if (!item)
        return;

      ui->pbtn4->setEnabled (true);
      ui->pbtn5->setEnabled (true);
      ui->pbtn6->setEnabled (true);
    }
}

void
Home::on_pbtn1_clicked ()
{
  if (!page_mod)
    page_mod = new Mod (this);
  if (page_mod->isVisible ())
    return;
  page_mod->show ();
}

void
Home::on_pbtn2_clicked ()
{
  close ();
}

void
Home::on_pbtn3_clicked ()
{
  load_dish ();
}

void
Home::on_pbtn4_clicked ()
{
  load_eva ();
}

void
Home::on_pbtn5_clicked ()
{
  switch (typ)
    {
    case type::STUDENT:
      if (!page_eva)
        page_eva = new Eva (this);
      if (page_eva->isVisible ())
        return;
      page_eva->show (oper::NEW);
      break;
    case type::MERCHANT:
      if (!page_new)
        page_new = new New (this);
      if (page_new->isVisible ())
        return;
      page_new->show (oper::NEW);
      break;
    default:
      break;
    }
}

void
Home::on_pbtn6_clicked ()
{
  switch (typ)
    {
    case type::STUDENT:
      if (!page_eva)
        page_eva = new Eva (this);
      if (page_eva->isVisible ())
        return;
      page_eva->show (oper::MOD);
      break;
    case type::MERCHANT:
      if (!page_new)
        page_new = new New (this);
      if (page_new->isVisible ())
        return;
      page_new->show (oper::MOD);
      break;
    default:
      break;
    }
}
