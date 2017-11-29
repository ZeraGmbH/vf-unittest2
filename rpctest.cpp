#include "rpctest.h"
#include <vcmp_remoteproceduredata.h>
#include <vcmp_errordata.h>
#include <ve_commandevent.h>
#include <QCoreApplication>

RPCTest::RPCTest(QObject *t_parent) :
  VeinEvent::EventSystem(t_parent),
  m_remoteProcedures({ VF_RPC_BIND(unitTestParamReturn, std::bind(&RPCTest::unitTestParamReturn, this, std::placeholders::_1)) })
{
}

void RPCTest::initData(QObject *t_eventHandler, int t_entityId)
{
  m_entityId = t_entityId;
  VeinComponent::RemoteProcedureData * rpData = new VeinComponent::RemoteProcedureData();
  rpData->setEntityId(t_entityId);
  rpData->setCommand(VeinComponent::RemoteProcedureData::Command::RPCMD_REGISTER);
  rpData->setProcedureName(s_unitTestParamReturnProcedureName);

  QCoreApplication::postEvent(t_eventHandler, new VeinEvent::CommandEvent(VeinEvent::CommandEvent::EventSubtype::NOTIFICATION, rpData));
}

QVariantMap RPCTest::unitTestParamReturn(const QVariantMap &t_parameters)
{
  QVariantMap retVal = t_parameters;
  retVal.insert(VeinComponent::RemoteProcedureData::s_resultCodeString, 0);//result code OK
  return retVal;
}

bool RPCTest::processEvent(QEvent *t_event)
{
  bool retVal = false;
  if(t_event->type() == VeinEvent::CommandEvent::eventType())
  {
    VeinEvent::CommandEvent *cEvent = nullptr;
    cEvent = static_cast<VeinEvent::CommandEvent *>(t_event);
    Q_ASSERT(cEvent != nullptr);
    if(cEvent->eventData()->entityId() == m_entityId) //only our own entity is relevant
    {
      if(cEvent->eventData()->type() == VeinComponent::RemoteProcedureData::dataType())
      {
        VeinComponent::RemoteProcedureData *rpcData = nullptr;
        rpcData = static_cast<VeinComponent::RemoteProcedureData *>(cEvent->eventData());
        Q_ASSERT(rpcData != nullptr);
        if(rpcData->command() == VeinComponent::RemoteProcedureData::Command::RPCMD_CALL)
        {
          retVal = true;
          if(m_remoteProcedures.contains(rpcData->procedureName()))
          {
            retVal = true;
            VeinComponent::RemoteProcedureData *resultData = new VeinComponent::RemoteProcedureData();
            resultData->setCommand(VeinComponent::RemoteProcedureData::Command::RPCMD_RESULT);
            resultData->setEntityId(rpcData->entityId());
            resultData->setProcedureName(rpcData->procedureName());
            resultData->setEventOrigin(VeinEvent::EventData::EventOrigin::EO_LOCAL);
            resultData->setEventTarget(VeinEvent::EventData::EventTarget::ET_ALL);

            //execute the procedure and store the result as invokationData of the resultData
            resultData->setInvokationData(m_remoteProcedures.value(rpcData->procedureName())(rpcData->invokationData()));

            VeinEvent::CommandEvent *rpcResultEvent = new VeinEvent::CommandEvent(VeinEvent::CommandEvent::EventSubtype::NOTIFICATION, resultData);
            rpcResultEvent->setPeerId(cEvent->peerId());
            t_event->accept();
            emit sigSendEvent(rpcResultEvent);
          }
          else //unknown procedure
          {
            retVal = true;
            qDebug() << "No remote procedure with entityId:" << m_entityId << "name:" << rpcData->procedureName();
            VF_ASSERT(false, QStringC(QString("No remote procedure with entityId: %1 name: %2").arg(m_entityId).arg(rpcData->procedureName())));
            VeinComponent::ErrorData *eData = new VeinComponent::ErrorData();
            eData->setEntityId(m_entityId);
            eData->setErrorDescription(QString("No remote procedure with name: %1").arg(rpcData->procedureName()));
            eData->setOriginalData(rpcData);
            eData->setEventOrigin(VeinEvent::EventData::EventOrigin::EO_LOCAL);
            eData->setEventTarget(VeinEvent::EventData::EventTarget::ET_ALL);
            VeinEvent::CommandEvent *errorEvent = new VeinEvent::CommandEvent(VeinEvent::CommandEvent::EventSubtype::NOTIFICATION, eData);
            errorEvent->setPeerId(cEvent->peerId());
            cEvent->accept();
            emit sigSendEvent(errorEvent);
          }
        }
      }
    }
  }
  return retVal;
}

constexpr QLatin1String RPCTest::s_unitTestParamReturnProcedureName;
constexpr QLatin1String RPCTest::s_unitTestParamReturnProcedureDescription;
