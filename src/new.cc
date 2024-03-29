#include "new.h"
#include "ditem.h"
#include "home.h"
#include "http.h"
#include "util.h"

#include <QJsonObject>
#include <QMessageBox>

#include <QtNetwork/QNetworkReply>

New::New (Home *parent) : QMainWindow (parent), prnt (parent)
{
  ui->setupUi (this);
}

void
New::show (oper op)
{
  QMainWindow::show ();
  this->op = op;
  switch (op)
    {
    case oper::NEW:
      ui->pbtn2->setVisible (false);
      setWindowTitle (tr ("新建菜品"));
      ui->pbtn3->setText (tr ("创建"));
      ui->label1->setText (tr ("新建菜品"));
      break;
    case oper::MOD:
      ui->pbtn2->setVisible (true);
      setWindowTitle (tr ("修改菜品"));
      ui->pbtn3->setText (tr ("修改"));
      ui->label1->setText (tr ("修改菜品"));
      break;
    default:
      break;
    }

  if (op == oper::MOD)
    {
      auto item = prnt->ui->list->currentItem ();
      if (!item)
        return;

      auto const &dish = item->data (Qt::UserRole).value<Dish> ();
      id = dish.id;
    }
}

void
New::on_pbtn1_clicked ()
{
  close ();
}

void
New::on_pbtn2_clicked ()
{
  QJsonObject req_data;
  req_data["user"] = prnt->info["user"];
  req_data["pass"] = prnt->info["pass"];
  req_data["id"] = id;

  QString req_url = URL_MENU_DEL;

  Http http;
  auto reply = http.post (req_url, req_data);
  if (!Http::get_data (reply, this).has_value ())
    return;

  QMessageBox::information (this, tr ("提示"), tr ("操作成功，返回查看"));
  prnt->load_dish ();
  close ();
}

void
New::on_pbtn3_clicked ()
{
  auto name = ui->ledit1->text ();
  auto price_str = ui->ledit2->text ();

  if (name.isEmpty () || price_str.isEmpty ())
    {
      QMessageBox::warning (this, tr ("提示"), tr ("请完整填写信息"));
      return;
    }

  auto price = price_str.toDouble ();

  QString req_url;
  QJsonObject req_data;
  req_data["user"] = prnt->info["user"];
  req_data["pass"] = prnt->info["pass"];

  switch (op)
    {
    case oper::NEW:
      req_data["name"] = name;
      req_data["price"] = price;
      req_url = URL_MENU_NEW;
      break;
    case oper::MOD:
      req_data["id"] = id;
      req_data["nname"] = name;
      req_data["nprice"] = price;
      req_url = URL_MENU_MOD;
      break;
    default:
      break;
    }

  Http http;
  auto reply = http.post (req_url, req_data);
  if (!Http::get_data (reply, this).has_value ())
    return;

  QMessageBox::information (this, tr ("提示"), tr ("操作成功，返回查看"));
  prnt->load_dish ();
  close ();
}
