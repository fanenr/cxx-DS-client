#include "home.h"
#include "ditem.h"
#include "eitem.h"
#include "eva.h"
#include "http.h"
#include "mod.h"
#include "new.h"

#include <QJsonArray>

Home::Home (type typ, info_type info)
    : typ (typ), info (std::move (info)), QMainWindow ()
{
  ui.setupUi (this);
  setAttribute (Qt::WA_DeleteOnClose);

  if (typ == type::MERCHANT)
    {
      ui.hint1->setText (tr ("店名: "));
      ui.hint3->setText (tr ("位置: "));
      ui.pbtn5->setText (tr ("添加菜品"));
      ui.pbtn6->setText (tr ("修改菜品"));
    }

  connect (ui.sort, &QCheckBox::toggled, this, &Home::sort_changed);
  connect (ui.list, &QListWidget::currentItemChanged, this,
           &Home::item_selected);

  load_info ();
  load_dish ();
}

void
Home::load_info ()
{
  ui.info1->setText (info["name"]);
  ui.info2->setText (info["user"]);
  ui.info4->setText (info["number"]);

  switch (typ)
    {
    case type::STUDENT:
      ui.info3->setText (info["id"]);
      break;

    case type::MERCHANT:
      ui.info3->setText (info["position"]);
      break;
    }
}

void
Home::load_dish ()
{
  sts = stat::DISH;
  ui.list->clear ();
  ui.list->clearSelection ();
  ui.group_2->setTitle ("菜品列表");

  ui.pbtn3->setEnabled (false);
  ui.pbtn4->setEnabled (false);
  ui.pbtn5->setEnabled (false);
  ui.pbtn6->setEnabled (false);

  static QNetworkAccessManager *nam;

  nam = Http::post (
      URL_MENU_LIST, {},
      [this] (QNetworkReply *reply) {
        ui.pbtn3->setEnabled (true);

        auto res = Http::get_data (reply, this);
        if (!res.has_value ())
          return;

        auto arr = res.value ()["data"].toArray ();
        for (auto item : arr)
          {
            auto obj = item.toObject ();
            auto dish = (Dish){
              .id = obj["id"].toInteger (),
              .price = obj["price"].toDouble (),
              .name = obj["name"].toString (),
              .user = obj["user"].toString (),
              .uname = obj["uname"].toString (),
              .position = obj["position"].toString (),
            };
            new DishItem (ui.list, std::move (dish));
          }

        if (typ == type::MERCHANT)
          ui.pbtn5->setEnabled (true);
      },
      nam);
}

void
Home::load_eval ()
{
  auto id = dynamic_cast<DishItem *> (ui.list->currentItem ())->data.id;

  sts = stat::EVA;
  ui.list->clear ();
  ui.list->clearSelection ();
  ui.group_2->setTitle ("评价列表");

  ui.pbtn4->setEnabled (false);
  ui.pbtn5->setEnabled (false);
  ui.pbtn6->setEnabled (false);

  static QNetworkAccessManager *nam;

  auto req_data = QJsonObject ();
  req_data["id"] = qint64 (id);

  nam = Http::post (
      URL_EVA_LIST, req_data,
      [this] (QNetworkReply *reply) {
        auto res = Http::get_data (reply, this);
        if (!res.has_value ())
          return;

        auto arr = res.value ()["data"].toArray ();
        for (auto elem : arr)
          {
            auto obj = elem.toObject ();
            auto eval = (Eval){
              .id = obj["id"].toInteger (),
              .grade = obj["grade"].toDouble (),
              .user = obj["user"].toString (),
              .uname = obj["uname"].toString (),
              .evaluation = obj["evaluation"].toString (),
            };
            new EvalItem (ui.list, std::move (eval));
          }
      },
      nam);
}

void
Home::sort_changed (bool dec)
{
  ui.list->sortItems (dec ? Qt::DescendingOrder : Qt::AscendingOrder);
}

void
Home::item_selected (QListWidgetItem *item, QListWidgetItem *prev)
{
  if (sts == stat::DISH && item)
    {
      ui.pbtn4->setEnabled (true);
      ui.pbtn5->setEnabled (true);
      ui.pbtn6->setEnabled (true);
    }
}

void
Home::on_pbtn1_clicked ()
{
  auto mod = Mod (this);
  mod.exec ();
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
  load_eval ();
}

void
Home::on_pbtn5_clicked ()
{
  switch (typ)
    {
    case type::STUDENT:
      {
        auto dialog = Eva (this, oper::NEW);
        dialog.exec ();
      }
      break;

    case type::MERCHANT:
      {
        auto dialog = New (this, oper::NEW);
        dialog.exec ();
      }
      break;
    }
}

void
Home::on_pbtn6_clicked ()
{
  switch (typ)
    {
    case type::STUDENT:
      {
        auto dialog = Eva (this, oper::MOD);
        dialog.exec ();
      }
      break;

    case type::MERCHANT:
      {
        auto dialog = New (this, oper::MOD);
        dialog.exec ();
      }
      break;
    }
}
