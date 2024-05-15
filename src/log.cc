#include "log.h"
#include "home.h"
#include "http.h"
#include "reg.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

#include <QtNetwork/QNetworkReply>

Log::Log () : QMainWindow ()
{
  ui->setupUi (this);
  setAttribute (Qt::WA_DeleteOnClose);

  btns.addButton (ui->rbtn1, 1);
  btns.addButton (ui->rbtn2, 2);
}

type
Log::category ()
{
  if (ui->rbtn1->isChecked ())
    return type::STUDENT;
  if (ui->rbtn2->isChecked ())
    return type::MERCHANT;
  abort ();
}

void
Log::on_pbtn1_clicked ()
{
  auto user = ui->ledit1->text ();
  auto pass = ui->ledit2->text ();

  if (user.isEmpty () || pass.isEmpty ())
    {
      QMessageBox::warning (this, tr ("提示"), tr ("请输入帐号密码"));
      return;
    }

  if (!page_reg)
    page_reg = new Reg (this);
  if (page_reg->isVisible ())
    return;
  page_reg->show ();
}

void
Log::on_pbtn2_clicked ()
{
  auto user = ui->ledit1->text ();
  auto pass = ui->ledit2->text ();

  if (user.isEmpty () || pass.isEmpty ())
    {
      QMessageBox::warning (this, tr ("提示"), tr ("请输入帐号密码"));
      return;
    }

  QString req_url;
  auto typ = category ();

  switch (typ)
    {
    case type::STUDENT:
      req_url = URL_STUDENT_LOG;
      break;

    case type::MERCHANT:
      req_url = URL_MERCHANT_LOG;
      break;
    }

  QJsonObject req_data;
  req_data["user"] = user;
  req_data["pass"] = pass;

  Http http;
  auto reply = http.post (req_url, req_data);

  auto res = Http::get_data (reply, this);
  if (!res.has_value ())
    return;

  QMap<QString, QString> info;
  auto map = res.value ()["data"].toObject ().toVariantMap ();
  for (auto it = map.cbegin (); it != map.cend (); it++)
    info.insert (it.key (), it.value ().toString ());

  auto home = new Home (typ, std::move (info));
  home->show ();
  close ();
}
