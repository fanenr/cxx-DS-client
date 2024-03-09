#include "mod.h"
#include "home.h"
#include "http.h"
#include "util.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

#include <QtNetwork/QNetworkReply>

Mod::Mod (QMainWindow *parent, type typ, decltype (old) old)
    : QMainWindow (parent), typ (typ), old (old)
{
  ui->setupUi (this);

  switch (typ)
    {
    case type::STUDENT:
      ui->label2->setText (tr ("新密码"));
      ui->label3->setText (tr ("新昵称"));
      ui->label4->setText (tr ("新电话"));
      ui->label5->hide ();
      ui->ledit4->hide ();
      break;
    case type::MERCHANT:
      ui->label2->setText (tr ("新密码"));
      ui->label3->setText (tr ("新名称"));
      ui->label4->setText (tr ("新电话"));
      ui->label5->setText (tr ("新位置"));
      break;
    default:
      break;
    }
}

void
Mod::load_old ()
{
  switch (typ)
    {
    case type::STUDENT:
      ui->ledit1->setText (old["pass"]);
      ui->ledit2->setText (old["name"]);
      ui->ledit3->setText (old["number"]);
      break;
    case type::MERCHANT:
      ui->ledit1->setText (old["pass"]);
      ui->ledit2->setText (old["name"]);
      ui->ledit3->setText (old["number"]);
      ui->ledit4->setText (old["position"]);
      break;
    default:
      break;
    }
}

void
Mod::on_pbtn1_clicked ()
{
  this->close ();
}

void
Mod::on_pbtn2_clicked ()
{
  QString req_url;
  QJsonObject req_data;

  req_data["user"] = old["user"];
  req_data["pass"] = old["pass"];

  switch (typ)
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

  close ();
  parentWidget ()->close ();
  QMessageBox::information (nullptr, tr ("提示"), tr ("注销成功"));
}

void
Mod::on_pbtn3_clicked ()
{
  auto npass = ui->ledit1->text ();
  auto nname = ui->ledit2->text ();
  auto nnumber = ui->ledit3->text ();
  auto nposition = ui->ledit4->text ();

  if (npass.isEmpty () || nname.isEmpty () || nnumber.isEmpty ()
      || (nposition.isEmpty () && typ == type::MERCHANT))
    {
      QMessageBox::warning (this, tr ("提示"), tr ("请完整填写信息"));
      return;
    }

  QString req_url;
  QJsonObject req_data;

  req_data["user"] = old["user"];
  req_data["pass"] = old["pass"];
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

  old["pass"] = std::move (npass);
  old["name"] = std::move (nname);
  old["number"] = std::move (nnumber);
  if (typ == type::MERCHANT)
    old["position"] = std::move (nposition);

  auto prnt = dynamic_cast<Home *> (parent ());
  prnt->load_info ();
  close ();
}
