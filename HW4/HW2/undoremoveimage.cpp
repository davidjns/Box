#include "undoremoveimage.h"
#include "imagelabel.h"
#include "boximage.h"
#include <algorithm>
#include <vector>
#include <QLabel>

using namespace std;

UndoRemoveImage::UndoRemoveImage(BoxImage *_box, vector<ImageLabel*> _images) :
    box(_box),
    oldImages(_images)
{
    ImageLabel *image;
    ImageLabel *selectedImage = box->getSelected();

    try
    {
        int num = box->numImages;
        for(int i = 0; i < num; i++)
        {
            image = box->images.at(i);

            if(image != selectedImage)
                newImages.push_back(image);
        }
    }
    catch(...)
    {
        qDebug("undo single image: index issue");
    }
}

void UndoRemoveImage::undo()
{
    clearGrid();
    fillGrid(oldImages);
    box->fixBox();
}

void UndoRemoveImage::redo()
{
    clearGrid();
    fillGrid(newImages);
    box->fixBox();
    box->resetSelected();
}

void UndoRemoveImage::fillGrid(vector<ImageLabel*> images)
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
        qDebug("Remove Single fillGrid: indexing error");
    }
}

void UndoRemoveImage::clearGrid()
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
        qDebug("Remove Single clearGrid: indexing error");
    }
}
