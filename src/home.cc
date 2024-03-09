#include "home.h"
#include "mod.h"
#include "util.h"

Home::Home (QWidget *parent, type typ) : QMainWindow (parent), typ (typ)
{
  ui->setupUi (this);

  switch (typ)
    {
    case type::STUDENT:
      ui->hint3->setText ("学号: ");
      break;
    case type::MERCHANT:
      ui->hint3->setText ("位置: ");
      break;
    default:
      break;
    }
}

void
Home::load_info ()
{
  ui->info1->setText (info["name"]);
  ui->info2->setText (info["user"]);
  ui->info4->setText (info["number"]);

  switch (typ)
    {
    case type::STUDENT:
      ui->info3->setText (info["id"]);
      break;
    case type::MERCHANT:
      ui->info3->setText (info["position"]);
      break;
    default:
      break;
    }
}

void
Home::closeEvent (QCloseEvent *event)
{
  QMainWindow::closeEvent (event);
  deleteLater ();
}

void
Home::on_pbtn1_clicked ()
{
  Mod *mod;
  static Mod *smod;
  static Mod *mmod;

  switch (typ)
    {
    case type::STUDENT:
      if (!smod)
        smod = new Mod (this, type::STUDENT, info);
      mod = smod;
      break;
    case type::MERCHANT:
      if (!mmod)
        mmod = new Mod (this, type::MERCHANT, info);
      mod = mmod;
      break;
    default:
      break;
    }

  if (mod->isVisible ())
    return;

  mod->load_old ();
  mod->show ();
}

void
Home::on_pbtn2_clicked ()
{
  close ();
}
