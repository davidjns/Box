#ifndef UNDOADD_H
#define UNDOADD_H

class BoxImage;
class ImageLabel;
class QLabel;

#include <vector>
#include <QUndoCommand>

class undoAdd : public QUndoCommand
{
public:
    explicit UndoAdd(QObject *parent = 0);
    UndoAdd(BoxImage *_box, std::vector<ImageLabel*> _images);

    void undo();
    void redo();

private:
    BoxImage *box;
    std::vector<ImageLabel*> images;
};

#endif // UNDOADD_H
