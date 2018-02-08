#ifndef VIGILCAP_H
#define VIGILCAP_H
#define __MODULE__ "VigilCap"

#include <QtCore/QUrl>

#include <eirExe/Console.h>
#include <eirFile/QQDir.h>

class VigilDvr;
class VigilLiveFeed;
class VigilPlayback;


class VigilCap : public Console
{
    Q_OBJECT
public:
    explicit VigilCap(int argc, char ** argv);
    
signals:
    
public slots:

private slots:
    void initialize(void);
    void login(void);
    void start(void);
    void liveStarted(void);
    void playStarted(void);
    void playNext(void);
    void playFinished(void);

private:
    QUrl vigil_url;
    QQDir output_dir;
    VigilDvr * vigil_dvr;
    VigilLiveFeed * vigil_live;
    VigilPlayback * vigil_play;
};

#endif // VIGILCAP_H
