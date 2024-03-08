#ifndef MAIN_H
#define MAIN_H

#include "ui_main.h"
#include "ui_reg.h"
#include "util.h"

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
  QNetworkRequest req;
  Ui::Reg *ui = new Ui::Reg;

  int type;
  QString user;
  QString pass;

public:
  Reg (QWidget *parent, int category);
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

inline Reg::Reg (QWidget *parent, int category)
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

  req.setHeader (QNetworkRequest::ContentTypeHeader, "application/json");
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

  QJsonObject req_data;
  req_data["user"] = user;
  req_data["pass"] = pass;

  switch (type)
    {
    case 1:
      req_data["id"] = str1;
      req_data["name"] = str2;
      req_data["number"] = str3;
      req.setUrl (QUrl (URL_STUDENT_NEW));
      break;
    case 2:
      req_data["name"] = str1;
      req_data["number"] = str2;
      req_data["position"] = str3;
      req.setUrl (QUrl (URL_MERCHANT_NEW));
      break;
    default:
      return;
    }

  static QNetworkAccessManager *nam;
  if (!nam)
    {
      nam = new QNetworkAccessManager (this);
      connect (nam, &QNetworkAccessManager::finished, this,
               &Reg::req_finished);
    }

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
  Reg *sreg = nullptr;
  Reg *mreg = nullptr;
  QNetworkRequest req;
  Ui::Main *ui = new Ui::Main;
  QButtonGroup *btns = nullptr;

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
    abort ();
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

  req.setHeader (QNetworkRequest::ContentTypeHeader, "application/json");
}

inline void
Main::on_pbtn1_clicked ()
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

  QJsonObject req_data;
  req_data["user"] = user;
  req_data["pass"] = pass;
  qInfo () << req_data;

  switch (category ())
    {
    case 1:
      req.setUrl (QUrl (URL_STUDENT_LOG));
      break;
    case 2:
      req.setUrl (QUrl (URL_MERCHANT_LOG));
      break;
    }

  static QNetworkAccessManager *nam;
  if (!nam)
    {
      nam = new QNetworkAccessManager (this);
      connect (nam, &QNetworkAccessManager::finished, this,
               &Main::req_finished);
    }

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
