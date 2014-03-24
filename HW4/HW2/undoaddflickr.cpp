#include "undoaddflickr.h"
#include "boximage.h"
#include "imagelabel.h"
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QImageReader>

using namespace std;

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

    box->clearGrid();
    box->fillGrid(oldImages);
}

void UndoAddFlickr::redo()
{
    qDebug() << "flickr redo called";

    box->clearGrid();
    box->fillGrid(newImages);

}
