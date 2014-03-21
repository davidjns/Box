#ifndef BOXIMAGE_H
#define BOXIMAGE_H

class UndoAddImages;
class UndoRemoveImage;
class UndoRemoveImages;
class UndoCut;
class UndoCopy;
class UndoPaste;
class QLabel;
class ImageLabel;
class ImageBox;
class QClipboard;
class QTimer;
class FlickrCollector;

#include <QGridLayout>
#include <QUndoStack>
#include <QNetworkAccessManager>
#include <list>
#include <vector>

class BoxImage : public QGridLayout
{
    friend class UndoAddImages;
    friend class UndoRemoveImages;
    friend class UndoRemoveImage;
    friend class UndoCut;
    friend class UndoCopy;
    friend class UndoPaste;

    Q_OBJECT
public:
    explicit BoxImage(QWidget *parent = 0);
    BoxImage(QWidget *_parent, ImageBox *_parentBox);

    void imageClick(ImageLabel *nextSelect);

    ImageLabel* getSelected()
        { return selected; }
    void resetSelected()
        { selected = NULL; }

    ImageLabel* getCopied();
        //{ return copied; }
    void setCopied(ImageLabel *image);
        //{ copied = image; }
    void resetCopied()
        { copied = NULL; }

    void clearGrid();
    void fillGrid(std::vector<ImageLabel*> images);

    void setFlickrReady();

private:
    QWidget *parent;
    ImageBox *parentBox;
    std::vector<ImageLabel*> images;

    int numImages;

    QUndoStack *stack;

    ImageLabel *selected;

    ImageLabel *copied;

    QClipboard *clipboard;

    QNetworkAccessManager *nam;
    int numFinished;
    std::vector<ImageLabel*> newImages;

    FlickrCollector *collector;
    bool collectorReady;
    int waitingCalls;
    bool flickrReady;
    std::list<QStringList> waitingLists;

    QTimer *timer;
    int slideNum;

    void resetSlide();

    std::vector<ImageLabel*> copyImages();
    void fixBox();
    void resizeGrid();
    void badImage(QString fileName);

public slots:
    void undo()
        { stack->undo(); }
    void redo()
        { stack->redo(); }

    void addCollection();
    void addImage();
    void removeAll();
    void removeImage();

    void cut();
    void copy();
    void paste();

    void addFlickr();
    void addFlickrReady();
    void replyFinished(QNetworkReply*);

    void changeSlide();
    void playSlide();
    void stopSlide();
};

#endif // BOXIMAGE_H
