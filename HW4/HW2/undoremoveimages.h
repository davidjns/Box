#ifndef UNDOREMOVEIMAGES_H
#define UNDOREMOVEIMAGES_H

class BoxImage;
class ImageLabel;
class QLabel;

#include <vector>
#include <QUndoCommand>

class UndoRemoveImages : public QUndoCommand
{
public:
    explicit UndoRemoveImages(QObject *parent = 0);
    UndoRemoveImages(BoxImage *_box, std::vector<ImageLabel*> _images);

    void undo();
    void redo();

private:
    BoxImage *box;
    std::vector<ImageLabel*> images;

signals:

public slots:

};

#endif // UNDOREMOVEIMAGES_H
