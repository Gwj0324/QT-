#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <iostream>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->lineEdit->setReadOnly(true);//只读模式
    ui->lineEdit->move(QPoint(0,0));//设置lineEdit位置
    ui->lineEdit->setAlignment(Qt::AlignRight);//从右侧显示

    setWindowTitle(QString("简易计算器"));//设置标题
    setWindowIcon(QIcon(":/new/prefix1/icon/title.ico"));

    menu = new QMenu();

    m_ShowAction = new QAction(QString("显示"),menu);
    m_ExitAction = new QAction(QString("退出"),menu);

    menu->addAction(m_ShowAction);
    menu->addAction(m_ExitAction);

    m_SystemTrayIcon = new QSystemTrayIcon(this);
    m_SystemTrayIcon->setIcon(QIcon(":/new/prefix1/icon/title.ico"));
    m_SystemTrayIcon->setContextMenu(menu);
    m_SystemTrayIcon->show();

    connect(m_ShowAction,&QAction::triggered,this,&Widget::show);
    connect(m_ExitAction,&QAction::triggered,this,&Widget::m_close);
    connect(m_SystemTrayIcon,&QSystemTrayIcon::activated,this,&Widget::iconActivated);


}

Widget::~Widget()
{
    delete ui;
}

void Widget::getChar()
{
    ba=in.toLatin1();
}

void Widget::getSuffix()
{
  for(int i=0;i<ba.size();i++)
  {
      if((ba[i]>='0' && ba[i]<='9')||ba[i]=='.')//1,数字直接输出
      {
         QString str;
         QByteArray m;
         for(;(ba[i]>='0' && ba[i]<='9')||ba[i]=='.';i++)
         {
             //将一个数字存储到字节数组m中
             char x=ba[i];
             m.append(x);
         }
         //将字节数组m转换为QString，并将字符串输出
         str=QString(m);
         out.append(str);
         i--;
      }else if(s1.isEmpty())//2,遇见非数字时, 如果堆栈为空，则直接把该字符放入堆栈
      {
          s1.push(ba[i]);
      }else if(ba[i]=='+' || ba[i]=='-')
          //2.1,如果是+或- 那么遍历堆栈栈顶元素 一直输出优先级>=加减的 即输出栈顶的+-*/,
          //当遇到其他字符时（如左括号）或堆栈空时break
      {
          while(!s1.empty()){
              if(s1.top()=='+' || s1.top()=='-'||s1.top()=='*' || s1.top()=='/'){
                  //把字符转换为字符串输出
                  QString str=QString(s1.pop());
                  out.append(str);
               }else break;
          }
          s1.push(ba[i]);
      }else if(ba[i]=='*' || ba[i]=='/')
          //2.2,如果是*或/ 那么遍历堆栈栈顶元素 一直输出优先级>=乘除的 即输出栈顶的*/,
          //当遇到其他字符时（如左括号，减号，除号）或堆栈空时break
      {
          while(!s1.empty()){
              if(s1.top()=='*' || s1.top()=='/'){
                  //把字符转换为字符串输出
                  QString str=QString(s1.pop());
                  out.append(str);
               }else break;
          }
          s1.push(ba[i]);
      }else if(ba[i]==')')
          //2.3,如果该字符是右括号时，一直输出栈顶元素，直到遇见左括号为止break:
      {
          while(!s1.empty()){
              if(s1.top()=='('){
                  s1.pop();
                  break;
              }
              //把字符转换为字符串输出
              QString str=QString(s1.pop());
              out.append(str);
          }
      }else if(ba[i]=='(')
          //2.4,如果该字符是左括号时，直接放入堆栈
      {
          s1.push(ba[i]);
      }
  }
  //字符串循环结束后 再输出堆栈中剩余的数据
  while(!s1.empty()){
      //把字符转换为字符串输出
      QString str=QString(s1.pop());
      out.append(str);
  }
  //输出后缀表达式
  qDebug()<<out;
}

void Widget::Calc_Suffix()
{
  for(int i=0;i<out.size();i++)
  {
      bool is_Num =true;
      out[i].toFloat(&is_Num);
      if(is_Num)//如果是数字，直接入栈
      {
          s2.push(out[i].toFloat(&is_Num));
      }else if(out[i]=="+"||out[i]=="-"||out[i]=="*"||out[i]=="/")//如果是运算符，弹栈两次，并进行运算，运算后，结果入栈
      {
          float x=s2.pop();
          float y=s2.pop();
          if(out[i]=="+")
          {
              s2.push(y+x);
          }else if(out[i]=="-"){
              s2.push(y-x);
          }else if(out[i]=="*"){
              s2.push(y*x);
          }else if(out[i]=="/"){
              s2.push(y/x);
          }
      }
  }
  result = s2.pop();
  qDebug()<<result;
}

void Widget::m_init()
{
    ba.clear();
    out.clear();
}

void Widget::m_out()
{

}

void Widget::closeEvent(QCloseEvent *event)
{
    if(m_SystemTrayIcon->isVisible())
    {
        this->hide();
        event->ignore();
    }else{
        this->show();
    }
}

void Widget::on_pushButton_clicked()
{
    in=ui->lineEdit->text();//获取表达式
    getChar();
    getSuffix();
    Calc_Suffix();
    ui->lineEdit->clear();//清空文本编辑框
    ui->lineEdit->setText(QString("%1").arg(result));//显示结果
    m_init();
}

void Widget::on_pushButton_2_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg(0));
}

void Widget::on_pushButton_3_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg(1));
}

void Widget::on_pushButton_4_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg(2));
}

void Widget::on_pushButton_5_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg(3));
}

void Widget::on_pushButton_6_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg(4));
}

void Widget::on_pushButton_7_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg(5));
}

void Widget::on_pushButton_8_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg(6));
}

void Widget::on_pushButton_9_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg(7));
}

void Widget::on_pushButton_10_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg(8));
}

void Widget::on_pushButton_11_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg(9));
}

void Widget::on_pushButton_12_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("+"));
}

void Widget::on_pushButton_13_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("-"));
}

void Widget::on_pushButton_14_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("*"));
}

void Widget::on_pushButton_15_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("/"));
}

void Widget::on_pushButton_20_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("."));
}

void Widget::on_pushButton_18_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("("));
}

void Widget::on_pushButton_19_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg(")"));
}

void Widget::on_pushButton_16_clicked()
{
    //清空所有内容，并初始化
    ui->lineEdit->clear();
    m_init();
}

void Widget::on_pushButton_17_clicked()
{
    //退格删除一个字符
    ui->lineEdit->backspace();
}

void Widget::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason==QSystemTrayIcon::DoubleClick)
    {
        this->show();
    }
}

void Widget::m_close()
{
    m_SystemTrayIcon->hide();
    this->close();
}
