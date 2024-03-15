#include "eva.h"
#include "ditem.h"
#include "home.h"
#include "http.h"
#include "util.h"

Eva::Eva (Home *parent) : QMainWindow (parent), prnt (parent)
{
  ui->setupUi (this);
}

void
Eva::show (oper op)
{
  QMainWindow::show ();
  this->op = op;
  switch (op)
    {
    case oper::NEW:
      ui->pbtn2->setVisible (false);
      setWindowTitle (tr ("添加评价"));
      ui->label1->setText (tr ("添加评价"));
      break;
    case oper::MOD:
      ui->pbtn2->setVisible (true);
      setWindowTitle (tr ("修改评价"));
      ui->label1->setText (tr ("修改评价"));
      break;
    default:
      break;
    }

  auto item = prnt->ui->list->currentItem ();
  if (!item)
    return;

  auto const &eval = item->data (Qt::UserRole).value<Dish> ();
  id = eval.id;
}

void
Eva::on_pbtn1_clicked ()
{
  close ();
}

void
Eva::on_pbtn2_clicked ()
{
  QJsonObject req_data;
  req_data["user"] = prnt->info["user"];
  req_data["pass"] = prnt->info["pass"];
  req_data["id"] = id;

  QString req_url = URL_EVA_DEL;

  Http http;
  auto reply = http.post (req_url, req_data);
  if (!Http::get_data (reply, this).has_value ())
    return;

  QMessageBox::information (this, tr ("提示"), tr ("操作成功，返回查看"));
  prnt->load_dish ();
  close ();
}

void
Eva::on_pbtn3_clicked ()
{
  auto grade = ui->spin->value ();
  auto evaluation = ui->edit->toPlainText ();

  if (evaluation.isEmpty ())
    {
      QMessageBox::warning (this, tr ("提示"), tr ("请完整填写信息"));
      return;
    }

  QString req_url;
  QJsonObject req_data;
  req_data["id"] = this->id;
  req_data["user"] = prnt->info["user"];
  req_data["pass"] = prnt->info["pass"];

  switch (op)
    {
    case oper::NEW:
      req_data["grade"] = grade;
      req_data["evaluation"] = evaluation;
      req_url = URL_EVA_NEW;
      break;
    case oper::MOD:
      req_data["ngrade"] = grade;
      req_data["nevaluation"] = evaluation;
      req_url = URL_EVA_MOD;
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
