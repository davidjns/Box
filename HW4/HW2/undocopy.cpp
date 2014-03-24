#include "undocopy.h"
#include "boximage.h"
#include "imagelabel.h"

UndoCopy::UndoCopy(BoxImage *_box, ImageLabel *image) :
    box(_box)
{
    prevCopied = box->getCopied();

    copiedImage = new ImageLabel(box);
    copiedImage->setPixmap(*(image->pixmap()));
}

void UndoCopy::undo()
{
    box->setCopied(prevCopied);
}

void UndoCopy::redo()
{
    box->setCopied(copiedImage);
}
