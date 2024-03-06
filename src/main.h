#ifndef MAIN_H
#define MAIN_H

#include "ui_main.h"
#include "ui_reg.h"
#include <QButtonGroup>
#include <QMessageBox>

class Sreg : public QMainWindow
{
private:
  Q_OBJECT
  Ui::Reg *ui;

public:
  Sreg (QWidget *parent = nullptr);
  ~Sreg () { delete ui; }

public:
  void reg (QString const &user, QString const &pass);

private slots:
  void
  on_pbtn1_clicked ()
  {
    close ();
  }
};

class Mreg : public QMainWindow
{
private:
  Q_OBJECT
  Ui::Reg *ui;

public:
  Mreg (QWidget *parent = nullptr);
  ~Mreg () { delete ui; }

public:
  void reg (QString const &user, QString const &pass);

private slots:
  void
  on_pbtn1_clicked ()
  {
    close ();
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

inline Sreg::Sreg (QWidget *parent) : ui (new Ui::Reg)
{
  ui->setupUi (this);

  setWindowTitle (tr ("学生注册"));
  ui->label1->setText (tr ("学生注册"));
  ui->label2->setText (tr ("学号"));
  ui->label3->setText (tr ("昵称"));
  ui->label4->setText (tr ("电话"));
}

inline void
Sreg::reg (QString const &user, QString const &pass)
{
  if (isVisible ())
    return;
  show ();
}

inline Mreg::Mreg (QWidget *parent) : ui (new Ui::Reg)
{
  ui->setupUi (this);

  setWindowTitle (tr ("商户注册"));
  ui->label1->setText (tr ("商户注册"));
  ui->label2->setText (tr ("名称"));
  ui->label3->setText (tr ("电话"));
  ui->label4->setText (tr ("位置"));
}

inline void
Mreg::reg (QString const &user, QString const &pass)
{
  if (isVisible ())
    return;
  show ();
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

  auto const &user = ui->ledit1->text ();
  auto const &pass = ui->ledit2->text ();

  if (user.isEmpty () || pass.isEmpty ())
    {
      QMessageBox::warning (this, tr ("提示"), tr ("请输入帐号密码"));
      return;
    }

  switch (category ())
    {
    case 1:
      sreg.reg (user, pass);
      break;
    case 2:
      mreg.reg (user, pass);
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
