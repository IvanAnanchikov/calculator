
#ifndef _Calculator_h_
#define _Calculator_h_
#include <QWidget>
#include <QStack>
#include <QLCDNumber>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QTCore/qmath.h>
#include <QKeyEvent>
#include <Qt>

 class Calculator : public QWidget {
  Q_OBJECT
  private:
   QLCDNumber *lcd;
   QLabel *label;
   QLabel *display_str;
   QStack <QString> stack;
 protected:
    virtual void keyPressEvent(QKeyEvent *event);
  public:
   Calculator (QWidget* pwgt = 0);
   QPushButton* addButton (const QString& str);
   void clearAll ();
   void sqrt();
   void X_1();
   void calculate ();
   void ButtonHandling(QString);
  public slots:
   void slotButtonClicked ();
 };
#endif
