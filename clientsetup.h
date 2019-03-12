#ifndef CLIENTSETUP_H
#define CLIENTSETUP_H

#include <QObject>

namespace VeinEvent {
  class EventHandler;
  class EventSystem;
}

namespace VeinNet {
  class NetworkSystem;
  class TcpSystem;
}

namespace VeinApiQml {
  class VeinQml;
}

namespace VFUnitTest
{
  class ClientSetup : public QObject
  {
    Q_OBJECT
  public:
    explicit ClientSetup(QObject *t_parent = nullptr);
    ~ClientSetup();

  signals:
    void sigReady();

  public slots:
    void initData();

  private:
    VeinEvent::EventHandler* m_evHandler;
    VeinNet::NetworkSystem *m_netSystem;
    VeinNet::TcpSystem *m_tcpSystem;

    QList<VeinEvent::EventSystem*> m_subSystems;
  };
}

#endif // CLIENTSETUP_H
