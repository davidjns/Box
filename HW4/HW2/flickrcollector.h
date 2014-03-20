
#ifndef FLICKRCOLLECTOR_H
#define FLICKRCOLLECTOR_H

#include <QtGui>
#include <QtNetwork>
#include <QtXml>
#include <QTextEdit>

#include <iostream>
#include <string>


using namespace std;

#include <stdio.h>

class FlickrCollector: public QObject
{
  Q_OBJECT

 public:
    FlickrCollector(QWidget *w);
    void doDownload(const QUrl &url);
    void parseDomTree(QDomElement subroot);
    QStringList list(void);
    QString collectionName();

 protected:
    void parseDomTreeSub(int level, QDomElement parentElement);
    QDomDocument domTree;
    QTextEdit edit;
    QWidget *dparent;
    QStringList itemList;
    void printDomTreeSub(int level, QDomElement parentElement);
    QDate currentDate;
    QNetworkAccessManager *manager;

 public slots:
    void execute();
    void downloadFinished(QNetworkReply *reply);
    void printList(void);
    void printDomTree(QDomElement subRoot);

 signals:
    void ready(void);
    void failed(void);
};

#endif
