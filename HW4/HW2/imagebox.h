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

private:
    void initializeActions();
    void initializeMenus();
    void initializeButtonsGrid();
    void initializeImagesGrid();

    void actionStub(QString call);

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *imagesMenu;

    QSplitter *splitter;
    QGridLayout *buttonGrid;
    QGridLayout *imageGrid;
    QScrollArea *scrollArea;
    BoxImage *boxImage;

    QGridLayout *previewGrid;
    QLabel *previewImage;

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

private slots:
    void quit()     { QApplication::quit(); }

    //Unimplemented Menu Functions
    void newFile()  { actionStub("New File"); }
    void open()     { actionStub("Open File"); }
    void save()     { actionStub("Save File"); }
    void saveAs()   { actionStub("Save As File"); }
};

#endif // IMAGEBOX_H
