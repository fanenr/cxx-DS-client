#include "mod.h"
#include "home.h"
#include "http.h"
#include "qmainwindow.h"
#include "util.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

#include <QtNetwork/QNetworkReply>

Mod::Mod (Home *parent) : QMainWindow (parent), prnt (parent)
{
  ui->setupUi (this);

  ui->label2->setText (tr ("新密码"));
  ui->label4->setText (tr ("新电话"));
  
  switch (prnt->typ)
    {
    case type::STUDENT:
      ui->label3->setText (tr ("新昵称"));
      ui->label5->hide ();
      ui->ledit4->hide ();
      break;
    case type::MERCHANT:
      ui->label3->setText (tr ("新名称"));
      ui->label5->setText (tr ("新位置"));
      break;
    default:
      break;
    }
}

void
Mod::showEvent (QShowEvent *event)
{
  QMainWindow::showEvent (event);

  ui->ledit1->setText (prnt->info["pass"]);
  ui->ledit2->setText (prnt->info["name"]);
  ui->ledit3->setText (prnt->info["number"]);

  if (prnt->typ == type::MERCHANT)
    ui->ledit4->setText (prnt->info["position"]);
}

void
Mod::on_pbtn1_clicked ()
{
  close ();
}

void
Mod::on_pbtn2_clicked ()
{
  QString req_url;
  QJsonObject req_data;

  req_data["user"] = prnt->info["user"];
  req_data["pass"] = prnt->info["pass"];

  switch (prnt->typ)
    {
    case type::STUDENT:
      req_url = URL_STUDENT_DEL;
      break;
    case type::MERCHANT:
      req_url = URL_MERCHANT_DEL;
      break;
    default:
      break;
    }

  Http http;
  auto reply = http.post (req_url, req_data);
  if (reply->error ())
    {
      QMessageBox::warning (this, tr ("失败"), tr ("无法发送网络请求"));
      return;
    }

  auto res = QJsonDocument::fromJson (reply->readAll ()).object ();
  if (res["code"] != 0)
    {
      QMessageBox::warning (this, tr ("失败"),
                            res["data"].toString (tr ("信息丢失")));
      return;
    }

  QMessageBox::information (this, tr ("提示"), tr ("注销成功"));
  prnt->close ();
}

void
Mod::on_pbtn3_clicked ()
{
  auto npass = ui->ledit1->text ();
  auto nname = ui->ledit2->text ();
  auto nnumber = ui->ledit3->text ();
  auto nposition = ui->ledit4->text ();

  auto typ = prnt->typ;

  if (npass.isEmpty () || nname.isEmpty () || nnumber.isEmpty ()
      || (nposition.isEmpty () && typ == type::MERCHANT))
    {
      QMessageBox::warning (this, tr ("提示"), tr ("请完整填写信息"));
      return;
    }

  QString req_url;
  QJsonObject req_data;
  auto &info = prnt->info;

  req_data["user"] = info["user"];
  req_data["pass"] = info["pass"];
  req_data["npass"] = npass;
  req_data["nname"] = nname;
  req_data["nnumber"] = nnumber;

  switch (typ)
    {
    case type::STUDENT:
      req_url = URL_STUDENT_MOD;
      break;
    case type::MERCHANT:
      req_url = URL_MERCHANT_MOD;
      req_data["nposition"] = nposition;
      break;
    default:
      break;
    }

  Http http;
  auto reply = http.post (req_url, req_data);
  if (reply->error ())
    {
      QMessageBox::warning (this, tr ("失败"), tr ("无法发送网络请求"));
      return;
    }

  auto res = QJsonDocument::fromJson (reply->readAll ()).object ();
  if (res["code"] != 0)
    {
      QMessageBox::warning (this, tr ("失败"),
                            res["data"].toString (tr ("信息丢失")));
      return;
    }

  info["pass"] = std::move (npass);
  info["name"] = std::move (nname);
  info["number"] = std::move (nnumber);
  if (typ == type::MERCHANT)
    info["position"] = std::move (nposition);

  prnt->load_info ();
  close ();
}