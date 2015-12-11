#ifndef SERVERSETUP_H
#define SERVERSETUP_H

#include <QObject>

namespace VeinEvent {
  class EventHandler;
  class Validator;
  class EventSystem;
}

namespace VeinStorage {
  class VeinHash;
}

namespace VeinNet {
  class IntrospectionSystem;
  class NetworkSystem;
  class TcpSystem;
}

class ServerSetup : public QObject
{
  Q_OBJECT

public:
  explicit ServerSetup(QObject *t_parent = 0);
  ~ServerSetup();

signals:
  void sigReady();

public slots:
  void initData(int t_entityId, const QString &t_entityName);

private:
  VeinEvent::EventHandler *m_evHandler;

  VeinEvent::Validator *m_validator;
  VeinStorage::VeinHash *m_storSystem;
  VeinNet::IntrospectionSystem *m_introspectionSystem;
  VeinNet::NetworkSystem *m_netSystem;
  VeinNet::TcpSystem *m_tcpSystem;

  QList<VeinEvent::EventSystem*> m_subSystems;

};

#endif // SERVERSETUP_H
