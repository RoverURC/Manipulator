#include <QCoreApplication>
#include <servodriver.h>
#include <manipulator.h>
int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
  Manipulator *myManipulator = new Manipulator(1502);
  myManipulator->startListening();
  return a.exec();
}
