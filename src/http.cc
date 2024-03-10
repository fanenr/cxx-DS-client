#include "http.h"

#include <QEventLoop>
#include <QJsonDocument>

QNetworkRequest
json_req (QString const &url)
{
  QNetworkRequest req;
  req.setUrl (QUrl (url));
  req.setHeader (QNetworkRequest::ContentTypeHeader, "application/json");
  return req;
}

QNetworkReply *
Http::post (QString const &url, QJsonObject const &data)
{
  auto const &req = json_req (url);

  QByteArray req_data = QJsonDocument (data).toJson (QJsonDocument::Compact);
  auto reply = nam.post (req, req_data);

  QEventLoop loop;
  connect (&nam, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

  loop.exec ();
  return reply;
}

template <typename Recv, typename Slot>
void
Http::post (QString const &url, QJsonObject const &data, Recv *recv,
            Slot &&slot)
{
  auto const &req = json_req (url);

  auto nam = new QNetworkAccessManager;
  connect (nam, &QNetworkAccessManager::finished, recv,
           std::forward<Slot> (slot));

  QByteArray req_data = QJsonDocument (data).toJson (QJsonDocument::Compact);
  auto reply = nam->post (req, req_data);

  map.insert ((qintptr)reply, nam);
}

template <typename Slot>
void
Http::post (QString const &url, QJsonObject const &data, Slot &&slot)
{
  auto const &req = json_req (url);

  auto nam = new QNetworkAccessManager;
  connect (nam, &QNetworkAccessManager::finished, std::forward<Slot> (slot));

  QByteArray req_data = QJsonDocument (data).toJson (QJsonDocument::Compact);
  auto reply = nam->post (req, req_data);

  map.insert ((qintptr)reply, nam);
}

void
Http::finished (QNetworkReply *reply)
{
  auto iter = map.find ((qintptr)reply);
  delete *iter;
  map.erase (iter);
}
