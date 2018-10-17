#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QDebug>
#include <QTime>
#include <QSplashScreen>
#include "windows.h"
#include "global.h"


bool checkOnly(){
    //  创建互斥量
    HANDLE m_hMutex  =  CreateMutex(NULL, FALSE,  L"AGVFactory" );
    //  检查错误代码
    if  (GetLastError()  ==  ERROR_ALREADY_EXISTS)  {
      //  如果已有互斥量存在则释放句柄并复位互斥量
     CloseHandle(m_hMutex);
     m_hMutex  =  NULL;
      //  程序退出
      return  false;
    }
    else
        return true;
}

int main(int argc, char *argv[])
{

  // QApplication::addLibraryPath("./plugins");
   // QStringList paths = QApplication::libraryPaths();

   // paths.append("./plugins");
   // paths.append("./");
   // QApplication::setLibraryPaths(paths);
    QApplication a(argc, argv);
    if(checkOnly()==false){
        return 0;
    }
    QSplashScreen *splash = new QSplashScreen(QPixmap(":/image/images/splash.png"));
    splash->setDisabled(true);
    splash->show();
    global_init();
    MainWindow w;
    w.showMaximized();
    splash->finish(&w);
    delete splash;
    return a.exec();
}
