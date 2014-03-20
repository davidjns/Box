#ifndef UNDOCUT_H
#define UNDOCUT_H

class BoxImage;
class ImageLabel;

#include <QUndoCommand>
#include <vector>

class UndoCut : public QUndoCommand
{
public:
    UndoCut(BoxImage *_box, std::vector<ImageLabel*> _images);

    void undo();
    void redo();

private:
    void clearGrid();
    void fillGrid(std::vector<ImageLabel*> images);

    BoxImage *box;
    ImageLabel *copiedImage;
    ImageLabel *prevCopied;

    std::vector<ImageLabel*> oldImages;
    std::vector<ImageLabel*> newImages;
};

#endif // UNDOCUT_H
