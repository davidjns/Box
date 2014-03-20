#include "undoaddflickr.h"
#include "boximage.h"
#include "imagelabel.h"
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkReply>

using namespace std;

UndoAddFlickr::UndoAddFlickr(BoxImage *_box, QStringList _urls, vector<ImageLabel*> _oldImages) :
    oldImages(_oldImages),
    newImages(_oldImages),
    box(_box),
    urls(_urls)
{
    numFinished = 0;
    undone = false;

    nam = new QNetworkAccessManager(this);
    connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

    int size = urls.size();
    for(int i = 0; i < size; i++)
    {
        QString string = urls.front();
        urls.pop_front();
        QUrl url(string);
        QNetworkReply *reply = nam->get(QNetworkRequest(url));
    }
}

void UndoAddFlickr::undo()
{
    undone = true;
    box->clearGrid();
    box->fillGrid(oldImages);
}

void UndoAddFlickr::redo()
{
    undone = false;
    box->clearGrid();

    if(numFinished == 10)
    {
        box->fillGrid(newImages);
        //TODO: box->fixBox();
    }
}

void UndoAddFlickr::replyFinished(QNetworkReply *reply)
{
    numFinished++;
    QImageReader imageReader(reply);
    imageReader.setAutoDetectImageFormat(true);
    QImage image = imageReader.read();

    ImageLabel *newImage = new ImageLabel(this);
    QPixmap pix = QPixmap::fromImage(image);
    newImage->setPixmap(pix.scaledToWidth(150));

    newImages.push_back(newImage);

    if(numFinished == 10)
    {
        box->setFlickrReady();
        if(!undone)
        {
            box->fillGrid(newImages);
            //FIXBOX()
        }
    }
}
