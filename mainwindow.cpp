#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mult_thread.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    showNetworkCard();

    mult_thread *thread = new mult_thread;
    static bool running = false;

    connect(ui->actionrun_stop,&QAction::triggered,this,[=](){
        running = !running;
        if(running){
            int capture_exec = capture();
            if(capture_exec != -1 && pcap_t_pointer){
                thread->setPointer(pcap_t_pointer);
                thread->setFlag();
                thread->start();
                ui->actionrun_stop->setIcon(QIcon(":/stop.png"));
                ui->comboBox->setEnabled(false);
            }
        }
        else{
            thread->resetFlag();
            thread->quit();
            thread->wait();
            ui->actionrun_stop->setIcon(QIcon(":/play.png"));
            ui->comboBox->setEnabled(true);
            pcap_close(pcap_t_pointer);
            pcap_t_pointer = nullptr;
        }
    });

    connect(ui->comboBox,&QComboBox::currentIndexChanged,this,[&](int cb_index){
        int i = 0;
        if(cb_index != 0){
            for(device = all_device;i < cb_index - 1;device = device->next,++i);
        }
    });

    connect(thread,&mult_thread::send,this,&MainWindow::HandleMessage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showNetworkCard(){
    int n = pcap_findalldevs(&all_device,errbuf); //取得所有網卡

    if(n == -1){
        ui->comboBox->addItem("error: " + QString(errbuf));
    }
    else{
        ui->comboBox->clear();
        ui->comboBox->addItem("choose your Network Card.");
        for(device = all_device;device != nullptr;device = device->next){
            QString device_name = device->name;
            device_name.replace("\\Device\\","");

            QString device_description = device->description;
            QString item = device_name + device_description;
            ui->comboBox->addItem(item);
        }
    }

}

int MainWindow::capture(){
    if(device){
        pcap_t_pointer = pcap_open_live(device->name,65536,1,1000,errbuf);
    }
    else{
        return -1;
    }
    if(!pcap_t_pointer){
        pcap_freealldevs(all_device);
        device = nullptr;
        return -1;
    }

    if(pcap_datalink(pcap_t_pointer) != DLT_EN10MB){ //如果不是Ethernet封包，不抓
        pcap_close(pcap_t_pointer);
        pcap_freealldevs(all_device);
        device = nullptr;
        pcap_t_pointer = nullptr;
        return -1;
    }
    statusBar()->showMessage(device->name);
    return 0;
}

void MainWindow::HandleMessage(data_package data){
    qDebug() << data.getTimeStamp() << " " << data.getInfo() << "\n";
}
