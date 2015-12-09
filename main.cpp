#include <QtQuickTest/quicktest.h>

#include <QStringList>
#include <QLoggingCategory>
#include <QTimer>

#include <qqml.h>
#include <veinqmlwrapper.h>

#include "serversetup.h"
#include "clientsetup.h"

int main(int argc, char **argv)
{
  QStringList loggingFilters = QStringList() << QString("*.debug=false");
  QLoggingCategory::setFilterRules(loggingFilters.join("\n"));
  QString categoryLoggingFormat = "%{if-debug}DD%{endif}%{if-warning}WW%{endif}%{if-critical}EE%{endif}%{if-fatal}FATAL%{endif} %{category} %{message}";
  qSetMessagePattern(categoryLoggingFormat);

  qmlRegisterType<ServerSetup>("ServerSetup", 1, 0, "ServerSetup");
  qmlRegisterType<ClientSetup>("ClientSetup", 1, 0, "ClientSetup");

  //register QML type
  VeinApiQml::QmlWrapper::registerTypes();

  return quick_test_main(argc, argv, "vein-framework unittest", QUICK_TEST_SOURCE_DIR);
}
