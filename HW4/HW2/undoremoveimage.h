#ifndef UNDOREMOVEIMAGE_H
#define UNDOREMOVEIMAGE_H

class BoxImage;
class ImageLabel;
class QLabel;

#include <vector>
#include <QUndoCommand>

class UndoRemoveImage : public QUndoCommand
{
public:
    UndoRemoveImage(BoxImage *_box, std::vector<ImageLabel*> _oldImages);

    void undo();
    void redo();

private:
    void clearGrid();
    void fillGrid(std::vector<ImageLabel*> images);

    BoxImage *box;
    std::vector<ImageLabel*> oldImages;
    std::vector<ImageLabel*> newImages;

};

#endif // UNDOREMOVEIMAGE_H
