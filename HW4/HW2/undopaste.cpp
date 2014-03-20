#include "undopaste.h"
#include "boximage.h"
#include "imagelabel.h"
#include <QPixmap>
#include <vector>

using namespace std;

UndoPaste::UndoPaste(BoxImage *_box, vector<ImageLabel*> _images, ImageLabel *_copied) :
    box(_box),
    oldImages(_images)
{
    ImageLabel *selected = box->getSelected();


    copiedImage = new ImageLabel(box);
    const QPixmap *copiedPixmap = _copied->pixmap();
    copiedImage->setPixmap(*copiedPixmap);

    if(copiedImage == NULL)
        qDebug("null copy");
    ImageLabel *image;
    try
    {
        int num = box->numImages;
        for(int i = 0; i < num; i++)
        {
            image = box->images.at(i);

            if(image == selected)
                newImages.push_back(copiedImage);
            newImages.push_back(image);
        }
        if(selected == NULL)
            newImages.push_back(copiedImage);
    }
    catch(...)
    {
        qDebug("UndoPaste ctor: index error");
    }
}

void UndoPaste::undo()
{
    clearGrid();
    fillGrid(oldImages);
    box->fixBox();
}

void UndoPaste::redo()
{
    clearGrid();
    fillGrid(newImages);
    box->fixBox();
}

void UndoPaste::fillGrid(vector<ImageLabel*> images)
{
    ImageLabel *image;
    try
    {
        int num = (int)images.size();
        for(int i = 0; i < num; i++)
        {
            image = images.at(i);
            image->show();
            box->addWidget(image, (i/5)+1, (i%5)+1);
            box->images.push_back(image);
            box->numImages++;
        }
    }
    catch(...)
    {
        qDebug("UndoPaste fillGrid: indexing error");
    }
}

void UndoPaste::clearGrid()
{
    ImageLabel *image;
    try
    {
        int num = box->numImages;
        for(int i = (num - 1); i >= 0; i--)
        {
            image = box->images.at(i);

            image->hide();
            box->removeWidget(image);
            box->images.pop_back();
            box->numImages--;
        }
    }
    catch(...)
    {
        qDebug("UndoPaste clearGrid: indexing error");
    }
}
