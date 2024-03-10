#ifndef HTTP_H
#define HTTP_H

#include <QEventLoop>
#include <QJsonDocument>

#include <QtNetwork/QNetworkAccessManager>

class Http : QObject
{
  Q_OBJECT

private:
  QEventLoop loop;
  QNetworkAccessManager nam;

public:
  Http (QObject *parent = nullptr) : QObject (parent)
  {
    connect (&nam, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
  }

  ~Http () = default;

public:
  static QNetworkRequest
  json_req (QString const &url)
  {
    QNetworkRequest req;
    req.setUrl (QUrl (url));
    req.setHeader (QNetworkRequest::ContentTypeHeader, "application/json");
    return req;
  }

  QNetworkReply *
  post (QString const &url, QJsonObject const &data)
  {
    auto const &req = json_req (url);

    QByteArray req_data = QJsonDocument (data).toJson (QJsonDocument::Compact);
    auto reply = nam.post (req, req_data);

    loop.exec ();
    return reply;
  }

public:
  template <typename Slot>
  static QNetworkAccessManager *
  post (QString const &url, QJsonObject const &data, QObject const *recv,
        Slot &&slot, QNetworkAccessManager *nam = nullptr)
  {
    auto const &req = json_req (url);

    if (!nam)
      {
        nam = new QNetworkAccessManager;
        connect (nam, &QNetworkAccessManager::finished, recv,
                 std::forward<Slot> (slot));
      }

    QByteArray req_data = QJsonDocument (data).toJson (QJsonDocument::Compact);
    auto reply = nam->post (req, req_data);
    return nam;
  }

  template <typename Slot>
  static QNetworkAccessManager *
  post (QString const &url, QJsonObject const &data, Slot &&slot,
        QNetworkAccessManager *nam = nullptr)
  {
    auto const &req = json_req (url);

    if (!nam)
      {
        nam = new QNetworkAccessManager;
        connect (nam, &QNetworkAccessManager::finished,
                 std::forward<Slot> (slot));
      }

    QByteArray req_data = QJsonDocument (data).toJson (QJsonDocument::Compact);
    auto reply = nam->post (req, req_data);
    return nam;
  }
};

#endif