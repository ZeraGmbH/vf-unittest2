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
  qDebug() << QString("Qt version: %1").arg(qVersion()) << QString("GCC version: %1.%2.%3").arg(__GNUC__).arg(__GNUC_MINOR__).arg(__GNUC_PATCHLEVEL__);
  QStringList loggingFilters = QStringList() << QString("*.debug=false");
  QLoggingCategory::setFilterRules(loggingFilters.join("\n"));
  QString categoryLoggingFormat = "%{if-debug}DD%{endif}%{if-warning}WW%{endif}%{if-critical}EE%{endif}%{if-fatal}FATAL%{endif} %{category} %{message}";
  qSetMessagePattern(categoryLoggingFormat);

  qmlRegisterType<ServerSetup>("TestSetup", 1, 0, "ServerSetup");
  qmlRegisterType<ClientSetup>("TestSetup", 1, 0, "ClientSetup");

  return quick_test_main(argc, argv, "vein-framework unittest", QUICK_TEST_SOURCE_DIR);
}
