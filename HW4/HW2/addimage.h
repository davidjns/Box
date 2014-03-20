#ifndef ADDIMAGE_H
#define ADDIMAGE_H

#include "boximage.h"
#include <QUndoCommand>

class AddImage : public QObject, public QUndoCommand
{
    Q_OBJECT
public:
    explicit AddImage(QObject *parent = 0);
    AddImage(BoxImage *_box, QLabel *_image);

    void undo();
    void redo();

private:
    BoxImage *box;
    QLabel *image;

signals:

public slots:

};

#endif // ADDIMAGE_H
