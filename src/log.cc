#include "http.h"
#include "log.h"
#include "reg.h"
#include "util.h"

#include <QButtonGroup>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

#include <QtNetwork/QNetworkReply>

Log::Log (QWidget *parent) : QMainWindow (parent)
{
  ui->setupUi (this);

  btns = new QButtonGroup (this);
  btns->addButton (ui->rbtn1, 1);
  btns->addButton (ui->rbtn2, 2);
}

int
Log::category ()
{
  if (ui->rbtn1->isChecked ())
    return 1;
  if (ui->rbtn2->isChecked ())
    return 2;
  abort ();
}

void
Log::on_pbtn1_clicked ()
{
  auto type = category ();
  auto user = ui->ledit1->text ();
  auto pass = ui->ledit2->text ();

  if (user.isEmpty () || pass.isEmpty ())
    {
      QMessageBox::warning (this, tr ("提示"), tr ("请输入帐号密码"));
      return;
    }

  Reg *reg;
  switch (type)
    {
    case 1:
      if (!sreg)
        sreg = new Reg (this, 1);
      reg = sreg;
      break;
    case 2:
      if (!mreg)
        mreg = new Reg (this, 2);
      reg = mreg;
      break;
    }

  if (reg->isVisible ())
    return;

  reg->user = std::move (user);
  reg->pass = std::move (pass);
  reg->show ();
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

  QString req_url = (category () == 1) ? URL_STUDENT_LOG : URL_MERCHANT_LOG;

  QJsonObject req_data;
  req_data["user"] = user;
  req_data["pass"] = pass;

  Http http;
  req_finished (http.post (req_url, req_data));
}

void
Log::req_finished (QNetworkReply *reply)
{
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

  QMessageBox::information (this, tr ("提示"), tr ("登录成功"));
}
