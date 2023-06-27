#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString memory;
    bool ok=true;
private slots:
    void paintEvent(QPaintEvent *);
    void on_pushButton_clicked();

    void on_pushButton_rt_clicked();

    void on_pushButton_phy_clicked();

    void on_pushButton_bk_clicked();

    void on_pushButton_1_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_0_clicked();

    void on_pushButton_plus_clicked();

    void on_pushButton_minus_clicked();

    void on_pushButton_mult_clicked();

    void on_pushButton_divd_clicked();

    void on_pushButton_dot_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_ac_clicked();

    void on_pushButton_left_clicked();

    void on_pushButton_right_clicked();

    void on_pushButton_eq_clicked();


    void on_pushButton_nega_clicked();

    void on_Memoy_clicked();

    void on_pushButton_tan_clicked();

    void on_pushButton_cos_clicked();

    void on_pushButton_sin_clicked();

    void on_pushButton_ln_clicked();

    void on_pushButton_log_clicked();

    void on_pushButton_23_clicked();

    void on_root_clicked();

    void on_pi_bt_clicked();

    void on_e_bt_clicked();

    void on_prt1_clicked();


    void on_car_to_clicked();

    void on_thermo_rt_clicked();

    void on_thermo_to_clicked();

    void on_Celes_to_clicked();

    void on_Celes_rt_clicked();

    void on_ff_rt_clicked();

    void on_ff_to_clicked();

    void on_magnetic_rt_clicked();

    void on_magnetic_to_clicked();

    void on_electric_rt_clicked();

    void on_electric_to_clicked();

    void on_kinetic_rt_clicked();

    void on_kinetic_to_clicked();

    void on_angular_rt_clicked();

    void on_angular_to_clicked();

    void on_Pendulum_rt_clicked();

    void on_Pendulum_to_clicked();

    void on_car_1_clicked();

    void on_thermo_1_clicked();

    void on_thermo_2_clicked();


    void on_celes_1_clicked();

    void on_celes_2_clicked();

    void on_ff_1_clicked();

    void on_kinetic_1_clicked();

    void on_kinetic_3_clicked();

    void on_electric_1_clicked();

    void on_electric_2_clicked();

    void on_magnetic_1_clicked();

    void on_magnetic_2_clicked();

    void on_pend_1_clicked();

    void on_pend_2_clicked();

    void on_pushButton_12_clicked();

    void on_rt6_clicked();

    void on_e1_clicked();

    void on_rt6_2_clicked();

    void on_close_clicked();

    void on_e1_2_clicked();

    void on_thermo_rt_2_clicked();

    void on_e1_3_clicked();

    void on_Celes_rt_2_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
