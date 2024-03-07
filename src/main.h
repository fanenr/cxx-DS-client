#ifndef MAIN_H
#define MAIN_H

#include "ui_main.h"
#include "ui_reg.h"
#include "util.h"
#include <utility>

#include <QButtonGroup>
#include <QMessageBox>

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

class Reg : public QMainWindow
{
  friend class Main;

private:
  Q_OBJECT
  Ui::Reg *ui = new Ui::Reg;

  QNetworkRequest req;
  QNetworkAccessManager *nam = nullptr;

  int type;
  QString user;
  QString pass;

public:
  Reg (QWidget *parent);
  ~Reg () { delete ui; }

private:
  void req_finished (QNetworkReply *reply);

private slots:
  void
  on_pbtn1_clicked ()
  {
    close ();
  }

  void on_pbtn2_clicked ();
};

inline Reg::Reg (QWidget *parent) : QMainWindow (parent)
{
  ui->setupUi (this);

  nam = new QNetworkAccessManager (this);
  req.setHeader (QNetworkRequest::ContentTypeHeader, "application/json");
  connect (nam, &QNetworkAccessManager::finished, this, &Reg::req_finished);
};

inline void
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

  QUrl req_url;
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
    default:
      return;
    }

  req.setUrl (req_url);
  auto req_json = QJsonDocument (req_data).toJson (QJsonDocument::Compact);
  nam->post (req, req_json);
}

inline void
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

class Main : public QMainWindow
{
private:
  Q_OBJECT
  Reg *reg = nullptr;
  Ui::Main *ui = new Ui::Main;
  QButtonGroup *btns = nullptr;

  QNetworkRequest req;
  QNetworkAccessManager *nam = nullptr;

public:
  explicit Main (QWidget *parent = nullptr);
  ~Main () { delete ui; }

private:
  void req_finished (QNetworkReply *reply);

  int
  category ()
  {
    if (ui->rbtn1->isChecked ())
      return 1;
    if (ui->rbtn2->isChecked ())
      return 2;
    return 0;
  }

private slots:
  void on_pbtn1_clicked ();
  void on_pbtn2_clicked ();
};

inline Main::Main (QWidget *parent) : QMainWindow (parent)
{
  ui->setupUi (this);

  btns = new QButtonGroup (this);
  btns->addButton (ui->rbtn1, 1);
  btns->addButton (ui->rbtn2, 2);

  nam = new QNetworkAccessManager (this);
  req.setHeader (QNetworkRequest::ContentTypeHeader, "application/json");
  connect (nam, &QNetworkAccessManager::finished, this, &Main::req_finished);
}

inline void
Main::on_pbtn1_clicked ()
{
  if (!reg)
    reg = new Reg (this);

  auto type = category ();
  auto user = ui->ledit1->text ();
  auto pass = ui->ledit2->text ();

  if (user.isEmpty () || pass.isEmpty ())
    {
      QMessageBox::warning (this, tr ("提示"), tr ("请输入帐号密码"));
      return;
    }

  if (reg->isVisible ())
    return;

  switch (type)
    {
    case 1:
      reg->setWindowTitle (tr ("学生注册"));
      reg->ui->label1->setText (tr ("学生注册"));
      reg->ui->label2->setText (tr ("学号"));
      reg->ui->label3->setText (tr ("昵称"));
      reg->ui->label4->setText (tr ("电话"));
      break;
    case 2:
      reg->setWindowTitle (tr ("商户注册"));
      reg->ui->label1->setText (tr ("商户注册"));
      reg->ui->label2->setText (tr ("名称"));
      reg->ui->label3->setText (tr ("电话"));
      reg->ui->label4->setText (tr ("位置"));
      break;
    default:
      return;
    }

  reg->user = std::move (user);
  reg->pass = std::move (pass);
  reg->type = type;
  reg->show ();
}

inline void
Main::on_pbtn2_clicked ()
{
  auto user = ui->ledit1->text ();
  auto pass = ui->ledit2->text ();

  if (user.isEmpty () || pass.isEmpty ())
    {
      QMessageBox::warning (this, tr ("提示"), tr ("请输入帐号密码"));
      return;
    }

  QUrl req_url;
  QJsonObject req_data;
  req_data["user"] = user;
  req_data["pass"] = pass;

  switch (category ())
    {
    case 1:
      req_url = URL_STUDENT_LOG;
      break;
    case 2:
      req_url = URL_MERCHANT_LOG;
      break;
    default:
      return;
    }

  req.setUrl (req_url);
  auto req_json = QJsonDocument (req_data).toJson (QJsonDocument::Compact);
  nam->post (req, req_json);
}

inline void
Main::req_finished (QNetworkReply *reply)
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

#endif
