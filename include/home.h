#ifndef HOME_H
#define HOME_H

#include "ui_home.h"
#include "util.h"

class Mod;
class New;
class Eva;
class Log;

class Home : public QMainWindow
{
  Q_OBJECT
  friend class Log;
  friend class Mod;
  friend class New;
  friend class Eva;

private:
  type typ;
  Log *page_log = nullptr;
  Mod *page_mod = nullptr;
  New *page_new = nullptr;
  Eva *page_eva = nullptr;
  Ui::Home *ui = new Ui::Home;
  QMap<QString, QString> info;

public:
  Home ();
  ~Home () { delete ui; }

public:
  void init_ui ();
  void load_info ();
  void load_dish ();

private:
  void item_selected (QListWidgetItem *item, QListWidgetItem *prev);

private slots:
  void on_pbtn1_clicked ();
  void on_pbtn2_clicked ();
  void on_pbtn3_clicked ();
  // void on_pbtn4_clicked ();
  void on_pbtn5_clicked ();
  void on_pbtn6_clicked ();
};

#endif
