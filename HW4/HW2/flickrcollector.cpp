/****************************************************************************
**
** Copyright (C) 2008-2013 University of Michigan
**
** GNU General Public License Usage
** This file may be used under the terms of the GNU
** General Public License versions 2.0 or 3.0 as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file.  Please review the following information
** to ensure GNU General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.
**
** Based on Qt example code.
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
****************************************************************************/

/**************************************************************************

Singleton class to retrieve collections of images from Flickr

Mark Ackerman, February 2014
EECS 493

To use:

1. Replace the missing key in the SEARCH_URL_START string below with
   the API key you obtain from Flickr.

2.  Create an instance of FlickrCollector.

3.  Call execute().  It will emit a ready() signal when it's finished
    downloading.

4.  Get the list of urls as a QStringList by calling list().

The search string for the retrieved set is arbitrary.  You can change
it if you want.  This currently retrieves the first 10
"interestingness" images for days in reverse order.  This uses a
separate Flickr API call from other, tagged searches.  See the Flickr
API spec for details.

**************************************************************************/

#include <QtGui>
#include <QtNetwork>
#include <QtXml>

#include "flickrcollector.h"

#include <iostream>
#include <string>


using namespace std;

#include <stdio.h>

static const int MAX_IMAGES = 10; // if this is changed, change the query string

/* First create a FlickrCollector to obtain lists of FLickr images' urls. */
FlickrCollector::FlickrCollector(QWidget *parent_incoming)
{
  dparent = parent_incoming; // need this for popping up a dialog box
  manager = new QNetworkAccessManager();
  connect(manager, SIGNAL(finished(QNetworkReply*)),
      SLOT(downloadFinished(QNetworkReply*)));
  currentDate = QDate::currentDate();
  // bug in Flickr where previous day isn't available around 1am
  currentDate = currentDate.addDays(-2);
}


/******  CHANGE THE API KEY HERE *********/
const char *SEARCH_URL_START = "http://api.flickr.com/services/rest/?format=rest&method=flickr.interestingness.getList&per_page=10&api_key=c9e359c595991a191dea24c0b2408a06&date=";

const char *SEARCH_URL_END = "";

/* To obtain a list of images, call execute().  It will emit a ready()
   signal when it's done downloading the list.  Then retrieve the list
   with list(). */

void FlickrCollector::execute()
{
  QString tempStr;
  QString urlStr;

  /* if you want to change the algorithm for how search is done,
     this is the location.  currently, it's just picking up MAX_IMAGES
     from the specified day.*/
  currentDate = currentDate.addDays(-1);
  urlStr.append(SEARCH_URL_START);
  urlStr.append(currentDate.toString(Qt::ISODate));

  // if you want to search for tags, put them here */
  /*
  tempStr.simplified();
  tempStr.replace(" ","+");
  urlStr.append(tempStr);
  urlStr.append(SEARCH_URL_END);
  */

  QUrl url = QUrl(urlStr);
  doDownload(url);
}

void FlickrCollector::doDownload(const QUrl &url)
{
    QNetworkRequest request(url);
    QNetworkReply *reply = manager->get(request);
}


void FlickrCollector::downloadFinished(QNetworkReply *reply)
{
  QUrl url = reply->url();
  if (reply->error())
    {
      // error message to user
      QString tempStr;
      tempStr.append(tr("download of "));
      tempStr.append(url.toEncoded().constData());
      tempStr.append(tr("failed: "));
      tempStr.append(reply->errorString());
      tempStr.append(tr(".  Try again in a minute."));
      qDebug() << tempStr << endl;
    }
  else
    {
      if (!domTree.setContent(reply->readAll()))
    {
      qDebug() << tr("parsing error for search results. Continuing....");
    }
      else
    {
      QDomElement root = domTree.documentElement();
      try {
      parseDomTree(root);
      } catch (...) {
        emit failed();
      }
      //	  domTree->deleteLater();
      emit ready();
    }
    }
  reply->deleteLater();
}

/* Get a name for the retrieved set.  This is an arbitrary string.
   You can change it. */
QString FlickrCollector::collectionName()
{
  QString nameStr("Interesting ");
  nameStr.append(currentDate.toString(Qt::ISODate));
  return nameStr;
}

/* After the retrieval is done, get the returned urls here as a
   QStringList */
QStringList FlickrCollector::list()
{
  return itemList;
}


/* routine for examining the url list.*/
void FlickrCollector::printList()
{
  for (int i=0;i<itemList.size();i++)
    cout << itemList.at(i).toStdString() << endl;
}

void FlickrCollector::parseDomTree(QDomElement subRoot)
{
  itemList.clear();

  //printDomTree(subRoot);
  parseDomTreeSub(1,subRoot);
}

void FlickrCollector::parseDomTreeSub(int level, QDomElement parentElement)
{
  QDomNode node;
  // a bit inefficient declaring these here
  bool farmFlag = false;
  bool secretFlag = false;
  bool idFlag = false;
  bool serverFlag = false;
  QString serverStr;
  QString farmStr;
  QString idStr;
  QString secretStr;
  QString urlStr;

  node = parentElement.firstChild();

  while ( !node.isNull() )
    {
      if (node.nodeName() == "photo")
    {
      QDomNamedNodeMap attributes = node.attributes();
      for(int i = 0; i<attributes.size(); ++i)
        {
          QDomAttr attribute = attributes.item(i).toAttr();
          if (attribute.name() == "farm" && !farmFlag)
        {
          farmStr = attribute.value();
          farmFlag = true;
        }
          else if (attribute.name() == "server" && !serverFlag)
        {
          serverStr = attribute.value();
          serverFlag = true;
        }
          else if (attribute.name() == "id" && !idFlag)
        {
          idStr = attribute.value();
          idFlag = true;
        }
          else if (attribute.name() == "secret" && !secretFlag)
        {
          secretStr = attribute.value();
          secretFlag = true;
        }
        } /* for */
      urlStr.clear();
      urlStr.append("http://farm").append(farmStr)
        .append(".staticflickr.com/");
      urlStr.append(serverStr).append("/");
      urlStr.append(idStr).append("_").append(secretStr)
        .append("_z.jpg");

      itemList.append(urlStr);

      idFlag = secretFlag = farmFlag = serverFlag = false;
      idStr.clear();
      farmStr.clear();
      serverStr.clear();
      secretStr.clear();
    }
      QDomElement body = node.toElement();
      parseDomTreeSub(level+1, body);
      node = node.nextSibling();
    }
}

/* routine for examining the dom tree.*/
void FlickrCollector::printDomTree(QDomElement subRoot)
{
  printDomTreeSub(1, subRoot);
}

void FlickrCollector::printDomTreeSub(int level, QDomElement parentElement)
{
  QDomNode node;

  node = parentElement.firstChild();

  while ( !node.isNull() )
    {
      for (int j=0;j<level;j++)
    cout << " ";
      cout << "Node nodename is: " << node.nodeName().toStdString()
       << ", value is: " << node.nodeValue().toStdString()
       << ", type is: "<< node.nodeType() << endl;
      QDomNamedNodeMap attributes = node.attributes();
      for(int i = 0; i<attributes.size(); ++i)
    {
      QDomAttr attribute = attributes.item(i).toAttr();
      for (int j=0;j<level+1;j++)
        cout << " ";
      cout << "attr" << i << " name: " << attribute.name().toStdString()
           << ", value: " << attribute.value().toStdString() << endl;
    }
      QDomElement body = node.toElement();
      printDomTreeSub(level+1, body);
      node = node.nextSibling();
    }
}
