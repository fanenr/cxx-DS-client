#include "http.h"
#include "reg.h"
#include "util.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

#include <QtNetwork/QNetworkReply>

Reg::Reg (QWidget *parent, int category)
    : QMainWindow (parent), type (category)
{
  ui->setupUi (this);

  switch (type)
    {
    case 1:
      setWindowTitle (tr ("学生注册"));
      ui->label1->setText (tr ("学生注册"));
      ui->label2->setText (tr ("学号"));
      ui->label3->setText (tr ("昵称"));
      ui->label4->setText (tr ("电话"));
      break;
    case 2:
      setWindowTitle (tr ("商户注册"));
      ui->label1->setText (tr ("商户注册"));
      ui->label2->setText (tr ("名称"));
      ui->label3->setText (tr ("电话"));
      ui->label4->setText (tr ("位置"));
      break;
    }
};

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
  req_data["user"] = user;
  req_data["pass"] = pass;

  switch (type)
    {
    case 1:
      req_data["id"] = str1;
      req_data["name"] = str2;
      req_data["number"] = str3;
      req_url = URL_STUDENT_NEW;
      break;
    case 2:
      req_data["name"] = str1;
      req_data["number"] = str2;
      req_data["position"] = str3;
      req_url = URL_MERCHANT_NEW;
      break;
    }

  Http http;
  req_finished (http.post (req_url, req_data));
}

void
Reg::req_finished (QNetworkReply *reply)
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

  this->close ();
  QMessageBox::information (nullptr, tr ("提示"), tr ("注册成功"));
}
