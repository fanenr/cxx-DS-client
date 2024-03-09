#include "mod.h"
#include "qmainwindow.h"
#include "util.h"

Mod::Mod (QMainWindow *parent, type typ, decltype (old) info)
    : QMainWindow (parent), typ (typ), old (info)
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
}

void
Mod::on_pbtn3_clicked ()
{
}
