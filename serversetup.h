#ifndef SERVERSETUP_H
#define SERVERSETUP_H

#include <QObject>

namespace VeinEvent {
  class EventHandler;
  class Validator;
  class EventSystem;
}

namespace VeinStorage {
  class StorageEventSystem;
}

namespace VeinNet {
  class IntrospectionSystem;
  class NetworkSystem;
  class TcpSystem;
}

namespace VFUnitTest
{
  class RPCTest;

  class ServerSetup : public QObject
  {
    Q_OBJECT

  public:
    explicit ServerSetup(QObject *t_parent = nullptr);
    ~ServerSetup();

  signals:
    void sigReady();

  public slots:
    void initData(int t_entityId, const QString &t_entityName);

  private:
    VeinEvent::EventHandler *m_evHandler;

    VeinStorage::StorageEventSystem *m_storSystem;
    VeinNet::IntrospectionSystem *m_introspectionSystem;
    VeinNet::NetworkSystem *m_netSystem;
    VeinNet::TcpSystem *m_tcpSystem;
    RPCTest *m_rpcTest;

    QList<VeinEvent::EventSystem*> m_subSystems;

  };
}

#endif // SERVERSETUP_H
