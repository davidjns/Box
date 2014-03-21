#include "boximage.h"
#include "imagebox.h"
#include "imagelabel.h"
//#include "flickrcollector.h"
#include <QAction>
#include <QGridLayout>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QSize>
#include <QSplitter>
#include <QScrollArea>
#include <QToolButton>
#include <QFileDialog>
#include <QLabel>
#include <QPixmap>

ImageBox::ImageBox()
    : QMainWindow()
{
    splitter = new QSplitter(Qt::Horizontal);
    setCentralWidget(splitter);

    boxImage = new BoxImage(this, this);

    previewImage = new QLabel();

    initializeActions();
    initializeMenus();
    initializeButtonsGrid();
    initializeImagesGrid();
}

void ImageBox::initializeActions()
{
    fileNew = new QAction(tr("&New"), this);
    fileNew->setShortcuts(QKeySequence::New);
    connect(fileNew, SIGNAL(triggered()), this, SLOT(newFile()));

    fileOpen = new QAction(tr("&Open"), this);
    fileOpen->setShortcuts(QKeySequence::Open);
    connect(fileOpen, SIGNAL(triggered()), this, SLOT(open()));

    fileSave = new QAction(tr("&Save"), this);
    fileSave->setShortcuts(QKeySequence::Save);
    connect(fileSave, SIGNAL(triggered()), this, SLOT(save()));

    fileSaveAs = new QAction(tr("&Save as"), this);
    fileSaveAs->setShortcut(QKeySequence("Ctrl+Shift+S"));
    connect(fileSaveAs, SIGNAL(triggered()), this, SLOT(saveAs()));

    fileQuit = new QAction(tr("&Quit"), this);
    fileQuit->setShortcut(QKeySequence("Ctrl+W"));
    connect(fileQuit, SIGNAL(triggered()), this, SLOT(quit()));

    editUndo = new QAction(tr("&Undo"), this);
    editUndo->setShortcuts(QKeySequence::Undo);
    connect(editUndo, SIGNAL(triggered()), boxImage, SLOT(undo()));

    editRedo = new QAction(tr("&Redo"), this);
    editRedo->setShortcuts(QKeySequence::Redo);
    connect(editRedo, SIGNAL(triggered()), boxImage, SLOT(redo()));

    editCut = new QAction(tr("&Cut"), this);
    editCut->setShortcuts(QKeySequence::Cut);
    editCut->setDisabled(true);
    connect(editCut, SIGNAL(triggered()), boxImage, SLOT(cut()));

    editCopy = new QAction(tr("&Copy"), this);
    editCopy->setShortcuts(QKeySequence::Copy);
    editCopy->setDisabled(true);
    connect(editCopy, SIGNAL(triggered()), boxImage, SLOT(copy()));

    editPaste = new QAction(tr("&Paste"), this);
    editPaste->setShortcuts(QKeySequence::Paste);
    editPaste->setDisabled(true);
    connect(editPaste, SIGNAL(triggered()), boxImage, SLOT(paste()));

    imAddCollection = new QAction(tr("&Add Collection"), this);
    imAddCollection->setShortcut(QKeySequence("F1"));
    connect(imAddCollection, SIGNAL(triggered()), boxImage, SLOT(addCollection()));

    imAddImage = new QAction(tr("&Add Image"), this);
    imAddImage->setShortcut(QKeySequence("F2"));
    connect(imAddImage, SIGNAL(triggered()), boxImage, SLOT(addImage()));

    imRemoveAll = new QAction(tr("&Remove All Images"), this);
    imRemoveAll->setShortcut(QKeySequence("F3"));
    imRemoveAll->setDisabled(true);
    connect(imRemoveAll, SIGNAL(triggered()), boxImage, SLOT(removeAll()));

    imRemoveImage = new QAction(tr("&Remove Image"), this);
    imRemoveImage->setShortcut(QKeySequence("F4"));
    imRemoveImage->setDisabled(true);
    connect(imRemoveImage, SIGNAL(triggered()), boxImage, SLOT(removeImage()));

    imAddFlickr = new QAction(tr("&Add Flickr Images"), this);
    imAddFlickr->setShortcut(QKeySequence("F5"));
    connect(imAddFlickr, SIGNAL(triggered()), boxImage, SLOT(addFlickr()));
}

void ImageBox::initializeMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(fileNew);
    fileMenu->addAction(fileOpen);
    fileMenu->addSeparator();
    fileMenu->addAction(fileSave);
    fileMenu->addAction(fileSaveAs);
    fileMenu->addSeparator();
    fileMenu->addAction(fileQuit);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(editUndo);
    editMenu->addAction(editRedo);
    editMenu->addSeparator();
    editMenu->addAction(editCut);
    editMenu->addAction(editCopy);
    editMenu->addAction(editPaste);

    imagesMenu = menuBar()->addMenu(tr("&Images"));
    imagesMenu->addAction(imAddCollection);
    imagesMenu->addAction(imAddImage);
    imagesMenu->addAction(imRemoveAll);
    imagesMenu->addAction(imRemoveImage);
    imagesMenu->addAction(imAddFlickr);
}

void ImageBox::initializeButtonsGrid()
{
    QWidget *previewGridWidget = new QWidget();
    previewGrid = new QGridLayout(splitter);

    previewGridWidget->setLayout(previewGrid->layout());
    previewGridWidget->setMaximumWidth(325);   //don't think i need this guy

    splitter->insertWidget(0, previewGridWidget);
    splitter->setStretchFactor(0, 1);

    //CODE BELOW FROM P3
//    buttonGrid = new QGridLayout(splitter);
//    buttonGrid->setSpacing(4);

//    QWidget *w = new QWidget();
//    w->setLayout(buttonGrid);

//    addCollectionButton = new QToolButton(this);
//    addCollectionButton->setDefaultAction(imAddCollection);
//    addCollectionButton->setText("Add Collection");
//    addCollectionButton->setFixedWidth(125);

//    addImageButton = new QToolButton(this);
//    addImageButton->setDefaultAction(imAddImage);
//    addImageButton->setText("Add Image");
//    addImageButton->setFixedWidth(125);

//    removeAllButton = new QToolButton(this);
//    removeAllButton->setDefaultAction(imRemoveAll);
//    removeAllButton->setText("Remove All");
//    removeAllButton->setFixedWidth(125);

//    removeImageButton = new QToolButton(this);
//    removeImageButton->setDefaultAction(imRemoveImage);
//    removeImageButton->setText("Remove Image");
//    removeImageButton->setFixedWidth(125);

//    buttonGrid->addWidget(addCollectionButton);
//    buttonGrid->addWidget(addImageButton);
//    buttonGrid->addWidget(removeAllButton);
//    buttonGrid->addWidget(removeImageButton);

//    buttonGrid->setAlignment(Qt::AlignLeft);
//    buttonGrid->setAlignment(Qt::AlignTop);
//    leftBox->addWidget(w);

//    QWidget *leftWidget = new QWidget();
//    leftWidget->setLayout(leftBox);

//    splitter->insertWidget(0, w);
//    splitter->setStretchFactor(0, 0);
}

void ImageBox::initializeImagesGrid()
{
    QWidget *imageGridWidget = new QWidget();
    imageGridWidget->setLayout(boxImage->layout());

    scrollArea = new QScrollArea(splitter);
    splitter->addWidget(scrollArea);

    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(imageGridWidget);

    splitter->insertWidget(1, scrollArea);
    splitter->setStretchFactor(1, 1);
}

void ImageBox::toggleRemove(int numImages)
{
    imRemoveAll->setDisabled(numImages == 0);
    imRemoveImage->setDisabled(numImages == 0);
}

void ImageBox::togglePaste(ImageLabel *copiedImage)
{
    editPaste->setDisabled(copiedImage == NULL);
}

void ImageBox::toggleCutCopy(ImageLabel *selectedImage)
{
    editCut->setDisabled(selectedImage == NULL);
    editCopy->setDisabled(selectedImage == NULL);

//    if(image == NULL)
//    {
//        editCut->setDisabled(true);
//        editCopy->setDisabled(true);
//    }
//    else
//    {
//        editCut->setDisabled(false);
//        editCopy->setDisabled(false);
//    }
}

void ImageBox::actionStub(QString call)
{
    QMessageBox *messageBox = new QMessageBox();
    messageBox->setModal(true);
    messageBox->setText("Not implemented yet");
    messageBox->setWindowTitle(call);
    messageBox->exec();
    this->setFocus();
}

void ImageBox::showSelected(ImageLabel *image)
{
    if(image == NULL)
    {
        previewImage->hide();
        return;
    }

    const QPixmap *pixmap = image->pixmap();
    previewImage->setPixmap(pixmap->scaled(300, 300));
    previewImage->show();

    previewGrid->addWidget(previewImage, 1, 1);
}
