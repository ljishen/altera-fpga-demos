#include "dialog.h"
#include "ui_dialog.h"

#include <QtGui>
#include <QPainter>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    m_bHPS_ButtonPressed(false),
    m_FPGA_KeyStatus(0),
    m_FPGA_SwitchStatus(0)
{

    // hid help menu
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0); // default to first page

    memset(m_ir_rx_timeout, 0, sizeof(m_ir_rx_timeout));
    // hps and fpga init
    hps = new HPS;
    fpga = new FPGA;

    // fix windows size
    this->setFixedSize(this->width(),this->height());

    // overwrite draw event: draw for child widget
    ui->tabGsensor->installEventFilter(this);
    //ui->tabADC->installEventFilter(this);
    ui->tabButton->installEventFilter(this);
    ui->tabHEX->installEventFilter(this);
    ui->tabIR->installEventFilter(this);

    // create polling timer
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(TimerHandle()));
    timer->start(100);
}

Dialog::~Dialog()
{
    fpga->VideoEnable(false); //always close video-in
    delete ui;
}

void Dialog::on_pushButton_LightAllLed_clicked()
{
    UI_LedSet(true);
    HW_SetLed();
}

void Dialog::on_pushButton_UnlightAllLed_clicked()
{
    UI_LedSet(false);
    HW_SetLed();

}

void Dialog::ClickLED(){
   // qDebug() << "HPS LED Checked:" << ((ui->checkBox_HPS_LED0->isChecked())?"Yes":"No") << "\r\n";
    HW_SetLed();
}



void Dialog::UI_LedSet(bool AllOn){
    ui->checkBox_HPS_LED0->setChecked(AllOn);
    ui->checkBox_LED0->setChecked(AllOn);
    ui->checkBox_LED1->setChecked(AllOn);
    ui->checkBox_LED2->setChecked(AllOn);
    ui->checkBox_LED3->setChecked(AllOn);
    ui->checkBox_LED4->setChecked(AllOn);
    ui->checkBox_LED5->setChecked(AllOn);
    ui->checkBox_LED6->setChecked(AllOn);
    ui->checkBox_LED7->setChecked(AllOn);
    ui->checkBox_LED8->setChecked(AllOn);
    ui->checkBox_LED9->setChecked(AllOn);
}

void Dialog::HW_SetLed(){
    //qDebug() << "HW_SetLed is called/r/n";
    hps->LedSet(ui->checkBox_HPS_LED0->isChecked());

    uint32_t ledMask = 0;
    QCheckBox *szCheck[] = {
        ui->checkBox_LED0,
        ui->checkBox_LED1,
        ui->checkBox_LED2,
        ui->checkBox_LED3,
        ui->checkBox_LED4,
        ui->checkBox_LED5,
        ui->checkBox_LED6,
        ui->checkBox_LED7,
        ui->checkBox_LED8,
        ui->checkBox_LED9,


    };

    for(int i=0;i<10;i++){
        if (szCheck[i]->isChecked())
            ledMask |= (0x01 << i);
    }

    fpga->LedSet(ledMask);
}


bool Dialog::eventFilter(QObject* watched, QEvent* event)
{
    static int x = 0;
    if (watched == ui->tabButton && event->type() == QEvent::Paint) {
        TabButtonDraw();
        return true; // return true if you do not want to have the child widget paint on its own afterwards, otherwise, return false.

    }else if(watched == ui->tabGsensor && event->type() == QEvent::Paint) {
        TabGsensorDraw();
        return true;
    }else if(watched == ui->tabHEX && event->type() == QEvent::Paint) {
        TabHexDraw();
        return true;
    //}else if(watched == ui->tabADC && event->type() == QEvent::Paint) {
     //   TabAdcDraw();
       // return true;
    }else if(watched == ui->tabIR && event->type() == QEvent::Paint) {
        TabIrDraw();
        return true;
    }
    return false;
}




void Dialog::TimerHandle(){
    QWidget *widgetCurrent = ui->tabWidget->currentWidget();

    if (widgetCurrent == ui->tabGsensor){
        TabGsensorPolling(hps);
        ui->tabGsensor->update();

    }else if (widgetCurrent == ui->tabButton){
        TabButtonPolling(hps);
        ui->tabButton->update();


    }else if (widgetCurrent == ui->tabIR){
        TabIrPolling(fpga);
        ui->tabIR->update();

   //}else if (widgetCurrent == ui->tabADC){
          //  TabAdcPolling(fpga);
          //  ui->tabADC->update();
    }
}


void Dialog::on_spinBox_hex0_valueChanged(int val)
{
    ui->tabHEX->update();
    ui->checkBox_HEX_on_off->setChecked(true);
    fpga->HexSet(0, val);
}

void Dialog::on_spinBox_hex1_valueChanged(int val)
{
    ui->tabHEX->update();
    ui->checkBox_HEX_on_off->setChecked(true);
    fpga->HexSet(1, val);
}

void Dialog::on_spinBox_hex2_valueChanged(int val)
{
    ui->tabHEX->update();
    ui->checkBox_HEX_on_off->setChecked(true);
    fpga->HexSet(2, val);
}

void Dialog::on_spinBox_hex3_valueChanged(int val)
{
    ui->tabHEX->update();
    ui->checkBox_HEX_on_off->setChecked(true);
    fpga->HexSet(3, val);
}

void Dialog::on_spinBox_hex4_valueChanged(int val)
{
    ui->tabHEX->update();
    ui->checkBox_HEX_on_off->setChecked(true);
    fpga->HexSet(4, val);
}

void Dialog::on_spinBox_hex5_valueChanged(int val)
{
    ui->tabHEX->update();
    ui->checkBox_HEX_on_off->setChecked(true);
    fpga->HexSet(5, val);
}

void Dialog::ClickHEX() {
    if (!ui->checkBox_HEX_on_off->isChecked()) {
        for(int i=0;i<6;i++){
            fpga->HexOff(i);
        }
    } else {
        ui->spinBox_hex0->setValue(8);
        ui->spinBox_hex1->setValue(8);
        ui->spinBox_hex2->setValue(8);
        ui->spinBox_hex3->setValue(8);
        ui->spinBox_hex4->setValue(8);
        ui->spinBox_hex5->setValue(8);

        ui->tabHEX->update();
        for(int i=0;i<6;i++){
            fpga->HexSet(i, 8);
        }
    }
}



void Dialog::moveEvent(QMoveEvent * event){
    MoveVideo();
}

void Dialog::MoveVideo(){

    if (ui->tabWidget->currentWidget() != ui->tabVideoIn)
        return;

    if (!fpga->IsVideoEnabled())
        return;

    QPoint ptScreen;
    QPoint pt(ui->tabVideoIn->rect().left()+90, ui->tabVideoIn->rect().top()+17);
    ptScreen = ui->tabVideoIn->mapToGlobal(pt);
    fpga->VideoMove(ptScreen.x(),ptScreen.y());
}

void Dialog::on_tabWidget_currentChanged(int index)
{
    if (ui->tabWidget->currentWidget() == ui->tabVideoIn){
        fpga->VideoEnable(true);
        MoveVideo();
    }else{
        fpga->VideoEnable(false);
    }

}
