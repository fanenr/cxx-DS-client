#include "reg.h"
#include "http.h"
#include "log.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

#include <QtNetwork/QNetworkReply>

Reg::Reg (Log *parent) : QMainWindow (parent), prnt (parent)
{
  ui->setupUi (this);
}

void
Reg::show ()
{
  QMainWindow::show ();
  switch (prnt->category ())
    {
    case type::STUDENT:
      setWindowTitle (tr ("学生注册"));
      ui->label1->setText (tr ("学生注册"));
      ui->label2->setText (tr ("学号"));
      ui->label3->setText (tr ("昵称"));
      ui->label4->setText (tr ("电话"));
      break;
    case type::MERCHANT:
      setWindowTitle (tr ("商户注册"));
      ui->label1->setText (tr ("商户注册"));
      ui->label2->setText (tr ("名称"));
      ui->label3->setText (tr ("电话"));
      ui->label4->setText (tr ("位置"));
      break;
    default:
      break;
    }
}

void
Reg::on_pbtn1_clicked ()
{
  close ();
}

void
Reg::on_pbtn2_clicked ()
{
  auto str1 = ui->ledit1->text ();
  auto str2 = ui->ledit2->text ();
  auto str3 = ui->ledit3->text ();

  if (str1.isEmpty () || str2.isEmpty () || str3.isEmpty ())
    {
      QMessageBox::warning (this, tr ("提示"), tr ("请完整填写信息"));
      return;
    }

  QString req_url;
  QJsonObject req_data;

  req_data["user"] = prnt->ui->ledit1->text ();
  req_data["pass"] = prnt->ui->ledit2->text ();

  switch (prnt->category ())
    {
    case type::STUDENT:
      req_data["id"] = str1;
      req_data["name"] = str2;
      req_data["number"] = str3;
      req_url = URL_STUDENT_NEW;
      break;
    case type::MERCHANT:
      req_data["name"] = str1;
      req_data["number"] = str2;
      req_data["position"] = str3;
      req_url = URL_MERCHANT_NEW;
      break;
    default:
      break;
    }

  Http http;
  auto reply = http.post (req_url, req_data);
  if (!Http::get_data (reply, this).has_value ())
    return;

  QMessageBox::information (this, tr ("提示"), tr ("注册成功，请返回登录"));
  close ();
}
