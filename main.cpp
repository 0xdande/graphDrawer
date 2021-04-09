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
  qRegisterMetaType<std::vector<QMatrix4x4>>();
  QGuiApplication app(argc, argv);

  qRegisterMetaType<std::vector<QVector4D>>();
  qRegisterMetaType<std::vector<QMatrix4x4>>();

  QQmlApplicationEngine engine;
  engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
  if (engine.rootObjects().isEmpty()) return -1;
  Logic logic;
  return app.exec();
}
