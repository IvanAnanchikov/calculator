#include "calculator.h"

Calculator::Calculator (QWidget *parent) : QWidget(parent) {
 lcd = new QLCDNumber(this);
 lcd->setMinimumSize(150, 50);
 lcd->setSegmentStyle(QLCDNumber::Flat);
 lcd->setDigitCount(11);
 lcd->setSmallDecimalPoint(true);
 label = new QLabel("");
 label->setMinimumSize (150, 50);
 label->setAlignment(Qt::AlignRight);
 display_str = new QLabel("");
 QChar keys[4][4] = {
  {'7', '8', '9', '/'},
  {'4', '5', '6', '*'},
  {'1', '2', '3', '-'},
  {'0', '.', '=', '+'}
 };
 QGridLayout *gLay = new QGridLayout;
 gLay->addWidget(lcd, 0, 0, 1, 4);
 gLay->addWidget(label, 1, 0, 1, 4);
 gLay->addWidget(addButton("<-"), 2, 0);
 gLay->addWidget(addButton("sqrt"), 2, 1);
 gLay->addWidget(addButton("1/x"), 2, 2);
 gLay->addWidget(addButton("C"), 2, 3);
 for (int i = 0; i < 4; ++i) {
  for (int j = 0; j < 4; ++j) {
   gLay->addWidget(addButton(keys[i][j]), i + 3, j);
  }
 }
 setLayout(gLay);
}

QPushButton* Calculator::addButton (const QString& str) {
 QPushButton* pcmd = new QPushButton(str);
 pcmd->setMinimumSize(40, 33);
 connect(pcmd, SIGNAL(clicked()), this, SLOT(slotButtonClicked()));
 return pcmd;
}

void Calculator::ButtonHandling(QString str) {
 QString text = display_str->text();
 QString text_lcd = QString::number(lcd->value());
 int len = text.length();
 QString last = "";
 if (len>0) last = text.right(1);
 if (((len==0 && stack.count()==0) ||//если еще ничего не вводили
        ((stack.count()==2 && len>1 && //если уже есть операнд и операция
        (last=="+"||last=="-"||last=="*"||last=="/")))) &&
        (str.contains(QRegExp("[0-9]")) || str=="-")) {//если первый операнд отрицательное число
    label->setText(text);
    text=str; //отобразить нажатый символ
    text_lcd=str;
 }
 else if ((text+str).contains(QRegExp("^-?[0-9]+\\.?[0-9]*$"))) {
    text+=str; //продолжаем вводить число
    text_lcd+=str;
 }
 else if (text.contains(QRegExp("^-?[0-9]+\\.?[0-9]*$"))) {
    if (str=="*"||str=="/"||str=="+"||str=="-"||str=="=") {
        if (stack.count()==2) { //если есть операнд и оператор
            stack.push(text);
            label->setText(label->text()+text+"=");
            calculate();
            text=display_str->text();
            text_lcd=lcd->value();
    }
    if (str!="=") {
            stack.push(text);//первый операнд в стек
            text+=str;//отобразить операцию
            text_lcd+=str;//операцию в стек
            stack.push(str);
            label->setText(text);
    }
  }
 }
 display_str->setText(text);
 lcd->display(text);

 //унарные операции
 if (str == "sqrt") {
     stack.push(text);
     label->setText(label->text()+text+"=");
     sqrt();
     text=display_str->text();
     text_lcd=lcd->value();
 }
 else if (str == "1/x") {
     stack.push(text);
     label->setText(label->text()+text+"=");
     X_1();
     text=display_str->text();
     text_lcd=lcd->value();
 }
 else if (str == "<-") {
     QString temp = QString::number(lcd->value());
     temp.chop(1);
     text = temp;
     text_lcd = temp;
     display_str->setText(text);
     lcd->display(text_lcd);
 }
}

void Calculator::calculate() {
 double nmb_2 = stack.pop().toDouble();
 QString Op = stack.pop();
 double nmb_1 = stack.pop().toDouble();
 double res = 0;
 if (Op == "+") { res = nmb_1 + nmb_2; }
 else if (Op == "-") { res = nmb_1 - nmb_2; }
 else if (Op == "/") { res = nmb_1 / nmb_2; }
 else if (Op == "*") { res = nmb_1 * nmb_2; }
 display_str->setText(QString("%1").arg(res, 0, 'f', 3));
 lcd->display(res);
}

void Calculator::sqrt() {
 double nmb = stack.pop().toDouble();
 double res = 0;
 res = nmb * nmb;
 display_str->setText(QString("%1").arg(res, 0, 'f', 3));
 lcd->display(res);
}

void Calculator::X_1() {
 double nmb = stack.pop().toDouble();
 double res = 0;
 if (nmb == 0){
     lcd->display("D BY 0");
 }
 else
 {
    res = 1 / nmb;
    display_str->setText(QString("%1").arg(res, 0, 'f', 3));
    lcd->display(res);
 }
}

void Calculator::clearAll() {
  stack.clear(); display_str->setText(""); label->setText(""); lcd->display(0);
}

void Calculator::slotButtonClicked() {
 QString str = ((QPushButton*)sender())->text();
 if (str == "C") clearAll();
 else ButtonHandling(str);
}

void Calculator::keyPressEvent(QKeyEvent *event) {
 int key=event->key();
 QString str = QString(QChar(key));
 if (key==Qt::Key_Delete) clearAll();
 else if (key==Qt::Key_C) clearAll();
 else ButtonHandling(str);
}
