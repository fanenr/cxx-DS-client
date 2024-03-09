#ifndef HOME_H
#define HOME_H

#include "ui_home.h"
#include "util.h"

class Log;

class Home : public QMainWindow
{
  Q_OBJECT
  friend class Log;

private:
  type typ = type::NONE;
  Ui::Home *ui = new Ui::Home;
  QMap<QString, QString> info;

public:
  Home (QWidget *parent, type typ);
  ~Home () { delete ui; }

public:
  void load_info ();

private:
  void closeEvent (QCloseEvent *event) override;
};

#endif
