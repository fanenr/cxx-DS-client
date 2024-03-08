#include "http.h"

#include <QEventLoop>
#include <QJsonDocument>

QNetworkReply *
Http::post (QString const &url, QJsonObject const &data)
{
  QNetworkRequest req;
  req.setUrl (QUrl (url));
  req.setHeader (QNetworkRequest::ContentTypeHeader, "application/json");

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
  QNetworkRequest req;
  req.setUrl (QUrl (url));
  req.setHeader (QNetworkRequest::ContentTypeHeader, "application/json");

  auto nam = new QNetworkAccessManager;
  connect (nam, &QNetworkAccessManager::finished, recv,
           std::forward<Slot> (slot));

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
