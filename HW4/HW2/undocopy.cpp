#include "undocopy.h"
#include "boximage.h"
#include "imagelabel.h"

UndoCopy::UndoCopy(BoxImage *_box, ImageLabel *image) :
    box(_box),
    copiedImage(image)
{
    prevCopied = box->getCopied();
}

void UndoCopy::undo()
{
    box->setCopied(prevCopied);
}

void UndoCopy::redo()
{
    box->setCopied(copiedImage);
}
