#ifndef IMAGEBOX_H
#define IMAGEBOX_H

class BoxImage;
class ImageLabel;
class QGridLayout;
class QMenu;
class QPushButton;
class QScrollArea;
class QSplitter;
class QToolButton;
class QLabel;

#include <QMainWindow>
#include <QApplication>
#include <QAction>

class ImageBox : public QMainWindow
{
    Q_OBJECT

public:
    ImageBox();
    void toggleRemove(int num);
    void togglePaste(ImageLabel *copiedImage);
    void toggleCutCopy(ImageLabel *selectedImage);

    void togglePasteOn()
        { editPaste->setDisabled(false); }

    void showSelected(ImageLabel* image);
    void updateSlide(ImageLabel *image);

private:
    void initializeActions();
    void initializeMenus();
    void initializeButtonsGrid();
    void initializeImagesGrid();

    void actionStub(QString call);

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *imagesMenu;
    QMenu *slideMenu;

    QSplitter *splitter;
    QGridLayout *buttonGrid;
    QGridLayout *imageGrid;
    QScrollArea *scrollArea;
    BoxImage *boxImage;

    QGridLayout *previewGrid;
    QLabel *previewImage;
    QLabel *slideImage;

    QToolButton *addCollectionButton;
    QToolButton *addImageButton;
    QToolButton *removeAllButton;
    QToolButton *removeImageButton;

    QAction *fileNew;
    QAction *fileOpen;
    QAction *fileSave;
    QAction *fileSaveAs;
    QAction *fileQuit;
    QAction *editUndo;
    QAction *editRedo;
    QAction *editCut;
    QAction *editCopy;
    QAction *editPaste;
    QAction *imAddCollection;
    QAction *imAddImage;
    QAction *imRemoveAll;
    QAction *imRemoveImage;
    QAction *imAddFlickr;

    QAction *slidePlay;
    QAction *slideStop;
};

#endif // IMAGEBOX_H
