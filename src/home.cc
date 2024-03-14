#include "home.h"
#include "ditem.h"
#include "http.h"
#include "log.h"
#include "mod.h"
#include "new.h"
#include "util.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

#include <QtNetwork/QNetworkReply>

Home::Home ()
{
  ui->setupUi (this);

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
      ui->pbtn5->setText (tr ("新建菜品"));
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
Home::load_dish ()
{
  ui->list->clear ();
  ui->list->clearSelection ();
  ui->pbtn3->setEnabled (false);

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
      },
      nam);
}

void
Home::item_selected (QListWidgetItem *item, QListWidgetItem *prev)
{
  if (item && typ == type::MERCHANT)
    {
      auto const &dish = item->data (Qt::UserRole).value<Dish> ();
      ui->pbtn6->setEnabled (info["user"] == dish.user);
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
Home::on_pbtn5_clicked ()
{
  switch (typ)
    {
    case type::STUDENT:
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
