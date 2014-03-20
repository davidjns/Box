#include "undoaddimages.h"
#include "imagelabel.h"
#include "boximage.h"
#include <QLabel>

using namespace std;

UndoAddImages::UndoAddImages(BoxImage *_box, vector<ImageLabel*> _images) :
    box(_box),
    images(_images)
{}

void UndoAddImages::undo()
{
    try
    {
        int num = (int)images.size();
        for(int i = (num - 1); i >= 0; i--)
        {
            ImageLabel *image = images.at(i);

            image->hide();
            box->removeWidget(image);
            box->images.pop_back();
            box->numImages--;
        }
    }
    catch(...)
    {
        qDebug("Add Undo: indexing error");
    }

    box->fixBox();
}

void UndoAddImages::redo()
{
    ImageLabel *currImage;
    try
    {
        int num = (int)images.size();
        for(int i = 0; i < num; i++)
        {
            currImage = images.at(i);
            currImage->show();

            box->addWidget(currImage, (box->numImages/5)+1, (box->numImages%5)+1);
            box->images.push_back(currImage);
            box->numImages++;
        }
    }
    catch(...)
    {
        qDebug("Add Redo: indexing error");
    }

    box->fixBox();
}
