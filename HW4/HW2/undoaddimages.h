#ifndef UNDOADDIMAGES_H
#define UNDOADDIMAGES_H

class BoxImage;
class ImageLabel;
class QLabel;

#include <vector>
#include <QUndoCommand>

class UndoAddImages : public QUndoCommand
{
public:
    explicit UndoAddImages(QObject *parent = 0);
    UndoAddImages(BoxImage *_box, std::vector<ImageLabel*> _images);

    void undo();
    void redo();

private:
    BoxImage *box;
    std::vector<ImageLabel*> images;

signals:

public slots:
};

#endif // UNDOADDIMAGES_H
