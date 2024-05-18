#include "eva.h"
#include "ditem.h"
#include "home.h"
#include "http.h"
#include "util.h"

Eva::Eva (Home *parent) : QDialog (parent)
{
  ui.setupUi (this);
  this->parent = parent;
}

int
Eva::exec (oper op)
{
  switch ((this->op = op))
    {
    case oper::NEW:
      ui.pbtn2->setVisible (false);
      setWindowTitle (tr ("添加评价"));
      ui.label1->setText (tr ("添加评价"));
      break;

    case oper::MOD:
      ui.pbtn2->setVisible (true);
      setWindowTitle (tr ("修改评价"));
      ui.label1->setText (tr ("修改评价"));
      break;
    }

  if (auto item = parent->ui.list->currentItem (); item)
    {
      auto const &eval = (item->data (Qt::UserRole)).value<Dish> ();
      id = eval.id;
    }

  return QDialog::exec ();
}

void
Eva::on_pbtn1_clicked ()
{
  close ();
}

void
Eva::on_pbtn2_clicked ()
{
  auto req_data = QJsonObject ();
  req_data["user"] = parent->info["user"];
  req_data["pass"] = parent->info["pass"];
  req_data["id"] = qint64 (id);

  auto req_url = QString (URL_EVA_DEL);

  auto http = Http ();
  auto reply = http.post (req_url, req_data);
  if (!Http::get_data (reply, this).has_value ())
    return;

  QMessageBox::information (this, tr ("提示"), tr ("操作成功，返回查看"));
  parent->load_dish ();
  close ();
}

void
Eva::on_pbtn3_clicked ()
{
  auto grade = ui.spin->value ();
  auto evaluation = ui.edit->toPlainText ();

  if (evaluation.isEmpty ())
    {
      QMessageBox::warning (this, tr ("提示"), tr ("请完整填写信息"));
      return;
    }

  auto req_url = QString ();
  auto req_data = QJsonObject ();

  req_data["id"] = qint64 (id);
  req_data["user"] = parent->info["user"];
  req_data["pass"] = parent->info["pass"];

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
    }

  auto http = Http ();
  auto reply = http.post (req_url, req_data);
  if (!Http::get_data (reply, this).has_value ())
    return;

  QMessageBox::information (this, tr ("提示"), tr ("操作成功，返回查看"));
  parent->load_dish ();
  close ();
}
