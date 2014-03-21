#include "undoaddflickr.h"
#include "boximage.h"
#include "imagelabel.h"
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QImageReader>

using namespace std;

UndoAddFlickr::UndoAddFlickr(BoxImage *_box, QStringList _urls, vector<ImageLabel*> _oldImages) :
    oldImages(_oldImages),
    newImages(_oldImages),
    box(_box),
    urls(_urls)
{
//    numFinished = 0;
//    undone = false;

//    nam = new QNetworkAccessManager(box);
//    QObject::connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

//    int size = urls.size();
//    for(int i = 0; i < size; i++)
//    {
//        QString string = urls.front();
//        urls.pop_front();
//        QUrl url(string);
//        QNetworkReply *reply = nam->get(QNetworkRequest(url));
//    }
}

UndoAddFlickr::UndoAddFlickr(BoxImage *_box, std::vector<ImageLabel *> _oldImages, std::vector<ImageLabel *> _newImages):
    oldImages(_oldImages),
    box(_box)
{
   newImages = oldImages;

   int size = _newImages.size();
   for(int i = 0; i < size; i++)
   {
       newImages.push_back(_newImages.at(i));
   }
}

void UndoAddFlickr::undo()
{
    qDebug() << "flickr undo called";
//    undone = true;
    box->clearGrid();
    box->fillGrid(oldImages);
}

void UndoAddFlickr::redo()
{
    qDebug() << "flickr redo called";
//    undone = false;
    box->clearGrid();
    box->fillGrid(newImages);

//    if(numFinished == 10)
//    {
//        box->fillGrid(newImages);
//        //TODO: box->fixBox();
//    }
}

void UndoAddFlickr::replyFinished(QNetworkReply *reply)
{
//    numFinished++;
//    QImageReader imageReader(reply);
//    imageReader.setAutoDetectImageFormat(true);
//    QImage image = imageReader.read();

//    ImageLabel *newImage = new ImageLabel(box);
//    QPixmap pix = QPixmap::fromImage(image);
//    newImage->setPixmap(pix.scaledToWidth(150));

//    newImages.push_back(newImage);

//    if(numFinished == 10)
//    {
//        box->setFlickrReady();
//        if(!undone)
//        {
//            box->fillGrid(newImages);
//            //FIXBOX()
//        }
//    }
}
