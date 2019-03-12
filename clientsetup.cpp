#include "clientsetup.h"

#include <ve_eventhandler.h>
#include <vn_networksystem.h>
#include <vn_tcpsystem.h>
#include <vn_networkstatusevent.h>
#include <veinqml.h>
#include <veinqmlwrapper.h>

namespace VFUnitTest
{
  ClientSetup::ClientSetup(QObject *t_parent) : QObject(t_parent)
  {
    m_evHandler = new VeinEvent::EventHandler();
    m_netSystem = new VeinNet::NetworkSystem();
    m_tcpSystem = new VeinNet::TcpSystem();

    VeinApiQml::VeinQml::setStaticInstance(new VeinApiQml::VeinQml()); // owned/deleted by QML engine

    m_netSystem->setOperationMode(VeinNet::NetworkSystem::VNOM_PASS_THROUGH);


    QObject::connect(m_tcpSystem, &VeinNet::TcpSystem::sigSendEvent, [&](QEvent *t_event){
      if(t_event->type()==VeinNet::NetworkStatusEvent::getEventType())
      {
        qCritical() << "Aborting: Network failed";
      }
    });

    m_subSystems.append(m_netSystem);
    m_subSystems.append(m_tcpSystem);
    m_subSystems.append(VeinApiQml::VeinQml::getStaticInstance());

    m_evHandler->setSubsystems(m_subSystems);
  }

  ClientSetup::~ClientSetup()
  {
    m_evHandler->setSubsystems(QList<VeinEvent::EventSystem*>());
    delete m_evHandler;
    delete m_tcpSystem;
    delete m_netSystem;
    VeinApiQml::VeinQml::getStaticInstance()->deleteLater();
  }

  void ClientSetup::initData()
  {
    m_tcpSystem->connectToServer("127.0.0.1", 8008);
    QObject::connect(m_tcpSystem, &VeinNet::TcpSystem::sigConnnectionEstablished, this, &ClientSetup::sigReady);
  }
}
