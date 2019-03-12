#ifndef RPCTEST_H
#define RPCTEST_H

#include <ve_eventsystem.h>
#include <functional>

/// helper to save time with defining rpc function metadata
#define VF_RPC(rpcIdentifier, procedureNameString, procedureDescriptionString) \
  static constexpr QLatin1String s_##rpcIdentifier##ProcedureName = QLatin1String(procedureNameString); \
  static constexpr QLatin1String s_##rpcIdentifier##ProcedureDescription = QLatin1String(procedureDescriptionString);

/// helper to set up a map or hash of the remote procedures, mainly because qt creator breaks the indentation :(
#define VF_RPC_BIND(rpcIdentifier, rpcBind) \
{ \
  s_##rpcIdentifier##ProcedureName, \
  rpcBind \
  }

namespace VeinEvent
{
  using RemoteProcedureAtlas = QHash<QString, std::function<QVariantMap(QVariantMap)> >;
}

namespace VFUnitTest
{
  class RPCTest : public VeinEvent::EventSystem
  {
    Q_OBJECT
  public:
    explicit RPCTest(QObject *t_parent = nullptr);
    void initData(QObject *t_eventHandler, int t_entityId);

    // EventSystem interface
  public:
    bool processEvent(QEvent *t_event) override;

  private:
    VF_RPC(unitTestParamReturn, "unitTestParamReturn()", "unit test the rpc stuff")
    QVariantMap unitTestParamReturn(const QVariantMap &t_parameters);
    int m_entityId;
    const VeinEvent::RemoteProcedureAtlas m_remoteProcedures;


  };
}
#endif // RPCTEST_H
