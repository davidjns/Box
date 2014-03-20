#ifndef UNDOCOPY_H
#define UNDOCOPY_H

class BoxImage;
class ImageLabel;

#include <QUndoCommand>

class UndoCopy : public QUndoCommand
{
public:
    UndoCopy(BoxImage *_box, ImageLabel* image);

    void undo();
    void redo();

private:
    BoxImage *box;
    ImageLabel *copiedImage;
    ImageLabel *prevCopied;
};

#endif // UNDOCOPY_H
