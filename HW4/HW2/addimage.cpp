#include "addimage.h"
#include <QLabel>

AddImage::AddImage(BoxImage *_box, QLabel *_image) :
    QUndoCommand(),
    box(_box),
    image(_image)
{

}

void AddImage::undo()
{
    qDebug("undo called");
    if(!image)
    {
        qDebug("null image");
        return;
    }

    image->hide();
    box->removeWidget(image);

    box->images.pop_back();
    box->numImages--;
}

void AddImage::redo()
{
    qDebug("redo called");
    image->show();
    box->addWidget(image, (box->numImages/5)+1, (box->numImages%5)+1);
    box->images.push_back(image);
    box->numImages++;
}
