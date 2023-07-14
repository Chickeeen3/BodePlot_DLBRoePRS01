#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <stdio.h>
#include <math.h>
#include <string>
#include <iostream>
#include <QDebug>
#include <QLineEdit>
#include <QTextStream>
#define Pi 3.14159

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // loading and display of picture that shows the correct way of input
    ui->setupUi(this);
    QPixmap pix(":/files/Screenshot_Polynom.png");
    ui->pic->setPixmap(pix.scaled(500,500,Qt::KeepAspectRatio));
}

MainWindow::~MainWindow()
{
    delete ui;
}

int DegreeNumerator;
int DegreeDenominator;
float w_upper, w_lower;

// this function calculates the magnitude of the bode plot
// Based of the user input the real and imaginary part of the nominator and denominator are calculated
// there are 4 cases, based on the power of x in the function
// even powers result in real numbers (x^2 is negative as i^2=-1), x^4=positive
// unevenpower result in imaginary numbers with x^3 negative
// this behavior repeats itself for higher powers
float Magnitude(float NumeratorCoefficients[], float DenominatorCoefficients[],int DegreeNumerator, int DegreeDenominator, float f){
    int k; float RealNumerator=0.0; float ImaginaryNumerator=0.0;
    for (k=0;k<=DegreeNumerator;k++){
        if (k%4==0){
            RealNumerator=RealNumerator+(float)(NumeratorCoefficients[k]*pow(f,k));
        }
        else if ((k-1)%4==0){
            ImaginaryNumerator=ImaginaryNumerator+(float)(NumeratorCoefficients[k]*pow(f,k));
        }
        else if (k%2==0 && k%4!=0){
            RealNumerator=RealNumerator-(float)(NumeratorCoefficients[k]*pow(f,k));
        }
        else{
            ImaginaryNumerator=ImaginaryNumerator-(float)(NumeratorCoefficients[k]*pow(f,k));

        }
    }
     float RealDenominator=0.0; float ImaginaryDenominator=0.0; float Magnitude;
    for (k=0;k<=DegreeDenominator;k++){
        if (k%4==0){
            RealDenominator=RealDenominator+(float)(DenominatorCoefficients[k]*pow(f,k));
        }
        else if ((k-1)%4==0){
            ImaginaryDenominator=ImaginaryDenominator+(float)(DenominatorCoefficients[k]*pow(f,k));
        }
        else if (k%2==0 && k%4!=0){
            RealDenominator=RealDenominator-(float)(DenominatorCoefficients[k]*pow(f,k));
        }
        else{
            ImaginaryDenominator=ImaginaryDenominator-(float)(DenominatorCoefficients[k]*pow(f,k));

        }
    }

    // Calculation of the magnitude, conversion into logartihmic values is done in main function
    Magnitude=sqrt(pow(RealNumerator,2)+pow(ImaginaryNumerator,2))/sqrt(pow(RealDenominator,2)+pow(ImaginaryDenominator,2));

    return Magnitude;

};

// this function calculates the angle of the bode plot
// Based of the user input the real and imaginary part of the nominator and denominator are calculated
// the phase angle of nominator and denominator are calculated separately and substracted at the end
// there are 4 cases, based on the power of x in the function
// even powers result in real numbers (x^2 is negative as i^2=-1), x^4=positive
// unevenpower result in imaginary numbers with x^3 negative
// this behavior repeats itself for higher powers
float angle(float NumeratorCoefficients[], float DenominatorCoefficients[],int DegreeNumerator, int DegreeDenominator, float f){
    int tanangle;
    float val; val=180/Pi; int k; float RealNumerator=0.0; float ImaginaryNumerator=0.0;
    int tanvalueNumerator; int angleNumerator; int tanangleNumerator;
    for (k=0;k<=DegreeNumerator;k++){
        if (k%4==0){
            RealNumerator=RealNumerator+(float)(NumeratorCoefficients[k]*pow(f,k));
        }
        else if ((k-1)%4==0){
            ImaginaryNumerator=ImaginaryNumerator+(float)(NumeratorCoefficients[k]*pow(f,k));
        }
        else if (k%2==0 && k%4!=0){
            RealNumerator=RealNumerator-(float)(NumeratorCoefficients[k]*pow(f,k));
        }
        else{
            ImaginaryNumerator=ImaginaryNumerator-(float)(NumeratorCoefficients[k]*pow(f,k));

        }
    }

    // the angle is atan (Im/Re), the case of Re=0 has to be checked beforehand
    if (RealNumerator==0 && ImaginaryNumerator<0){
        tanangleNumerator=-90;
    }
    else if (RealNumerator==0 && ImaginaryNumerator>0){
        tanangleNumerator=90;
    }
    else{
        tanvalueNumerator=(float)ImaginaryNumerator/RealNumerator;
        tanangleNumerator=atan(tanvalueNumerator);
    }
// angle convention needs to be accounted for
    if (RealNumerator<0 && ImaginaryNumerator>0){
        tanangleNumerator=atan(tanvalueNumerator)+Pi;
    }
    else if    (RealNumerator<0 && ImaginaryNumerator<0){
        tanangleNumerator=atan(tanvalueNumerator)-Pi;

    }
    angleNumerator=tanangleNumerator*val;



// approach for denominator is the same as for numberator
    float RealDenominator=0.0; float ImaginaryDenominator=0.0; float Magnitude;
    for (k=0;k<=DegreeDenominator;k++){
        if (k%4==0){
            RealDenominator=RealDenominator+(float)(DenominatorCoefficients[k]*pow(f,k));
        }
        else if ((k-1)%4==0){
            ImaginaryDenominator=ImaginaryDenominator+(float)(DenominatorCoefficients[k]*pow(f,k));
        }
        else if (k%2==0 && k%4!=0){
            RealDenominator=RealDenominator-(float)(DenominatorCoefficients[k]*pow(f,k));
        }
        else{
            ImaginaryDenominator=ImaginaryDenominator-(float)(DenominatorCoefficients[k]*pow(f,k));

        }
    }
    int tanvalueDenominator; int angleDenominator; int tanangleDenominator;
    //qDebug() <<ImaginaryDenominator;
    if (RealDenominator==0 && ImaginaryDenominator<0){
        tanangleDenominator=-90;
    }
    else if (RealDenominator==0 && ImaginaryDenominator>0){
        tanangleDenominator=90;
    }
    else{
        tanvalueDenominator=(float)ImaginaryDenominator/RealDenominator;
        tanangleDenominator=atan(tanvalueDenominator);
    }
    if (RealDenominator<0 && ImaginaryDenominator>0){
        tanangleDenominator=atan(tanvalueDenominator)+Pi;
    }
    else if    (RealDenominator<0 && ImaginaryDenominator<0){
        tanangleDenominator=atan(tanvalueDenominator)-Pi;

    }
    angleDenominator=tanangleDenominator*val;

// calculation of the "final" angle for the plot
    tanangle=angleNumerator-angleDenominator;
    return tanangle;
};

// gainmargin is calculated via bisection method
// gainmargin is defined as margin at -180° angle, thus the Zero of the function angle+180
// starting points are the angles at the lowest and highest frequencies given by the user
float Gainmargin(float NumeratorCoefficients[], float DenominatorCoefficients[],int DegreeNumerator, int DegreeDenominator,float w_upper, float w_lower){

    float frequency_left=w_lower; float frequency_right=w_upper; float frequency_middle;
    float angle_left, angle_right, difference=w_upper-w_lower, angle_middle, Gain_margin;

    angle_left=angle(NumeratorCoefficients,DenominatorCoefficients,DegreeNumerator, DegreeDenominator, frequency_left)+180;
    angle_right=angle(NumeratorCoefficients,DenominatorCoefficients,DegreeNumerator, DegreeDenominator, frequency_right)+180;

    // check if the function angle +180 has a zero, if not an arbitrary number is the result which will be used later to display "no gain margin"
    if (angle_left*angle_right>0){

        Gain_margin=-101;
    }
    else {
// bisection method halving the interval until the difference is below 0.1
        while (difference>=0.1){
            frequency_middle=(frequency_left+frequency_right)/2;
            angle_middle=angle(NumeratorCoefficients,DenominatorCoefficients,DegreeNumerator, DegreeDenominator, frequency_middle)+180;
            if (angle_left*angle_middle<0){
                frequency_right=frequency_middle;
            }
            else {
                frequency_left=frequency_middle;
            }
            difference=frequency_right-frequency_left;
        }
// gain margin at that frequency is calculated
        Gain_margin=Magnitude(NumeratorCoefficients,DenominatorCoefficients, DegreeNumerator, DegreeDenominator, frequency_middle);
    }
    return Gain_margin;
}

//-----------------------------------------------------------
// phasemargin is calculated via bisection method
// phasemargin is defined as margin at zero gain
// phasemargin points are the margins at the lowest and highest frequencies given by the user
float Phasemargin(float NumeratorCoefficients[], float DenominatorCoefficients[],int DegreeNumerator, int DegreeDenominator,float w_upper, float w_lower){

    float frequency_left=w_lower; float frequency_right=w_upper; float frequency_middle;
    float margin_left, margin_right, difference=w_upper-w_lower, margin_middle, Phase_margin;

    margin_left=20*log10(Magnitude(NumeratorCoefficients,DenominatorCoefficients,DegreeNumerator, DegreeDenominator, frequency_left));
    margin_right=20*log10(Magnitude(NumeratorCoefficients,DenominatorCoefficients,DegreeNumerator, DegreeDenominator, frequency_right));
// check if the margin function has a zero, if not an arbitrary number is the result which will be used later to display "no phase margin"
    if ( margin_left* margin_right>0){

        Phase_margin=-101;
    }
    else {
// bisection method halving the interval until the difference is below 0.1
        while (difference>=0.1){
            frequency_middle=(frequency_left+frequency_right)/2;
            margin_middle=20*log10(Magnitude(NumeratorCoefficients,DenominatorCoefficients,DegreeNumerator, DegreeDenominator, frequency_middle));
            if ( margin_left* margin_middle<0){
                frequency_right=frequency_middle;
            }
            else {
                frequency_left=frequency_middle;
            }
            difference=frequency_right-frequency_left;
        }

        Phase_margin=angle(NumeratorCoefficients,DenominatorCoefficients, DegreeNumerator, DegreeDenominator, frequency_middle)+180;
    }
    return Phase_margin;
}


// pushbutton to triggers getting user input
void MainWindow::on_pushButton_clicked()
{
// set up of the main window
    // getting user input from the boxes
    float w_lower, w_upper_unit, w_upper; int valid=0;
    QString input_text=ui->lineEdit->text();
    DegreeNumerator=input_text.toInt();
    QString input_text2=ui->lineEdit_2->text();
    DegreeDenominator=input_text2.toInt();
    QString input_text3=ui->lineEdit_3->text();
    w_lower=input_text3.toFloat();
    QString input_text4=ui->lineEdit_4->text();
    w_upper_unit=input_text4.toFloat();
    QString input_text10=ui->lineEdit_10->text();

    //Apparently QString can only handle 3 digit numbers
    // to be able to enter higher frequencies the "unit" field is implemented
    // and the upper frequency calculated

    if (input_text10=="kHz"){
        w_upper=w_upper_unit*1000;
           }
    else if (input_text10=="MHz"){
          w_upper=w_upper_unit*1000*1000;
         }
    else if (input_text10=="GHz"){
          w_upper=w_upper_unit*1000*1000*1000;
         }
    else
          w_upper=w_upper_unit;

//
    float NumeratorCoefficients[DegreeNumerator+1], DenominatorCoefficients[DegreeDenominator+1];
    QString input_text5=ui->lineEdit_5->text();
    QStringList input_list = input_text5.split(" ");
    for (int i = 0; i < DegreeNumerator+1; i++)
        NumeratorCoefficients[i]=input_list.at(i).toFloat();

    QString input_text6=ui->lineEdit_6->text();
    QStringList input_list2 = input_text6.split(" ");
    for (int j = 0; j < DegreeDenominator+1; j++)
        DenominatorCoefficients[j]=input_list2.at(j).toFloat();
//check if the given system is technical feasible
    if (DegreeNumerator<DegreeDenominator && w_lower<w_upper){

        valid=0;
         ui->lineEdit_9->setText("Valid Input");
    }

    else
    {
         ui->lineEdit_9->setText("Invalid Input");
        valid=1;
    }

if (valid==0){

    QVector<double> Magnitude_plot(9000), Angle_plot(9000), Frequency_plot(9000); int l=0;
// interval given by the user is separated into 9000 evenly spaced points, for each Magnitude and phase are calculated with the functions given above
    for (l=0;l<9000;l++){
            Frequency_plot[l]=w_lower+(w_upper-w_lower)*l/9000;
    Magnitude_plot[l]=20*log10(Magnitude(NumeratorCoefficients, DenominatorCoefficients,DegreeNumerator, DegreeDenominator,  Frequency_plot[l]));
    Angle_plot[l]=angle(NumeratorCoefficients, DenominatorCoefficients,DegreeNumerator, DegreeDenominator, Frequency_plot[l]);
    }
// calculation of the margins (Gain and Phase) with the functions given above
    float margin1, margin2;
    margin1=Gainmargin(NumeratorCoefficients, DenominatorCoefficients,DegreeNumerator, DegreeDenominator,w_upper, w_lower);
    margin2=Phasemargin(NumeratorCoefficients, DenominatorCoefficients,DegreeNumerator, DegreeDenominator,w_upper, w_lower);


    // create graph for gain and assign data to it:
    ui->CustomPlot->addGraph();
    ui->CustomPlot->graph(0)->setData(Frequency_plot, Magnitude_plot);
    // give the axes some labels:
    ui->CustomPlot->xAxis->setLabel("Frequency [Hz]");
    ui->CustomPlot->yAxis->setLabel("Magnitdue [dB]");
    // set axes ranges, so we see all data:
    ui->CustomPlot->xAxis->setRange(0, 10*w_upper);
    ui->CustomPlot->xAxis->setScaleType(QCPAxis::stLogarithmic);
     ui->CustomPlot->yAxis->setRange(-180, 90);
    ui->CustomPlot->replot();
     QSharedPointer<QCPAxisTickerLog> logTicker(new QCPAxisTickerLog);
    ui->CustomPlot->xAxis->setTicker(logTicker);
    ui->CustomPlot->xAxis->setNumberFormat("eb");
    ui->CustomPlot->xAxis->setNumberPrecision(0);

    // create graph for phase and assign data to it:
    ui->CustomPlot2->addGraph();
    ui->CustomPlot2->graph(0)->setData(Frequency_plot, Angle_plot);
    // give the axes some labels:
    ui->CustomPlot2->xAxis->setLabel("Frequency [Hz]");
    ui->CustomPlot2->yAxis->setLabel("Phase [Degree]");
    // set axes ranges, so we see all data:
    ui->CustomPlot2->xAxis->setRange(0, 10*w_upper);
    ui->CustomPlot2->xAxis->setScaleType(QCPAxis::stLogarithmic);
    ui->CustomPlot2->yAxis->setRange(-180, 180);
    ui->CustomPlot2->replot();
    ui->CustomPlot2->xAxis->setTicker(logTicker);
    ui->CustomPlot2->xAxis->setNumberFormat("eb");
    ui->CustomPlot2->xAxis->setNumberPrecision(0);

    // output of the gain and phase margin, value of -101 means no margin as per function above
    if (margin1==-101){
     ui->lineEdit_7->setText("No Phasemargin for function");
    }
    else {
     QString Phase_margin_output= QString::number(margin1);
     ui->lineEdit_7->setText(Phase_margin_output);
    }

    if (margin2==-101){
     ui->lineEdit_7->setText("No Gainmargin for function");
    }
    else {
     QString Gain_margin_output= QString::number(margin2);
     ui->lineEdit_8->setText(Gain_margin_output);
    }
}

}
// push button to safe plots as png

void MainWindow::on_pushButton_2_clicked()
{
    QCustomPlot *plot = ui->CustomPlot;

    QString filename = QFileDialog::getSaveFileName();
    plot->savePng( filename, plot->width(), plot->height() );

    QCustomPlot *plot2 = ui->CustomPlot2;

    QString filename2 = QFileDialog::getSaveFileName();
    plot2->savePng( filename2, plot->width(), plot->height() );

}

