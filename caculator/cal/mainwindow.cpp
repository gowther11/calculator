#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>
#include <QtCore/qmath.h>
#include <qdebug.h>
#include "qpixmap.h"
#include "qpainter.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindow::ui->display->setAlignment(Qt::AlignRight);
    MainWindow::ui->res_disp->setAlignment(Qt::AlignRight);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute((Qt::WA_TranslucentBackground));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::paintEvent(QPaintEvent*)
{    QPixmap pixmap = QPixmap("D:/caculator/cal/bd.jpg").scaled(this->size());
    QPainter painter(this);
    painter.drawPixmap(this->rect(), pixmap);
}
//判断算式是否合法
bool ifinputvaild(QString disp)
{
    int cleft=0;
    int dotcnt=0;
    int contobj=0;
    int acon=0;
    int leg=disp.length();
    if(disp[0]=="*"||disp[0]=="/"||disp[0]==")"||disp[0]=="^")
    {
        return false;
    }
    for(int i=0;i<leg;i++)
    {
        if(disp[i]=="(")                                          /*左括号的情况*/
        { cleft++;
            if(disp[i+1]==")"){return false;}
            if(i>0)
            {
                if(disp[i-1]!="+"&&disp[i-1]!="-"&&disp[i-1]!="*"&&disp[i-1]!="/"&&disp[i-1]!="^")
                    return false;
            }
        }
        else
        {
            if(disp[i]==")")
            {
                cleft--;

                if(i<leg-1){
                if(disp[i+1]!="+"&&disp[i+1]!="-"&&disp[i+1]!="*"&&disp[i+1]!="/"&&disp[i+1]!=")"&&disp[i+1]!="^")
                {return false;}}
            }
            else
                {
                    if(disp[i]==".")
                    {
                        dotcnt++;
                        if(dotcnt>1){return false;}
                        acon++;
                        if(acon>1){return false;}
                    }
                    else{
                            if(disp[i]=="+"||disp[i]=="-"||disp[i]=="*"||disp[i]=="/"||disp[i]=="^")
                            {
                                contobj++;
                                if(dotcnt>0){dotcnt--;}
                                if(contobj>1){return false;}
                                if(i>0){
                                if(disp[i-1]!=")"){acon++;}
                                if(disp[i-1]=="("&&disp[i]=="-"){acon--;}
                                }
                                else{acon++;}
                                if(acon>1){return false;}
                             }
                             else
                                {if(disp[i]>="0"&&disp[i]<="9")
                                    if(contobj>0)
                                    {
                                        contobj--;

                                    }
                                    if(acon>0)
                                    {
                                        acon--;
                                    }


            }

    }
}
        }
}
    if(cleft<0||contobj!=0)
    {return false;}

    return true;
}

//补充修饰算式
QString modify(QString oin)
{
   int cleft=0;
   if(oin[0]=="-"||oin[0]==".")
   {
       oin = "0"+oin;
   }
   for (int i=0 ; i<oin.length() ; i++)
   {
       if(oin[i]=="(")
       {   cleft++;
           if(i!=0)
           {
               if(oin[i-1]>="0"&&oin[i-1]<="9")
               {
                   oin.insert(i,"*");
               }
           }
       }
       else if(oin[i]==".")
       {
           if(oin[i-1]<"0"||oin[i-1]>"9")
           {
               oin.insert(i,QString("0"));
               continue;
           }

           if(i<oin.length()-1){
           if(oin[i+1]<"0"||oin[i+1]>"9")
           {
               oin.remove(i,1);
           }}
           else if(i==oin.length()-1){oin.remove(i,1);}
       }
       else if(oin[i]=="-")
       {
           if(oin[i-1]=="(")
           {
               oin.insert(i,"0");
               continue;
           }
       }
       else if(oin[i]==")"){cleft--;}

   }
   while(cleft!=0)
   {
       oin = oin+")";
       cleft--;
   }
   return oin;
}


//运算符优先级
int GetPriOfOpe(QChar ope, int sch)
{
    if(sch==0)
    {
       if(ope=='+'||ope=='-')
       {
           return 4;
       }
       else
       {
           if(ope=='*'||ope=='/')
           {
               return 3;
           }
           else
           {
               if(ope=='('){return 1;}
               else{if(ope=='^'){return 2;}}
           }
       }
    }
    else
    {
        if(ope=='+'||ope=='-')
        {
            return 2;
        }
        else
        {
            if(ope=='*'||ope=='/')
            {
                return 3;
            }
            else
            {
                if(ope=='(')
                {
                    return 5;
                }
                else
                {
                    if(ope==')'){return 1;}
                    else{if(ope=='^'){return 4;}}
                }
            }
        }
    }
}


//获取后缀表达式
int GetPostExp(QString exp_str,QString pos_exp[])
{

    int pos_exp_top = 0;

    QChar opes_temp[999];       //存放临时符号栈
    int opes_temp_top = 0;
    opes_temp[opes_temp_top++] = QChar('#');

    int length = exp_str.size();

    for(int i = 0;i<length;++i)
    {
        if(exp_str[i].isDigit())    //提取数字串
        {
            int beg = i;
            while(i<length)
            {
                if(exp_str[i].isDigit() || exp_str[i] == '.')
                    ++i;
                else
                    break;
            }
            pos_exp[pos_exp_top++] = exp_str.mid(beg,i-beg);

            i = i-1;

        }
        else       //提取运算符串
       {
            int pri_i = GetPriOfOpe(exp_str[i],1);      //入栈优先级

            while(opes_temp_top >= 0)
            {
                QChar ch_top = opes_temp[opes_temp_top-1];
                int pri_j = GetPriOfOpe(ch_top,0);      //出栈优先级

                if(pri_j >= pri_i)
                {
                     --opes_temp_top;        //栈递减

                    if(ch_top != '(')
                        pos_exp[pos_exp_top++] = QString(ch_top);       //入后缀表达式
                    else
                         break;      //右括号匹配到左括号，退出
                }
                else
                    break;
            }
            if(exp_str[i] != ')')
                opes_temp[opes_temp_top++] = exp_str[i];        //入暂时栈
       }

     }

    //将剩余的栈中元素装入后缀表达式
    for(int i = opes_temp_top-1;i>0;--i)
         pos_exp[pos_exp_top++] = QString(opes_temp[i]);
    return pos_exp_top;
}

//根据字符串计算数值
double CalFromStr(QString exp_str)
{


    exp_str = modify(exp_str);
    QString pos_exp[999];
    int num = GetPostExp(exp_str,pos_exp);

    double nums[num];
    int nums_top = 0;
    for(int i = 0;i<num;++i)
    {
        QString temp = pos_exp[i];

        if(temp[0].isDigit() == true)       //数字
        {
            nums[nums_top++] = temp.toDouble();
        }
        else
        {
            //出栈两个运算数
            double sec_num = nums[--nums_top];
            double fir_num = nums[--nums_top];
            double ans = 0;

            if(temp[0] == '+')
            {
                ans = fir_num+sec_num;
            }
            else if(temp[0] == '-')
            {
                  ans = fir_num- sec_num;
            }
            else if(temp[0] == '*')
              {
                 ans = fir_num*sec_num;
            }
            else if(temp[0] == '/')
            {
                 ans = fir_num/sec_num;
            }
            else if(temp[0] == '^')
            {
                 ans = pow(fir_num,sec_num);
            }

             nums[nums_top++] = ans;     //计算后结果入栈
        }
    }

    return nums[0];

}

void MainWindow::on_pushButton_clicked()
{
    MainWindow::ui->sw->setCurrentIndex(0);
}

void MainWindow::on_pushButton_phy_clicked()
{
    MainWindow::ui->sw->setCurrentIndex(1);
}

void MainWindow::on_pushButton_rt_clicked()
{
    MainWindow::ui->sw->setCurrentIndex(2);
}


void MainWindow::on_pushButton_bk_clicked()
{
    MainWindow::ui->sw->setCurrentIndex(2);
}

/*num*/

void MainWindow::on_pushButton_1_clicked()
{
    QString str = MainWindow::ui->display->text();
    MainWindow::ui->display-> setText(str+"1");
}

void MainWindow::on_pushButton_2_clicked()
{
    QString str = MainWindow::ui->display->text();
    MainWindow::ui->display-> setText(str+"2");
}

void MainWindow::on_pushButton_3_clicked()
{
    QString str = MainWindow::ui->display->text();
    MainWindow::ui->display-> setText(str+"3");
}

void MainWindow::on_pushButton_4_clicked()
{
    QString str = MainWindow::ui->display->text();
    MainWindow::ui->display-> setText(str+"4");
}

void MainWindow::on_pushButton_5_clicked()
{
    QString str = MainWindow::ui->display->text();
    MainWindow::ui->display-> setText(str+"5");
}

void MainWindow::on_pushButton_6_clicked()
{
    QString str = MainWindow::ui->display->text();
    MainWindow::ui->display-> setText(str+"6");
}

void MainWindow::on_pushButton_7_clicked()
{
    QString str = MainWindow::ui->display->text();
    MainWindow::ui->display-> setText(str+"7");
}

void MainWindow::on_pushButton_8_clicked()
{
    QString str = MainWindow::ui->display->text();
    MainWindow::ui->display-> setText(str+"8");
}

void MainWindow::on_pushButton_9_clicked()
{
    QString str = MainWindow::ui->display->text();
    MainWindow::ui->display-> setText(str+"9");
}

void MainWindow::on_pushButton_0_clicked()
{
    QString str = MainWindow::ui->display->text();
    MainWindow::ui->display-> setText(str+"0");
}

//   +-*/. ^  //

void MainWindow::on_pushButton_plus_clicked()
{
    QString str = MainWindow::ui->display->text();
    MainWindow::ui->display-> setText(str+"+");
}

void MainWindow::on_pushButton_minus_clicked()
{
    QString str = MainWindow::ui->display->text();
    MainWindow::ui->display-> setText(str+"-");
}

void MainWindow::on_pushButton_mult_clicked()
{
    QString str = MainWindow::ui->display->text();
    MainWindow::ui->display-> setText(str+"*");
}

void MainWindow::on_pushButton_divd_clicked()
{
    QString str = MainWindow::ui->display->text();
    MainWindow::ui->display-> setText(str+"/");
}

void MainWindow::on_pushButton_dot_clicked()
{
    QString str = MainWindow::ui->display->text();
    MainWindow::ui->display-> setText(str+".");
}

void MainWindow::on_pushButton_nega_clicked()
{
    QString str = MainWindow::ui->display->text();
    MainWindow::ui->display-> setText(str+"^");
}




//del ac m //
void MainWindow::on_pushButton_del_clicked()
{
    QString str = MainWindow::ui->display->text();
    str.remove(str.length()-1,str.length());
    MainWindow::ui->display-> setText(str);
}

void MainWindow::on_pushButton_ac_clicked()
{
    MainWindow::ui->display-> setText("");
}


void MainWindow::on_Memoy_clicked()
{
    QString str = MainWindow::ui->display->text();
     MainWindow::ui->display-> setText(str+memory);
}


// () //
void MainWindow::on_pushButton_left_clicked()
{
    QString str = MainWindow::ui->display->text();
    MainWindow::ui->display-> setText(str+"(");
}


void MainWindow::on_pushButton_right_clicked()
{
    QString str = MainWindow::ui->display->text();
    MainWindow::ui->display-> setText(str+")");
}


//等号运算
void MainWindow::on_pushButton_eq_clicked()
{
    QString str = MainWindow::ui->display->text();
    str = modify(str);
    bool b = ifinputvaild(str);
    if(b==false)
   { MainWindow::ui->display-> setText("Error：算式书写错误");}
    else
    {   QString res = QString::number(CalFromStr(str), 'f', 4);
        int i=res.length()-1;
        while(res[i]=='0')
        {
            res.remove(i,1);
            i--;
        }
        if(res[i]=='.'){res.remove(i,1);}
        //计算以及清除多余小数

        memory = res;
        MainWindow::ui->res_disp-> setText(res);
}
}



//三角函数

void MainWindow::on_pushButton_tan_clicked()
{
    QString str = MainWindow::ui->display->text();
    str = modify(str);
    bool b = ifinputvaild(str);
    if(b==false)
   { MainWindow::ui->display-> setText("Error：算式书写错误");}
    else
    {  double tmp_num = CalFromStr(str);
       double num_res = tan(tmp_num);
        //tan计算
        QString res = QString::number(num_res, 'f', 4);
        int i=res.length()-1;
        while(res[i]=='0')
        {
            res.remove(i,1);
            i--;
        }
        if(res[i]=='.'){res.remove(i,1);}

        //输入到M并输出
        memory = res;
        MainWindow::ui->res_disp-> setText("tan("+str+")= "+res);
}
}

void MainWindow::on_pushButton_cos_clicked()
{
    QString str = MainWindow::ui->display->text();
    str = modify(str);
    bool b = ifinputvaild(str);
    if(b==false)
   { MainWindow::ui->display-> setText("Error：算式书写错误");}
    else
    {  double tmp_num = CalFromStr(str);
       double num_res = cos(tmp_num);
        //cos计算
        QString res = QString::number(num_res, 'f', 4);
        int i=res.length()-1;
        while(res[i]=='0')
        {
            res.remove(i,1);
            i--;
        }
        if(res[i]=='.'){res.remove(i,1);}

        //输入到M并输出
        memory = res;
        MainWindow::ui->res_disp-> setText("cos("+str+")= "+res);
}
}

void MainWindow::on_pushButton_sin_clicked()
{
    QString str = MainWindow::ui->display->text();
    str = modify(str);
    bool b = ifinputvaild(str);
    if(b==false)
   { MainWindow::ui->display-> setText("Error：算式书写错误");}
    else
    {  double tmp_num = CalFromStr(str);
       double num_res = sin(tmp_num);
        //sin计算
        QString res = QString::number(num_res, 'f', 4);
        int i=res.length()-1;
        while(res[i]=='0')
        {
            res.remove(i,1);
            i--;
        }
        if(res[i]=='.'){res.remove(i,1);}

        //输入到M并输出
        memory = res;
        MainWindow::ui->res_disp-> setText("sin("+str+")= "+res);
}
}



// log ln

void MainWindow::on_pushButton_ln_clicked()
{
    QString str = MainWindow::ui->display->text();
    str = modify(str);
    bool b = ifinputvaild(str);
    if(b==false)
   { MainWindow::ui->display-> setText("Error：算式书写错误");}
    else
    {  double tmp_num = CalFromStr(str);
        if(tmp_num<=0)
        {
            MainWindow::ui->res_disp-> setText("输入小于0");
        }
        else{
       double num_res = qLn(tmp_num);
        //ln计算
        QString res = QString::number(num_res, 'f', 4);
        int i=res.length()-1;
        while(res[i]=='0')
        {
            res.remove(i,1);
            i--;
        }
        if(res[i]=='.'){res.remove(i,1);}

        //输入到M并输出
        memory = res;
        MainWindow::ui->res_disp-> setText("ln("+str+")= "+res);
        }
}
}

void MainWindow::on_pushButton_log_clicked()
{
    QString str = MainWindow::ui->display->text();
    str = modify(str);
    bool b = ifinputvaild(str);
    if(b==false)
   { MainWindow::ui->display-> setText("Error：算式书写错误");}
    else
    {  double tmp_num = CalFromStr(str);
        if(tmp_num<=0)
        {
            MainWindow::ui->res_disp-> setText("输入小于0");
        }
        else{
       double num_res = qLn(tmp_num)/qLn(10);
        //lg计算
        QString res = QString::number(num_res, 'f', 4);
        int i=res.length()-1;
        while(res[i]=='0')
        {
            res.remove(i,1);
            i--;
        }
        if(res[i]=='.'){res.remove(i,1);}

        //输入到M并输出
        memory = res;
        MainWindow::ui->res_disp-> setText("ln("+str+")= "+res);
        }
}
}


//平方与根号
void MainWindow::on_pushButton_23_clicked()
{
    QString str = MainWindow::ui->display->text();
    str = modify(str);
    bool b = ifinputvaild(str);
    if(b==false)
   { MainWindow::ui->display-> setText("Error：算式书写错误");}
    else
    {  double tmp_num = CalFromStr(str);
       double num_res = tmp_num*tmp_num;
        //平方计算
        QString res = QString::number(num_res, 'f', 4);
        int i=res.length()-1;
        while(res[i]=='0')
        {
            res.remove(i,1);
            i--;
        }
        if(res[i]=='.'){res.remove(i,1);}

        //输入到M并输出
        memory = res;
        MainWindow::ui->res_disp-> setText(str+"²  = "+res);
}
}

void MainWindow::on_root_clicked()
{
    QString str = MainWindow::ui->display->text();
    str = modify(str);
    bool b = ifinputvaild(str);
    if(b==false)
   { MainWindow::ui->display-> setText("Error：算式书写错误");}
    else
    {  double tmp_num = CalFromStr(str);
        if(tmp_num<=0)
        {
            MainWindow::ui->res_disp-> setText("输入小于0");
        }
        else{
       double num_res = qSqrt(tmp_num);
        //平方根计算
        QString res = QString::number(num_res);
        int i=res.length()-1;
        while(res[i]=='0')
        {
            res.remove(i,1);
            i--;
        }
        if(res[i]=='.'){res.remove(i,1);}

        //输入到M并输出
        memory = res;
        MainWindow::ui->res_disp-> setText("√ "+str+" = "+res);
        }
}
}


//π和e
void MainWindow::on_pi_bt_clicked()
{
    QString str = MainWindow::ui->display->text();
    MainWindow::ui->display-> setText(str+"3.141593");
}

void MainWindow::on_e_bt_clicked()
{
    QString str = MainWindow::ui->display->text();
    MainWindow::ui->display-> setText(str+"2.718282");
}










//返回
void MainWindow::on_prt1_clicked()
{
   MainWindow::ui->sw->setCurrentIndex(1) ;
}
void MainWindow::on_car_to_clicked()
{
    MainWindow::ui->sw->setCurrentIndex(3) ;
}


void MainWindow::on_thermo_rt_clicked()
{
    MainWindow::ui->sw->setCurrentIndex(1) ;
}
void MainWindow::on_thermo_to_clicked()
{
    MainWindow::ui->sw->setCurrentIndex(4) ;
}


void MainWindow::on_Celes_rt_clicked()
{
    MainWindow::ui->sw->setCurrentIndex(1) ;
}
void MainWindow::on_Celes_to_clicked()
{
    MainWindow::ui->sw->setCurrentIndex(5) ;
}

void MainWindow::on_ff_rt_clicked()
{
    MainWindow::ui->sw->setCurrentIndex(1) ;
}
void MainWindow::on_ff_to_clicked()
{
    MainWindow::ui->sw->setCurrentIndex(7) ;
}

void MainWindow::on_magnetic_rt_clicked()
{
    MainWindow::ui->sw->setCurrentIndex(1) ;
}

void MainWindow::on_magnetic_to_clicked()
{
    MainWindow::ui->sw->setCurrentIndex(8);
}

void MainWindow::on_electric_rt_clicked()
{
    MainWindow::ui->sw->setCurrentIndex(1);
}

void MainWindow::on_electric_to_clicked()
{
    MainWindow::ui->sw->setCurrentIndex(9);
}

void MainWindow::on_kinetic_rt_clicked()
{
    MainWindow::ui->sw->setCurrentIndex(1);
}
void MainWindow::on_kinetic_to_clicked()
{
    MainWindow::ui->sw->setCurrentIndex(10);
}

void MainWindow::on_angular_rt_clicked()
{
    MainWindow::ui->sw->setCurrentIndex(1);
}
void MainWindow::on_angular_to_clicked()
{
    MainWindow::ui->sw->setCurrentIndex(11);
}

void MainWindow::on_Pendulum_rt_clicked()
{
    MainWindow::ui->sw->setCurrentIndex(1);
}

void MainWindow::on_Pendulum_to_clicked()
{
    MainWindow::ui->sw->setCurrentIndex(12);
}



//直线运动实现
void MainWindow::on_car_1_clicked()
{
    QString v0 = MainWindow::ui->car_v0->text();
    QString v = MainWindow::ui->car_v->text();
    QString a = MainWindow::ui->car_a->text();
    QString t = MainWindow::ui->car_t->text();
    QString s = MainWindow::ui->car_s->text();
    int lgv0 = v0.length();                     //获取所有框中的字符及长度
    int lgv = v.length();
    int lga = a.length();
    int lgt = t.length();
    int lgs = s.length();
    double nv0=v0.toDouble();
    double nv=v.toDouble();
    double na=a.toDouble();
    double nt=t.toDouble();
    double ns=s.toDouble();
    int emp = 0 ;
    MainWindow::ui->car_err-> setText("");
    if(lgv0==0){emp++;}
    if(lgv==0){emp++;}
    if(lga==0){emp++;}
    if(lgt==0){emp++;}
    if(lgs==0){emp++;}

    if(emp>2){MainWindow::ui->car_err-> setText("错误：参数少于3个");}
    else{

    if(lgv0==0)
    {
     if(lgv==0)
     {nv0=ns/nt-na*nt/2;
      nv=nv0+na*nt;}
     else if(lga==0)
     {na=2*(nv*nt-ns)/(nt*nt);
      nv0=nv-na*nt;}
     else if(lgt==0)
     {nv0=qSqrt(nv*nv-2*na*ns);
      nt=(nv-nv0)/nt;}
     else
     {nv0=nv-na*nt;
      ns=nv*nt-na*nt*nt/2;}
    }
    else if(lgv==0)                               // 对所有可能的输入计算
    {
     if(lga==0)
     {na=(ns-nv0*nt)*2/(nt*nt);
      nv=nv0+na*nt;}
     else if(lgt==0)
     {nv=qSqrt(2*na*ns+nv0*nv0);
      nt=(nv-nv0)/na;}
     else
     {nv=nv0+na*nt;
      ns=(nv0+nv)*nt/2;}
    }
    else if(lga==0)
    {
     if(lgt==0)
     {na=(nv*nv-nv0*nv0)/(ns*2);
      nt=(nv-nv0)/na;}
     else
     {na=(nv-nv0)/nt;
      ns=nv0*nt+na*nt*nt/2;}
    }
    else if(lgt==0)
    {nt=(nv-nv0)/na;
     ns=nv0*nt+na*nt*nt/2;}

    QString strv0 = QString::number(nv0);
    QString strv = QString::number(nv);
    QString stra = QString::number(na);
    QString strt = QString::number(nt);
    QString strs = QString::number(ns);                       // 输出结果到框中

    MainWindow::ui->car_v0-> setText(strv0);
    MainWindow::ui->car_v-> setText(strv);
    MainWindow::ui->car_t-> setText(strt);
    MainWindow::ui->car_a-> setText(stra);
    MainWindow::ui->car_s-> setText(strs);
}
}


//热力学问题1
void MainWindow::on_thermo_1_clicked()
{   MainWindow::ui->thermo_err-> setText("");
    QString p = MainWindow::ui->thermo_p->text();
    QString v = MainWindow::ui->thermo_v->text();
    QString n = MainWindow::ui->thermo_n->text();
    QString t = MainWindow::ui->thermo_t->text();

    int lgp = p.length();
    int lgv = v.length();
    int lgn = n.length();
    int lgt = t.length();

    double np=p.toDouble();
    double nv=v.toDouble();
    double nn=n.toDouble();
    double nt=t.toDouble();
    int emp=0;
    double R=8.314;
    if(lgp==0){emp++;}
    if(lgv==0){emp++;}
    if(lgn==0){emp++;}
    if(lgt==0){emp++;}
    if(emp>1){MainWindow::ui->thermo_err-> setText("错误：参数少于3个");}
    else{

    if(lgp==0){np=nn*R*nt/nv;}
    else if(lgv==0){nv=nn*R*nt/np;}
    else if(lgn==0){nn=np*nv/(R*nt);}
    else if(lgt==0){nt=np*nv/(R*nn);}

    QString strp = QString::number(np);
    QString strv = QString::number(nv);
    QString strn = QString::number(nn);
    QString strt = QString::number(nt);

    MainWindow::ui->thermo_p-> setText(strp);
    MainWindow::ui->thermo_v-> setText(strv);
    MainWindow::ui->thermo_n-> setText(strn);
    MainWindow::ui->thermo_t-> setText(strt);
}
}
//热力学问题2
void MainWindow::on_thermo_2_clicked()
{
    MainWindow::ui->thermo_err-> setText("");
       QString p1 = MainWindow::ui->thermo_p1->text();
       QString v1 = MainWindow::ui->thermo_v1->text();
       QString t1 = MainWindow::ui->thermo_t1->text();
       QString p2 = MainWindow::ui->thermo_p2->text();
       QString v2 = MainWindow::ui->thermo_v2->text();
       QString t2 = MainWindow::ui->thermo_t2->text();

       int lgp1 = p1.length();
       int lgv1 = v1.length();
       int lgt1 = t1.length();
       int lgp2 = p2.length();
       int lgv2 = v2.length();
       int lgt2 = t2.length();

       double np1=p1.toDouble();
       double nv1=v1.toDouble();
       double nt1=t1.toDouble();
       double np2=p2.toDouble();
       double nv2=v2.toDouble();
       double nt2=t2.toDouble();
       int emp=0;
       if(lgp1==0){emp++;}
       if(lgv1==0){emp++;}
       if(lgt1==0){emp++;}
       if(lgp2==0){emp++;}
       if(lgv2==0){emp++;}
       if(lgt2==0){emp++;}
       if(emp>1){MainWindow::ui->thermo_err-> setText("错误：参数少于5个");}
       else{
       if(lgp1==0){np1=(np2*nv2/nt2)/(nv1/nt1);}
       else if(lgv1==0){nv1=(np2*nv2/nt2)/(np1/nt1);}
       else if(lgt1==0){nt1=np1*nv1*nt2/(np2*nv2);}
       else if(lgp2==0){np2=(np1*nv1/nt1)/(nv2/nt2);}
       else if(lgv2==0){nv2=(np1*nv1/nt1)/(np2/nt2);}
       else if(lgt2==0){nt2=np2*nv2*nt1/(np1*nv1);}

       QString strp1 = QString::number(np1);
       QString strv1 = QString::number(nv1);
       QString strt1 = QString::number(nt1);
       QString strp2 = QString::number(np2);
       QString strv2 = QString::number(nv2);
       QString strt2 = QString::number(nt2);

       MainWindow::ui->thermo_p1-> setText(strp1);
       MainWindow::ui->thermo_v1-> setText(strv1);
       MainWindow::ui->thermo_t1-> setText(strt1);
       MainWindow::ui->thermo_p2-> setText(strp2);
       MainWindow::ui->thermo_v2-> setText(strv2);
       MainWindow::ui->thermo_t2-> setText(strt2);

}
}

//天体运动问题
void MainWindow::on_celes_1_clicked()
{
     MainWindow::ui->celes_err-> setText("");
     QString M = MainWindow::ui->celes_M->text();
     QString t = MainWindow::ui->celes_t->text();
     QString r = MainWindow::ui->celes_r->text();
     QString v = MainWindow::ui->celes_v->text();
     QString w = MainWindow::ui->celes_w->text();

     int lgM = M.length();
     int lgt = t.length();
     int lgr = r.length();
     int lgv = v.length();
     int lgw = w.length();

     double G=6.67*pow(10,9);
     double pi=3.14159;
     double tri= 1.0/3;
     double nM=M.toDouble();
     double nt=t.toDouble();
     double nr=r.toDouble();
     double nv=v.toDouble();
     double nw=w.toDouble();

     if(lgM==0){MainWindow::ui->celes_err-> setText("错误：M未输入");}
     else if(lgt==0&&lgr==0&&lgv==0&&lgw==0){MainWindow::ui->celes_err-> setText("错误：参数少于1");}
     else{
     double gm= G*nM;
     if(lgt!=0){nr=pow(nt*nt*gm/(4*pi*pi),tri);
                nv=2*pi*nr/nt;
                nw=nv/nr;}
     else if(lgr!=0){nt=qSqrt(4*pi*pi*pow(nr,3)/gm);
                     nv=2*pi*nr/nt;
                     nw=nv/nr;}
     else if(lgv!=0){nr=gm/(nv*nv);
                     nt=2*pi*nr/nv;
                     nw=nv/nr;}
     else if(lgw!=0){nr=pow(gm/(nw*nw),tri);
                     nv=nw*nr;
                     nt=2*pi*nr/nv;}
     QString strt = QString::number(nt);
     QString strr = QString::number(nr);
     QString strv = QString::number(nv);
     QString strw = QString::number(nw);

     MainWindow::ui->celes_t-> setText(strt);
     MainWindow::ui->celes_r-> setText(strr);
     MainWindow::ui->celes_v-> setText(strv);
     MainWindow::ui->celes_w-> setText(strw);
}




}
//天体运动2
void MainWindow::on_celes_2_clicked()
{
    MainWindow::ui->celes_err-> setText("");
    QString rt = MainWindow::ui->celes_ratio_t->text();
    QString rr = MainWindow::ui->celes_ratio_r->text();
    QString rv = MainWindow::ui->celes_ratio_v->text();
    QString rw = MainWindow::ui->celes_ratio_w->text();

    int lgt = rt.length();
    int lgr = rr.length();
    int lgv = rv.length();
    int lgw = rw.length();

    double nt=rt.toDouble();
    double nr=rr.toDouble();
    double nv=rv.toDouble();
    double nw=rw.toDouble();

    if(lgt==0&&lgr==0&&lgv==0&&lgw==0){MainWindow::ui->celes_err-> setText("错误：参数少于1");}
    else {if(lgt!=0){nr=pow(nt,2.0/3);
                    nv=nr/nt;
                    nw=1/nt;}
    else if(lgr!=0){nt=pow(nr,1.5);
                    nv=nr/nt;
                    nw=1/nt;}
    else if(lgv!=0){nr=1/(nv*nv);
                    nt=nr/nv;
                    nw=nv/nr;}
    else if(lgw!=0){nt=1/nw;
                    nr=pow(nt,2.0/3);
                    nv=nr/nt;}

        QString strt = QString::number(nt);
        QString strr = QString::number(nr);
        QString strv = QString::number(nv);
        QString strw = QString::number(nw);

        MainWindow::ui->celes_ratio_t-> setText(strt);
        MainWindow::ui->celes_ratio_r-> setText(strr);
        MainWindow::ui->celes_ratio_v-> setText(strv);
        MainWindow::ui->celes_ratio_w-> setText(strw);
}
}


//抛物线
void MainWindow::on_ff_1_clicked()
{
    MainWindow::ui->ff_err-> setText("");
    QString vx = MainWindow::ui->ff_vx->text();
    QString vy = MainWindow::ui->ff_vy->text();
    QString h = MainWindow::ui->ff_h->text();
    QString g = MainWindow::ui->ff_g->text();

    int lgvx = vx.length();
    int lgvy = vy.length();
    int lgh = h.length();
    int lgg = g.length();

    double nvx=vx.toDouble();
    double nvy=vy.toDouble();
    double nh=h.toDouble();
    double ng=g.toDouble();


    if(lgvx==0||lgvy==0||lgh==0||lgg==0){MainWindow::ui->ff_err-> setText("错误：缺少参数");}
    else{
    double mt1=nvy/ng;
    double mt2=qSqrt((2*nh+nvy*mt1)/ng);
    double mt=mt1+mt2;
    double ms=nvx*mt;
    double mh=nh+nvy*mt1/2;
    double a=-ng/(2*nvx*nvx);
    double b=nvy/nvx;

     QString strmt = QString::number(mt);
     QString strms = QString::number(ms);
     QString strmh = QString::number(mh);
     QString stra = QString::number(a);
     QString strb = QString::number(b);

     MainWindow::ui->ff_mt-> setText(strmt);
     MainWindow::ui->ff_ms-> setText(strms);
     MainWindow::ui->ff_mh-> setText(strmh);
     MainWindow::ui->ff_parab-> setText("y = "+stra+"x²+"+strb+"x+"+h);

    }
}


//功能关系        按键输入角度未完成
void MainWindow::on_kinetic_1_clicked()
{
    MainWindow::ui->kinetic_err-> setText("");
    QString f = MainWindow::ui->kinetic_f->text();
    QString s = MainWindow::ui->kinetic_s->text();
    QString ac = MainWindow::ui->kinetic_ac->text();
    QString w = MainWindow::ui->kinetic_w->text();

    int lgf = f.length();
    int lgs = s.length();
    int lgac = ac.length();
    int lgw = w.length();
    int emp=0;
    double nacn=ac.toDouble();
    double nf=f.toDouble();
    double ns=s.toDouble();
    double nac=nacn*3.1415926/180;
    double nw=w.toDouble();
    if(lgf==0){emp++;}
    if(lgs==0){emp++;}
    if(lgac==0){emp++;}
    if(lgw==0){emp++;}
    if(emp>1){MainWindow::ui->kinetic_err-> setText("错误：缺少参数");}
    else{if(nacn==90||nac<0||nac>3.1415926){MainWindow::ui->kinetic_err-> setText("错误：角度为90°或越界");}
        else if(lgf==0){nf=nw/(ns*cos(nac));}
        else if(lgs==0){ns=nw/(nf*cos(nac));}
        else if(lgac==0){nac=cosh(nw/(nf*ns));
                         nacn=nac*180/3.1415926;}
        else if(lgw==0){nw=nf*ns*cos(nac);}

        QString strf = QString::number(nf);
        QString strs = QString::number(ns);
        QString strac = QString::number(nacn);
        QString strw = QString::number(nw);

        MainWindow::ui->kinetic_f-> setText(strf);
        MainWindow::ui->kinetic_s-> setText(strs);
        MainWindow::ui->kinetic_ac-> setText(strac);
        MainWindow::ui->kinetic_w-> setText(strw);
    }

}

//动能定理
void MainWindow::on_kinetic_3_clicked()
{
    MainWindow::ui->kinetic_err-> setText("");
    QString v1 = MainWindow::ui->kinetic_v1->text();
    QString v2 = MainWindow::ui->kinetic_v2->text();
    QString m = MainWindow::ui->kinetic_m->text();
    QString ek1 = MainWindow::ui->kinetic_ek1->text();
    QString ek2 = MainWindow::ui->kinetic_ek2->text();
    QString w2 = MainWindow::ui->kinetic_w2->text();

    int lgv1 = v1.length();
    int lgv2 = v2.length();
    int lgm = m.length();
    int lgek1 = ek1.length();
    int lgek2 = ek2.length();
    int lgw2 = w2.length();

     double nv1=v1.toDouble();
     double nv2=v2.toDouble();
     double nm=m.toDouble();
     double nek1=ek1.toDouble();
     double nek2=ek2.toDouble();
     double nw2=w2.toDouble();

    if(lgm==0){
        int emp1=0;
        if(lgek1==0){emp1++;}
        if(lgek2==0){emp1++;}
        if(lgw2==0){emp1++;}
        if(emp1>1)
        {MainWindow::ui->kinetic_err-> setText("错误：缺少参数");}
        else{
        if(lgek1==0){nek1=nw2+nek2;}
        else if(lgek2==0){nek2=nw2-nek1;}
        else if(lgw2==0){nw2=nek1-nek2;}
        QString strw2 = QString::number(nw2);
        QString strek1 = QString::number(nek1);
        QString strek2 = QString::number(nek2);
        MainWindow::ui->kinetic_w2-> setText(strw2);
        MainWindow::ui->kinetic_ek1-> setText(strek1);
        MainWindow::ui->kinetic_ek2-> setText(strek2);
        }
    }
    else{
        if(lgw2==0){
        if(lgv1!=0){
            nek1=nm*nv1*nv1/2;
        }
        else if(lgek1!=0){nv1=qSqrt(2*nek1/nm);}
        else{MainWindow::ui->kinetic_err-> setText("错误：缺少参数");}


        if(lgv2!=0){
            nek2=nm*nv2*nv2/2;
        }
        else if(lgek2!=0){nv2=qSqrt(2*nek2/nm);}
        else{MainWindow::ui->kinetic_err-> setText("错误：缺少参数");}

        nw2=nek1-nek2;

        QString strv1 = QString::number(nv1);
        QString strv2 = QString::number(nv2);
        QString strm = QString::number(nm);
        QString strek1 = QString::number(nek1);
        QString strek2 = QString::number(nek2);
        QString strw2 = QString::number(nw2);

        MainWindow::ui->kinetic_v1-> setText(strv1);
        MainWindow::ui->kinetic_v2-> setText(strv2);
        MainWindow::ui->kinetic_m-> setText(strm);
        MainWindow::ui->kinetic_ek1-> setText(strek1);
        MainWindow::ui->kinetic_ek2-> setText(strek2);
        MainWindow::ui->kinetic_w2-> setText(strw2);
        }
        else{
            if(lgv1!=0){
                nek1=nm*nv1*nv1/2;
                nek2=nek1-nw2;
                nv2=qSqrt(2*nek2/nm);
            }
            else if(lgek1!=0){
                nv1=qSqrt(2*nek1/nm);
                nek2=nek1-nw2;
                nv2=qSqrt(2*nek2/nm);
            }
            else if(lgv2!=0){
                nek2=nm*nv2*nv2/2;
                nek1=nw2+nek2;
                nv1=qSqrt(2*nek1/nm);
            }
            else if(lgek2!=0){
                nv2=qSqrt(2*nek2/nm);
                nek1=nw2+nek2;
                nv1=qSqrt(2*nek1/nm);
            }
            QString strv1 = QString::number(nv1);
            QString strv2 = QString::number(nv2);
            QString strm = QString::number(nm);
            QString strek1 = QString::number(nek1);
            QString strek2 = QString::number(nek2);
            QString strw2 = QString::number(nw2);

            MainWindow::ui->kinetic_v1-> setText(strv1);
            MainWindow::ui->kinetic_v2-> setText(strv2);
            MainWindow::ui->kinetic_m-> setText(strm);
            MainWindow::ui->kinetic_ek1-> setText(strek1);
            MainWindow::ui->kinetic_ek2-> setText(strek2);
            MainWindow::ui->kinetic_w2-> setText(strw2);
        }
    }
}


//静电场
void MainWindow::on_electric_1_clicked()
{
    MainWindow::ui->electric_err-> setText("");
    QString E = MainWindow::ui->electric_E->text();
    QString Q = MainWindow::ui->electric_Q->text();
    QString q = MainWindow::ui->electric_q->text();
    QString f = MainWindow::ui->electric_f->text();
    QString r = MainWindow::ui->electric_r->text();

    int lgE = E.length();
    int lgQ = Q.length();
    int lgq = q.length();
    int lgf = f.length();
    int lgr = r.length();
    double k=90;
    double nE=E.toDouble();
    double nQ=Q.toDouble();
    double nq=q.toDouble();
    double nf=f.toDouble();
    double nr=r.toDouble();

    if(lgE==0)
    {
    if(lgQ==0)
    {if(nf==0||nq==0){MainWindow::ui->electric_err-> setText("错误：缺少参数");}
     else{
        nE=nf/nq;
        QString strE = QString::number(nE);
        MainWindow::ui->electric_E-> setText(strE);
         }
    }
    else{
        if(nr==0){
            if(nf==0||nq==0){MainWindow::ui->electric_err-> setText("错误：缺少参数");}
                 else{
                    nE=nf/nq;
                    nr=qSqrt(k*nQ/nE);//*10^4
                    QString strE = QString::number(nE);
                    QString strr = QString::number(nr);
                    MainWindow::ui->electric_E-> setText(strE);
                    MainWindow::ui->electric_r-> setText(strr);
                     }
        }
        else {
            nE=k*nQ/(nr*nr);//*10^8
            QString strE = QString::number(nE);
            MainWindow::ui->electric_E-> setText(strE);
            if(nf!=0){
                nq=nf/nE;
                QString strq = QString::number(nq);
                MainWindow::ui->electric_r-> setText(strq);
            }
            else if(nq!=0){
                nf=nE*nq;
                QString strf = QString::number(nf);
                MainWindow::ui->electric_f-> setText(strf);
            }
        }
    }

}
    else{
        if(lgQ!=0){
            nr=qSqrt(k*nQ/(nE));//*10^4
            QString strr = QString::number(nr);
            MainWindow::ui->electric_r-> setText(strr);
        }
        else if(lgr!=0){
            nQ=nE*nr*nr/k;//*10^-8
            QString strQ = QString::number(nQ);
            MainWindow::ui->electric_Q-> setText(strQ);
        }
        if(lgq!=0){
            nf=nE*nq;//*10^4
            QString strf = QString::number(nf);
            MainWindow::ui->electric_f-> setText(strf);
        }
        else if(lgf!=0){
            nq=nf/nE;//*10^-8
            QString strq = QString::number(nq);
            MainWindow::ui->electric_q-> setText(strq);
        }
        else{MainWindow::ui->electric_err-> setText("错误：缺少参数");}
    }
}


//电势能
void MainWindow::on_electric_2_clicked()
{
    MainWindow::ui->electric_err-> setText("");
    QString ep = MainWindow::ui->electric_ep->text();
    QString u = MainWindow::ui->electric_u->text();
    QString q1 = MainWindow::ui->electric_q1->text();

    int lgep = ep.length();
    int lgu = u.length();
    int lgq1 = q1.length();
    int emp=0;

    double nep=ep.toDouble();
    double nu=u.toDouble();
    double nq1=q1.toDouble();

    if(lgep==0){emp++;}
    if(lgu==0){emp++;}
    if(lgq1==0){emp++;}

    if(emp>1){MainWindow::ui->electric_err-> setText("错误：缺少参数");}
    else{
        if(lgep==0){
            nep=nu*nq1;
            QString strep = QString::number(nep);
            MainWindow::ui->electric_ep-> setText(strep);
        }
        else if(lgu==0){
            nu=nep/nq1;
            QString stru = QString::number(nu);
            MainWindow::ui->electric_u-> setText(stru);
        }
        else if(lgq1==0){
            nq1=nep/nu;
            QString strq1 = QString::number(nq1);
            MainWindow::ui->electric_q1-> setText(strq1);
        }
    }
}

//直导线在磁场中受力
void MainWindow::on_magnetic_1_clicked()
{
    MainWindow::ui->magnetic_err-> setText("");
    QString b1 = MainWindow::ui->magnetic_b1->text();
    QString l = MainWindow::ui->magnetic_l->text();
    QString I = MainWindow::ui->magnetic_i->text();
    QString f1 = MainWindow::ui->magnetic_f1->text();


    int lgf1 = f1.length();
    int lgb1 = b1.length();
    int lgl = l.length();
    int lgI = I.length();
    int emp=0;

    double nb1=b1.toDouble();
    double nf1=f1.toDouble();
    double nl=l.toDouble();
    double nI=I.toDouble();

    if(lgb1==0){emp++;}
    if(lgl==0){emp++;}
    if(lgI==0){emp++;}
    if(lgf1==0){emp++;}
    if(emp>1){MainWindow::ui->magnetic_err-> setText("错误：缺少参数");}
    else{
    if(lgb1==0)
    {
        nb1=nf1/(nl*nI);
        QString strb1 = QString::number(nb1);
        MainWindow::ui->magnetic_b1-> setText(strb1);
    }
    else if(lgf1==0)
    {
        nf1=nb1*nl*nI;
        QString strf1 = QString::number(nf1);
        MainWindow::ui->magnetic_f1-> setText(strf1);
    }
    else if(lgl==0)
    {
        nl=nf1/(nb1*nI);
        QString strl = QString::number(nl);
        MainWindow::ui->magnetic_l-> setText(strl);
    }
    else if(lgI==0)
    {
        nI=nf1/(nl*nb1);
        QString strI = QString::number(nI);
        MainWindow::ui->magnetic_i-> setText(strI);
    }

    }
}

//带电粒子在磁场中运动
void MainWindow::on_magnetic_2_clicked()
{
    MainWindow::ui->magnetic_err-> setText("");
    QString b2 = MainWindow::ui->magnetic_b2->text();
    QString q = MainWindow::ui->magnetic_q->text();
    QString v = MainWindow::ui->magnetic_v->text();
    QString f2 = MainWindow::ui->magnetic_f2->text();


    int lgf2 = f2.length();
    int lgb2 = b2.length();
    int lgq = q.length();
    int lgv = v.length();
    int emp=0;

    double nb2=b2.toDouble();
    double nf2=f2.toDouble();
    double nq=q.toDouble();
    double nv=v.toDouble();

    if(lgb2==0){emp++;}
    if(lgq==0){emp++;}
    if(lgv==0){emp++;}
    if(lgf2==0){emp++;}
    if(emp>1){MainWindow::ui->magnetic_err-> setText("错误：缺少参数");}
    else{
    if(lgb2==0)
    {
        nb2=nf2/(nq*nv);
        QString strb2 = QString::number(nb2);
        MainWindow::ui->magnetic_b2-> setText(strb2);
    }
    else if(lgf2==0)
    {
        nf2=nb2*nq*nv;
        QString strf2 = QString::number(nf2);
        MainWindow::ui->magnetic_f2-> setText(strf2);
    }
    else if(lgq==0)
    {
        nq=nf2/(nb2*nv);
        QString strq = QString::number(nq);
        MainWindow::ui->magnetic_q-> setText(strq);
    }
    else if(lgv==0)
    {
        nv=nf2/(nq*nb2);
        QString strv = QString::number(nv);
        MainWindow::ui->magnetic_v-> setText(strv);
    }

    }
}


//摆锤周期
void MainWindow::on_pend_1_clicked()
{
    MainWindow::ui->pend_err-> setText("");
    QString t = MainWindow::ui->pend_t->text();
    QString l = MainWindow::ui->pend_l->text();
    QString g = MainWindow::ui->pend_g->text();

    int lgt = t.length();
    int lgl = l.length();
    int lgg = g.length();
    int emp=0;

    double nt=t.toDouble();
    double nl=l.toDouble();
    double ng=g.toDouble();

    if(lgt==0){emp++;}
    if(lgl==0){emp++;}
    if(lgg==0){emp++;}
    if(emp>1){MainWindow::ui->pend_err-> setText("错误：缺少参数");}
    else{
    if(lgt==0)
    {
        nt=2*3.1415*qSqrt(nl/ng);
        QString strt = QString::number(nt);
        MainWindow::ui->pend_t-> setText(strt);
    }
    else if(lgl==0)
    {
        nl=ng*(nt*nt/(4*3.1415*3.1415));
        QString strl = QString::number(nl);
        MainWindow::ui->pend_l-> setText(strl);
    }
    else if(lgg==0)
    {
        ng=(nt*nt/(4*3.1415*3.1415))/nl;
        QString strg = QString::number(ng);
        MainWindow::ui->pend_g-> setText(strg);
    }
    }
}

















void MainWindow::on_pend_2_clicked()
{
    MainWindow::ui->pend_err-> setText("");
    QString v = MainWindow::ui->pend_v->text();
    QString li = MainWindow::ui->pend_li->text();
    QString f = MainWindow::ui->pend_f->text();

    int lgv = v.length();
    int lgli = li.length();
    int lgf = f.length();
    int emp=0;

    double nv=v.toDouble();
    double nli=li.toDouble();
    double nf=f.toDouble();

    if(lgv==0){emp++;}
    if(lgli==0){emp++;}
    if(lgf==0){emp++;}
    if(emp>1){MainWindow::ui->pend_err-> setText("错误：缺少参数");}
    else{
    if(lgv==0)
    {
        nv=nli*nf;
        QString strv = QString::number(nv);
        MainWindow::ui->pend_v-> setText(strv);
    }
    else if(lgli==0)
    {
        nli=nv/nf;
        QString strli = QString::number(nli);
        MainWindow::ui->pend_li-> setText(strli);
    }
    else if(lgf==0)
    {
        nf=nv/nli;
        QString strf = QString::number(nf);
        MainWindow::ui->pend_f-> setText(strf);
    }
    }
}




void MainWindow::on_pushButton_12_clicked()
{
       MainWindow::ui->sw->setCurrentIndex(6);
}

void MainWindow::on_rt6_clicked()
{
       MainWindow::ui->sw->setCurrentIndex(10);
}

void MainWindow::on_e1_clicked()
{
    MainWindow::ui->sw->setCurrentIndex(11);
}

void MainWindow::on_rt6_2_clicked()
{
    MainWindow::ui->sw->setCurrentIndex(9);
}

void MainWindow::on_e1_2_clicked()
{
    MainWindow::ui->sw->setCurrentIndex(13);
}

void MainWindow::on_thermo_rt_2_clicked()
{
    MainWindow::ui->sw->setCurrentIndex(4);
}

void MainWindow::on_e1_3_clicked()
{
    MainWindow::ui->sw->setCurrentIndex(14);
}

void MainWindow::on_Celes_rt_2_clicked()
{
    MainWindow::ui->sw->setCurrentIndex(5);
}

void MainWindow::on_close_clicked()
{

}
