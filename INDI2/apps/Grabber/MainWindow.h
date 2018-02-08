#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QUrl>

#include <eirCore/VariableSet.h>
#include <eirExe/GuiApplication.h>
#include <eirExe/Results.h>
#include <eirImage/ImageEntity.h>

class QQDir;
class CheckAction;
class ImageLabel;
class FrameSourceManager;
class FrameSourcePlugin;
class FrameSourceDirect;

class MainWindow : public GuiApplication
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void initialize(void);
    void setupUi(void);
    void initializeManager(void);
    void checkConfig(void);
    void autoStart(const VariableSet & config);
    void configured(ResultSet rset);
    void connected(ResultSet rset);
    void started(ResultSet rset);
    void complete(ResultSet rset);
    void takeGrab(int key);
    void stopStart(bool checked);
    void pauseResume(bool checked);
    void abort(void);

    void optWatch(QString vid,
                  QVariant var);
    void connectUrl(void);
    void autoStop(void);
    void warning(Result res);
    void showImage(ImageEntity ie);

private:
    ImageLabel * main_imageLabel;
//    CheckAction * stopStart_action;
    CheckAction * pauseResume_action;
    QAction * abort_action;
    QUrl _url;
    FrameSourceManager * _manager;
    FrameSourcePlugin * _plugin;
    FrameSourceDirect * _interface;
    int channel_i;
    QString beginTime_s;
    QString endTime_s;
    QQDir * output_dir;
};

#endif // MAINWINDOW_H
