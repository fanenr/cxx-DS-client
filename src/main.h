#ifndef MAIN_H
#define MAIN_H

#include "ui_main.h"
#include "ui_mreg.h"
#include "ui_sreg.h"
#include <QButtonGroup>
#include <QMessageBox>

class Sreg : public QMainWindow
{
private:
  Q_OBJECT
  Ui::Sreg *ui;

public:
  Sreg (QWidget *parent = nullptr) : ui (new Ui::Sreg) { ui->setupUi (this); }
  ~Sreg () { delete ui; }

public:
  void reg (QString const &user, QString const &pass);

private slots:
  void
  on_pbtn1_clicked ()
  {
    this->close ();
  }
};

class Mreg : public QMainWindow
{
private:
  Q_OBJECT
  Ui::Mreg *ui;

public:
  Mreg (QWidget *parent = nullptr) : ui (new Ui::Mreg) { ui->setupUi (this); }
  ~Mreg () { delete ui; }

public:
  void reg (QString const &user, QString const &pass);

private slots:
  void
  on_pbtn1_clicked ()
  {
    this->close ();
  }
};

class Main : public QMainWindow
{
private:
  Q_OBJECT
  Ui::Main *ui;
  QButtonGroup *btns;

public:
  explicit Main (QWidget *parent = nullptr);
  ~Main () { delete ui; }

private:
  int category ();

private slots:
  void on_pbtn1_clicked ();
  void on_pbtn2_clicked ();
};

inline Main::Main (QWidget *parent) : QMainWindow (parent), ui (new Ui::Main)
{
  ui->setupUi (this);

  btns = new QButtonGroup (this);
  btns->addButton (ui->rbtn1, 1);
  btns->addButton (ui->rbtn2, 2);
}

inline void
Sreg::reg (QString const &user, QString const &pass)
{
  if (user.isEmpty () || pass.isEmpty ())
    return;

  if (this->isVisible ())
    return;

  this->show ();
}

inline void
Mreg::reg (QString const &user, QString const &pass)
{
  if (user.isEmpty () || pass.isEmpty ())
    return;

  if (this->isVisible ())
    return;

  this->show ();
}

inline int
Main::category ()
{
  if (ui->rbtn1->isChecked ())
    return 1;
  if (ui->rbtn2->isChecked ())
    return 2;
  return 0;
}

inline void
Main::on_pbtn1_clicked ()
{
  static Sreg sreg;
  static Mreg mreg;

  switch (category ())
    {
    case 1:
      sreg.reg (ui->ledit1->text (), ui->ledit2->text ());
      break;
    case 2:
      mreg.reg (ui->ledit1->text (), ui->ledit2->text ());
      break;
    default:
      break;
    }
}

inline void
Main::on_pbtn2_clicked ()
{
  switch (category ())
    {
    case 1:
      break;
    case 2:
      break;
    default:
      break;
    }
}

#endif
