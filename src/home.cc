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

  auto log = new Log (this);
  log->show ();
}

void
Home::showEvent (QShowEvent *event)
{
  QMainWindow::showEvent (event);

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
  static bool load_dish_sent;
  static QNetworkAccessManager *nam;

  if (load_dish_sent)
    return;

  load_dish_sent = true;

  nam = Http::post (
      URL_MENU_LIST, {},
      [this] (QNetworkReply *reply) {
        load_dish_sent = false;

        if (reply->error ())
          {
            QMessageBox::warning (this, tr ("失败"), tr ("无法拉取菜品菜单"));
            return;
          }

        auto res = QJsonDocument::fromJson (reply->readAll ()).object ();
        if (res["code"] != 0)
          {
            QMessageBox::warning (this, tr ("失败"),
                                  res["data"].toString (tr ("信息丢失")));
            return;
          }

        QVector<Dish> vec;
        auto arr = res["data"].toArray ();
        for (auto const &item : arr)
          {
            auto const &obj = item.toObject ();
            vec.push_back ({ .id = obj["id"].toInteger (),
                             .name = obj["name"].toString (),
                             .user = obj["user"].toString (),
                             .price = obj["price"].toDouble (),
                             .position = obj["position"].toString () });
          }

        auto list = ui->list;
        list->clear ();
        for (auto &dish : vec)
          {
            auto item = new QListWidgetItem (list);
            auto widget = new Ditem (list, dish);

            item->setSizeHint (widget->sizeHint ());
            item->setData (Qt::UserRole,
                           QVariant::fromValue (std::move (dish)));

            list->setItemWidget (item, widget);
          }
      },
      nam);
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
        smod = new Mod (this);
      mod = smod;
      break;
    case type::MERCHANT:
      if (!mmod)
        mmod = new Mod (this);
      mod = mmod;
      break;
    default:
      break;
    }

  if (mod->isVisible ())
    return;

  mod->show ();
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
  QWidget *page;
  static New *dish;

  switch (typ)
    {
    case type::STUDENT:
      break;
    case type::MERCHANT:
      if (!dish)
        dish = new New (this, oper::NEW);
      page = dish;
      break;
    default:
      break;
    }

  if (page->isVisible ())
    return;

  page->show ();
}

void
Home::on_pbtn6_clicked ()
{
}
