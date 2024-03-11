#include "new.h"
#include "home.h"
#include "http.h"
#include "util.h"

#include <QJsonObject>
#include <QMessageBox>

#include <QtNetwork/QNetworkReply>

New::New (Home *parent, oper op) : QMainWindow (parent), prnt (parent), op (op)
{
  ui->setupUi (this);

  switch (op)
    {
    case oper::NEW:
      ui->pbtn2->hide ();
      ui->hlay2->removeItem (ui->hsp2);
      break;
    case oper::MOD:
      setWindowTitle (tr ("修改菜品"));
      ui->pbtn3->setText (tr ("修改"));
      ui->label1->setText (tr ("修改菜品"));
      break;
    default:
      break;
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

  QJsonObject req_data;
  req_data["name"] = name;
  req_data["user"] = prnt->info["user"];
  req_data["pass"] = prnt->info["pass"];

  QString req_url;
  switch (op)
    {
    case oper::NEW:
      req_data["price"] = price;
      req_url = URL_MENU_NEW;
      break;
    case oper::MOD:
      req_data["nprice"] = price;
      req_url = URL_MENU_MOD;
      break;
    default:
      break;
    }

  Http http;
  auto reply = http.post (req_url, req_data);
  if (reply->error ())
    {
      QMessageBox::warning (this, tr ("失败"), tr ("无法提交网络请求"));
      return;
    }

  auto res = QJsonDocument::fromJson (reply->readAll ()).object ();
  if (res["code"] != 0)
    {
      QMessageBox::warning (this, tr ("失败"),
                            res["data"].toString (tr ("信息丢失")));
      return;
    }

  QMessageBox::information (this, tr ("提示"), tr ("操作成功，返回查看"));
  prnt->load_dish ();
  close ();
}