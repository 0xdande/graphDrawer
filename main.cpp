#include <QFile>
#include <QGuiApplication>
#include <QQmlComponent>
#include <QQuickItem>
#include <QVector4D>
#include <QtQml/QQmlApplicationEngine>

#include "logic.h"
int main(int argc, char* argv[]) {
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  qRegisterMetaType<std::vector<QVector4D>>();
  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;
  engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
  if (engine.rootObjects().isEmpty()) return -1;
  auto a = engine.findChild<QObject*>("window")->findChild<QObject*>("graph");
  //  qDebug() << a->property("width");
  Logic logic;
  //  QMetaObject::invokeMethod(logic.canvas, "drawCircle", Q_ARG(int, 10),
  //                            Q_ARG(int, 10));
  /*qDebug() << engine.rootObjects();
  QQmlComponent component(&engine, QUrl(QStringLiteral("qrc:/canvas.qml")));
  if (component.isError()) {
    qWarning() << "QmlItemCreator::createQmlObject The QMLComponent for "
               << " has errors: " << component.errorString();
  } else if (component.isReady()) {
    QObject* object = component.create();
    QQuickItem* item = qobject_cast<QQuickItem*>(object);

    object->setParent(engine.rootObjects()[0]);
    item->setParentItem(qobject_cast<QQuickItem*>(engine.rootObjects()[0]));
    object->setProperty("parent",
                        QVariant::fromValue<QObject*>(engine.rootObjects()[0]));
    qDebug() << object->parent();
    object->setProperty("width", object->parent()->property("width"));
    object->setProperty("height", object->parent()->property("height"));
    //                            Q_ARG(int, 100));
    qDebug() << object->property("width");
    qDebug() << object->property("height");
    QMetaObject::invokeMethod(object, "requestPaint");
    QMetaObject::invokeMethod(object, "requestPaint");
    QMetaObject::invokeMethod(object, "requestPaint");
  }
  //  logic.engine_ = engine;
  //  qDebug() << engine;
  */
  return app.exec();
}
