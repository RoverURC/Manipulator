#ifndef LEDINDICATOR_H
#define LEDINDICATOR_H


#include <QObject>
#include <bcm2835.h>
#include <QTimer>
class LedIndicator : public QObject
{
  Q_OBJECT
public:
  explicit LedIndicator(QObject *parent = 0);

signals:

public slots:
  void startBlinking(int ms);
  void stopBlinking();
  void blink();
private:
  bool state;
  QTimer *timer;
};

#endif // LEDINDICATOR_H
