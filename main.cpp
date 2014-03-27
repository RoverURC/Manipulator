#include <QCoreApplication>
#include <modbusserver.h>
#include <servodriver.h>
#include <QObject>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ModbusServer *myModbusServer = new ModbusServer(1502);
    myModbusServer->startListening();

    ServoDriver *myServoDrive = new ServoDriver(PCA9685_WRITE);

    QObject::connect(myModbusServer,SIGNAL(registerChanged(int , quint16)),myServoDrive,SLOT(interpretChangedRegister(int , quint16)));
    return a.exec();
}
