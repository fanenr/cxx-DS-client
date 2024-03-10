#ifndef HTTP_H
#define HTTP_H

#include <QMap>
#include <QtNetwork/QNetworkAccessManager>

class Http : QObject
{
  Q_OBJECT

private:
  QNetworkAccessManager nam;
  static inline QMap<qintptr, QNetworkAccessManager *> map;

public:
  Http () = default;
  ~Http () = default;

private:
  static QNetworkRequest json_req (QString const &url);

public:
  QNetworkReply *post (QString const &url, QJsonObject const &data);

public:
  template <typename Recv, typename Slot>
  static void post (QString const &url, QJsonObject const &data, Recv *recv,
                    Slot &&slot);

  template <typename Slot>
  static void post (QString const &url, QJsonObject const &data, Slot &&slot);

  static void finished (QNetworkReply *reply);
};

#endif