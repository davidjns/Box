#ifndef UNDOADDFLICKR_H
#define UNDOADDFLICKR_H

class BoxImage;
class ImageLabel;
class QLabel;
class QNetworkAccessManager;
class QNetworkReply;

#include <QUndoCommand>
#include <QStringList>
#include <vector>

class UndoAddFlickr : public QUndoCommand
{
public:
    UndoAddFlickr(BoxImage *_box, QStringList _urls, std::vector<ImageLabel*> _oldImages);
    UndoAddFlickr(BoxImage *_box, std::vector<ImageLabel*> _oldImages, std::vector<ImageLabel*> _newImages);

    void undo();
    void redo();

private:
    BoxImage *box;
    QStringList urls;

    std::vector<ImageLabel*> oldImages;
    std::vector<ImageLabel*> newImages;

    QNetworkAccessManager *nam;

    int numFinished;
    bool undone;

public slots:
    void replyFinished(QNetworkReply*);

};

#endif // UNDOADDFLICKR_H
