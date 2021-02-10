#include <QFile>
#include <QGuiApplication>
#include <QQmlComponent>
#include <QQuickItem>
#include <QVector4D>
#include <QtQml/QQmlApplicationEngine>

#include "logic.h"
int main(int argc, char *argv[]) {
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  qRegisterMetaType<std::vector<QVector4D>>();
  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;
  engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
  if (engine.rootObjects().isEmpty())
    return -1;
  auto a = engine.findChild<QObject *>("window")->findChild<QObject *>("graph");
  Logic logic;
  return app.exec();
}
