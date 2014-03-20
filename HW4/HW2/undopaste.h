#ifndef UNDOPASTE_H
#define UNDOPASTE_H

class BoxImage;
class ImageLabel;

#include <QUndoCommand>
#include <vector>

class UndoPaste : public QUndoCommand
{
public:
    UndoPaste(BoxImage *_box, std::vector<ImageLabel*> _images, ImageLabel *_copied);

    void undo();
    void redo();

private:
    void clearGrid();
    void fillGrid(std::vector<ImageLabel*> images);

    BoxImage *box;
    ImageLabel *copiedImage;

    std::vector<ImageLabel*> oldImages;
    std::vector<ImageLabel*> newImages;
};

#endif // UNDOPASTE_H
