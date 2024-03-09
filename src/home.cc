#include "home.h"

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
