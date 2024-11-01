#include "serversetup.h"

#include <QCoreApplication>

#include <ve_eventhandler.h>
#include <vn_networksystem.h>
#include <vn_tcpsystem.h>
#include <vn_introspectionsystem.h>
#include <vs_storageeventsystem.h>
#include <vcmp_componentdata.h>
#include <vcmp_entitydata.h>
#include <vcmp_remoteproceduredata.h>
#include <ve_commandevent.h>

#include "rpctest.h"

namespace VFUnitTest
{
  ServerSetup::ServerSetup(QObject *t_parent) : QObject(t_parent)
  {
    m_evHandler = new VeinEvent::EventHandler(this);

    m_storSystem = new VeinStorage::StorageEventSystem(this);
    m_introspectionSystem = new VeinNet::IntrospectionSystem(this);
    m_netSystem = new VeinNet::NetworkSystem(this);
    m_tcpSystem = new VeinNet::TcpSystem(this);
    m_rpcTest = new RPCTest(this);

    m_netSystem->setOperationMode(VeinNet::NetworkSystem::VNOM_SUBSCRIPTION);

    m_subSystems.append(m_rpcTest);
    m_subSystems.append(m_storSystem);
    m_subSystems.append(m_introspectionSystem);
    m_subSystems.append(m_netSystem);
    m_subSystems.append(m_tcpSystem);


    m_evHandler->setSubsystems(m_subSystems);
  }

  ServerSetup::~ServerSetup()
  {
    m_evHandler->setSubsystems(QList<VeinEvent::EventSystem*>());
    delete m_evHandler;
    delete m_tcpSystem;
    delete m_netSystem;
    delete m_introspectionSystem;
    delete m_storSystem;
    delete m_rpcTest;
  }

  void ServerSetup::initData(int t_entityId, const QString &t_entityName)
  {
    QStringList componentList;

    //add the entity to the storage system
    VeinComponent::EntityData *eData = new VeinComponent::EntityData();
    eData->setCommand(VeinComponent::EntityData::Command::ECMD_ADD);
    eData->setEntityId(t_entityId);

    VeinEvent::CommandEvent *tmpEvent = new VeinEvent::CommandEvent(VeinEvent::CommandEvent::EventSubtype::NOTIFICATION, eData);
    QCoreApplication::postEvent(m_evHandler, tmpEvent);
    tmpEvent = 0;


    //set up the introspection
    VeinComponent::ComponentData *cData=nullptr;
    cData = new VeinComponent::ComponentData();
    cData->setEntityId(t_entityId);
    cData->setCommand(VeinComponent::ComponentData::Command::CCMD_ADD);
    cData->setComponentName(VeinNet::IntrospectionSystem::s_nameComponent);
    cData->setNewValue(t_entityName);

    tmpEvent = new VeinEvent::CommandEvent(VeinEvent::CommandEvent::EventSubtype::NOTIFICATION, cData);
    QCoreApplication::postEvent(m_evHandler, tmpEvent);
    tmpEvent = 0;


    //set up some components
    componentList << "testVariant";
    componentList << "testObject";

    const auto tmpComponentListCopy = componentList;
    for (const QString &componentName : tmpComponentListCopy)
    {
      cData = new VeinComponent::ComponentData();
      cData->setEntityId(t_entityId);
      cData->setCommand(VeinComponent::ComponentData::Command::CCMD_ADD);
      cData->setComponentName(componentName);
      cData->setNewValue(0);

      tmpEvent = new VeinEvent::CommandEvent(VeinEvent::CommandEvent::EventSubtype::NOTIFICATION, cData);
      QCoreApplication::postEvent(m_evHandler, tmpEvent);
      cData = 0;
    }

    //finally start the tcp server
    m_tcpSystem->startServer(8008);

    m_rpcTest->initData(m_evHandler, t_entityId);
    emit sigReady();
  }
}
