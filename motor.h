#ifndef MOTOR_H
#define MOTOR_H

#include <QObject>

class Motor : public QObject
{
  Q_OBJECT
public:
  explicit Motor(QObject *parent = 0);
  
  void setDirection(//wejscia//);
  void setSpeed();

signals:
  
public slots:

private:
    bool inA;
    bool inB;
    bool enA;
    bool enB;
    bool csDIsable;

  
};

#endif // MOTOR_H
