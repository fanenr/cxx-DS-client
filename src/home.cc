#include "home.h"
#include "ditem.h"
#include "http.h"
#include "mod.h"
#include "util.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

#include <QtNetwork/QNetworkReply>

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
            Dish dish;
            auto const &obj = item.toObject ();

            dish.id = obj["id"].toInteger ();
            dish.name = obj["name"].toString ();
            dish.user = obj["user"].toString ();
            dish.price = obj["price"].toInteger ();
            dish.position = obj["position"].toString ();

            vec.push_back (std::move (dish));
          }

        for (auto &dish : vec)
          {
            auto item = new QListWidgetItem (ui->list);
            auto widget = new Ditem (ui->list, dish);

            item->setSizeHint (widget->sizeHint ());
            item->setData (Qt::UserRole,
                           QVariant::fromValue (std::move (dish)));

            ui->list->setItemWidget (item, widget);
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
