#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_main.h"
#include <QButtonGroup>
#include <QMessageBox>

class Main : public QMainWindow
{
private:
  Q_OBJECT
  Ui::Main *ui;
  QButtonGroup *btns;

public:
  explicit Main (QWidget *parent = nullptr)
      : QMainWindow (parent), ui (new Ui::Main)
  {
    ui->setupUi (this);

    btns = new QButtonGroup (this);
    btns->addButton (ui->rbtn, 1);
    btns->addButton (ui->rbtn2, 2);
  }

  ~Main () { delete ui; }

private:
  int
  category ()
  {
    if (ui->rbtn->isChecked ())
      return 1;
    if (ui->rbtn2->isChecked ())
      return 2;
    return 0;
  }

public slots:
  void
  on_pbtn2_clicked ()
  {
    QMessageBox::information (this, tr ("登录"), tr ("显示内容"));
  }
};

#endif
