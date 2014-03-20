#ifndef UNDOADDFLICKR_H
#define UNDOADDFLICKR_H

class BoxImage;
class ImageLabel;
class QLabel;
class QNetworkAccessManager;

#include <QUndoCommand>
#include <vector>

class UndoAddFlickr : public QUndoCommand
{
public:
    UndoAddFlickr(BoxImage *_box, QStringList _urls, std::vector<ImageLabel*> _oldImages);

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
