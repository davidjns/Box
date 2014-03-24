#include "undocut.h"
#include "boximage.h"
#include "imagelabel.h"

using namespace std;

UndoCut::UndoCut(BoxImage *_box, vector<ImageLabel*> _images) :
    box(_box),
    oldImages(_images)
{
    copiedImage = new ImageLabel(box);
    copiedImage->setPixmap(*(box->getSelected()->pixmap()));

    prevCopied = new ImageLabel(box);
    prevCopied->setPixmap(*(box->getCopied())->pixmap());

    ImageLabel *image;
    try
    {
        int num = box->numImages;
        for(int i = 0; i < num; i++)
        {
            image = box->images.at(i);

            if(image != box->getSelected())
                newImages.push_back(image);
        }
    }
    catch(...)
    {
        qDebug("undo cut: index problem");
    }
}

void UndoCut::undo()
{
    box->setCopied(prevCopied);

    clearGrid();
    fillGrid(oldImages);
    box->fixBox();
}

void UndoCut::redo()
{
    box->setCopied(copiedImage);

    clearGrid();
    fillGrid(newImages);
    box->fixBox();
    box->resetSelected();
}

void UndoCut::fillGrid(vector<ImageLabel*> images)
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
        qDebug("UndoCut fillGrid: indexing error");
    }
}

void UndoCut::clearGrid()
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
        qDebug("UndoCut clearGrid: indexing error");
    }
}
