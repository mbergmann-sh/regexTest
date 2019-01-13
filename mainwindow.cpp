/****************************************************************************
**
** Copyright (C) 2018/2019 Michael Bergmann. Placed in the public domain
** with some restrictions!
**
** This file is part of my enhanced AmigaED editor example, using classes of
** the Qt and QScintilla toolkits.
**
** You may use or enhance this piece of software anyway you want to - as long
** as you don't violate laws or copyright issues.
** I hereby explicitely prohibit the usage of my work for people who believe
** in racism, fascism and any kind of attitude against democratic lifestyle.
** It is self-explanatory that this prohibits the usage of my work to any
** member or fan of the german AfD party.
**
** Further information about Qt licensing is available at:
** http://www.trolltech.com/products/qt/licensing.html or by
** contacting info@trolltech.com.
**
** Further information about QScintilla licensing is available at:
** https://www.riverbankcomputing.com/software/qscintilla/license or by
** contacting sales@riverbankcomputing.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Compiler RegEX Error Line Finder");
    ui->centralWidget->activateWindow();
    ui->comboBox->setCurrentIndex(0);
    ui->lcdNumber_Column->setDisabled(true);
    ui->btn_GO->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_GO_clicked()
{
    resetOutput();
    switch(ui->comboBox->currentIndex())
    {
        case 0:
            checkVBCC();
            break;

        case 1:
        case 2:
            checkGCC();
            break;
    }    
}

void MainWindow::on_btn_Clear_clicked()
{    
    ui->lineEdit_Searchstring->clear();
    resetOutput();
    ui->lineEdit_Searchstring->setFocus();
}

void MainWindow::checkVBCC()
{
//    $txt='error 0 in line 25 of "/home/bergmann/Dokumente/Qt5/gitAmigaED/build-application-Desktop-Release/jumptest.c": declaration expected';

//    $re1='((?:[a-z][a-z]+))';	# Word 1
//    $re2='.*?';	# Non-greedy match on filler
//    $re3='(?:[a-z][a-z]+)';	# Uninteresting: word
//    $re4='.*?';	# Non-greedy match on filler
//    $re5='((?:[a-z][a-z]+))';	# Word 2
//    $re6='.*?';	# Non-greedy match on filler
//    $re7='(\\d+)';	# Integer Number 1
//    $re8='.*?';	# Non-greedy match on filler
//    $re9='((?:\\/[\\w\\.\\-]+)+)';	# Unix Path 1

//    $re=$re1.$re2.$re3.$re4.$re5.$re6.$re7.$re8.$re9;
//    if ($txt =~ m/$re/is)
//    {
//        $word1=$1;
//        $word2=$2;
//        $int1=$3;
//        $unixpath1=$4;
//        print "($word1) ($word2) ($int1) ($unixpath1) \n";
//    }
    QString search ="error 0 in line 25 of \"/home/bergmann/Dokumente/Qt5/gitAmigaED/build-application-Desktop-Release/jumptest.c\": declaration expected'; ";
    QString re1="((?:[a-z][a-z]+))";	// Word 1
    QString re2=".*?";	//# Non-greedy match on filler
    QString re3="(?:[a-z][a-z]+)";	//# Uninteresting: word
    QString re4=".*?";	//# Non-greedy match on filler
    QString re5="((?:[a-z][a-z]+))";	//# Word 2
    QString re6=".*?";	//# Non-greedy match on filler
    QString re7="(\\d+)";	// # Integer Number 1
    QString re8=".*?";	//# Non-greedy match on filler
    QString re9="((?:\\/[\\w\\.\\-]+)+)";	//# Unix Path 1
    QStringList strlist;
    strlist << re1 << re2 << re3 << re4 << re5 << re6 << re7 << re8 << re9;
    QString reg = re1 + re2 + re3 + re4 + re5 + re6 + re7 + re8 + re9;

    ui->lcdNumber_Column->setDisabled(true);
    qDebug() << "Compiler selection index: " << ui->comboBox->currentIndex();
    qDebug() << "checkVBCC() called";

    QString str_to_search = ui->lineEdit_Searchstring->text();
    if(!(str_to_search.isEmpty()))
    {
        QRegularExpression rx_line("(\\d+)");                       // check for line (and column, if available)
        QRegularExpression rx_words("((?:\"[a-z][a-z]+\\.c)\")");   // check for ALL strings starting with \", containing .c, ending with \"
        QRegularExpression rx_file("((?:\\/[\\w\\.\\-]+)+)");       // unix path
        QRegularExpression rx_messagetype("(error)");               // check if error or warning


        QList<int> list;    // List to hold search results (integer values)
        QStringList list1;  // List to hold string-type results

        //
        // check if error or warning
        //
        QRegularExpressionMatchIterator i = rx_messagetype.globalMatch(str_to_search);
        while (i.hasNext())
        {
            QRegularExpressionMatch match = i.next();
            QString word = match.captured(1);
            list1 << word;
        }

        if(!(list1.isEmpty()))
        {
            if(list1[0] == "error")
            {
                ui->label_MessageType->setText("Error");
            }
            else
            {
                ui->label_MessageType->setText("unknown");
            }

            qDebug() << "List entries: " << list1 << "List count: " << list1.count();

        }
        else
        {
            ui->label_MessageType->setText("Warning");
            qDebug() << "No matches!";
        }

        //
        // check for simple filename
        //
        list1.clear();
        QRegularExpressionMatchIterator w = rx_words.globalMatch(str_to_search);
        while (w.hasNext())
        {
            QRegularExpressionMatch match = w.next();
            QString word = match.captured(1);
            list1 << word;
        }

        if(!(list1.isEmpty()))
        {
            qDebug() << "List entries: " << list1 << "List count: " << list1.count();
            ui->lineEdit_Filepath->setText(list1[0]);
        }
        else
            qDebug() << "no simple filename";


        //
        // check for unix path
        //
        list1.clear();
        QRegularExpressionMatchIterator f = rx_file.globalMatch(str_to_search);
        while (f.hasNext())
        {
            QRegularExpressionMatch match = f.next();
            QString word = match.captured(1);
            list1 << word;
        }

        if(!(list1.isEmpty()))
        {
            qDebug() << "List entries: " << list1 << "List count: " << list1.count();
            ui->lineEdit_Filepath->setText(list1[0]);
        }
        else
            qDebug() << "no unix path!";



        //
        // check for line and column
        //
        QRegularExpressionMatchIterator lc = rx_line.globalMatch(str_to_search);
        while (lc.hasNext())
        {
            QRegularExpressionMatch match = lc.next();
            QString word = match.captured(1);
            list << word.toInt();
        }

        if(!(list.isEmpty()))
        {
            int index = 0;
            qDebug() << "List entries: " << list << "List count: " << list.count();
            if(index <= list.count() -1)
                ui->label_MessageNumber->setText(QString::number(list[index]));

            ++index;

            if(index <= list.count() -1)
                ui->lcdNumber_Line->display(QString::number(list[index]));

        }
        else
            qDebug() << "WTF!";

    } // END str_to_search.isEmpty()
}

void MainWindow::checkGCC()
{
    ui->lcdNumber_Column->setDisabled(false);
    qDebug() << "Compiler selection index: " << ui->comboBox->currentIndex();
    qDebug() << "checkGCC() called";

    QString search ="error 0 in line 25 of \"/home/bergmann/Dokumente/Qt5/gitAmigaED/build-application-Desktop-Release/jumptest.c\": declaration expected'; ";
    QString re1="((?:[a-z][a-z]+))";	// Word 1
    QString re2=".*?";	//# Non-greedy match on filler
    QString re3="(?:[a-z][a-z]+)";	//# Uninteresting: word
    QString re4=".*?";	//# Non-greedy match on filler
    QString re5="((?:[a-z][a-z]+))";	//# Word 2
    QString re6=".*?";	//# Non-greedy match on filler
    QString re7="(\\d+)";	// # Integer Number 1
    QString re8=".*?";	//# Non-greedy match on filler
    QString re9="((?:\\/[\\w\\.\\-]+)+)";	//# Unix Path 1
    QStringList strlist;
    strlist << re1 << re2 << re3 << re4 << re5 << re6 << re7 << re8 << re9;
    QString reg = re1 + re2 + re3 + re4 + re5 + re6 + re7 + re8 + re9;

    ui->lcdNumber_Column->setDisabled(true);
    qDebug() << "Compiler selection index: " << ui->comboBox->currentIndex();
    qDebug() << "checkVBCC() called";

    QString str_to_search = ui->lineEdit_Searchstring->text();
    if(!(str_to_search.isEmpty()))
    {
        QRegularExpression rx_line("(\\d+)");                       // check for line (and column, if available)
        QRegularExpression rx_words("((?:[a-z][a-z]+\\.c))");       // check for ALL strings containing .c
        QRegularExpression rx_file("((?:\\/[\\w\\.\\-]+)+)");       // unix path
        QRegularExpression rx_messagetype("(error)");               // check if error or warning


        QList<int> list;    // List to hold search results (integer values)
        QStringList list1;  // List to hold string-type results

        //
        // check if error or warning
        //
        QRegularExpressionMatchIterator i = rx_messagetype.globalMatch(str_to_search);
        while (i.hasNext())
        {
            QRegularExpressionMatch match = i.next();
            QString word = match.captured(1);
            list1 << word;
        }

        if(!(list1.isEmpty()))
        {
            if(list1[0] == "error")
            {
                ui->label_MessageType->setText("Error");
            }
            else
            {
                ui->label_MessageType->setText("Warning");
            }

            qDebug() << "List entries: " << list1 << "List count: " << list1.count();

        }
        else
        {
            ui->label_MessageType->setText("Warning");
            qDebug() << "No matches!";
        }

        //
        // check for simple filename
        //
        list1.clear();
        QRegularExpressionMatchIterator w = rx_words.globalMatch(str_to_search);
        while (w.hasNext())
        {
            QRegularExpressionMatch match = w.next();
            QString word = match.captured(1);
            list1 << word;
        }

        if(!(list1.isEmpty()))
        {
            qDebug() << "List entries: " << list1 << "List count: " << list1.count();
            ui->lineEdit_Filepath->setText(list1[0]);
        }
        else
            qDebug() << "WTF!";

        //
        // check for unix path
        //
        list1.clear();
        QRegularExpressionMatchIterator f = rx_file.globalMatch(str_to_search);
        while (f.hasNext())
        {
            QRegularExpressionMatch match = f.next();
            QString word = match.captured(1);
            list1 << word;
        }

        if(!(list1.isEmpty()))
        {
            qDebug() << "List entries: " << list1 << "List count: " << list1.count();
            ui->lineEdit_Filepath->setText(list1[0]);
        }
        else
            qDebug() << "WTF!";

        //
        // check for line and column
        //
        QRegularExpressionMatchIterator lc = rx_line.globalMatch(str_to_search);
        while (lc.hasNext())
        {
            QRegularExpressionMatch match = lc.next();
            QString word = match.captured(1);
            list << word.toInt();
        }

        if(!(list.isEmpty()))
        {
            int index = 0;
            qDebug() << "List entries: " << list << "List count: " << list.count();
            if(index <= list.count() -1)
            {
                ui->label_MessageNumber->setText("gcc/g++ is unaware of error numbers!");
                ui->lcdNumber_Line->display(QString::number(list[index]));
            }


            ++index;

            if(index <= list.count() -1)
                ui->lcdNumber_Column->display(QString::number(list[index]));

        }
        else
            qDebug() << "WTF!";

    } // END str_to_search.isEmpty()
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    qDebug() << "Index: " << index;
    resetOutput();
    switch(index)
    {
        case 0:
            ui->lineEdit_Searchstring->setText("error 0 in line 25 of \"/home/bergmann/Dokumente/Qt5/gitAmigaED/build-application-Desktop-Release/jumptest.c\": declaration expected'; ");
            break;
        case 1:
            ui->lineEdit_Searchstring->setText("jumptest.c:38:9: error: expected expression before ')' token");
            break;
        case 2:
            ui->lineEdit_Searchstring->setText("jumptest.c:38:9: warning: expected expression before ')' token");
            break;
    }
}

//
// HELPER: reset all output
//
void MainWindow::resetOutput()
{
    ui->label_MessageType->setText("not computed yet..");
    ui->label_MessageNumber->setText("not computed yet..");
    ui->lineEdit_Filepath->clear();
    ui->lcdNumber_Column->display(0);
    ui->lcdNumber_Line->display(0);
    ui->btn_GO->setFocus();
}
