#include "boximage.h"
#include "imagebox.h"
#include "imagelabel.h"
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

    initializeActions();
    initializeMenus();
    initializeDisplayGrid();
    initializeImagesGrid();

    togglePaste(boxImage->getCopied());
}

void ImageBox::initializeActions()
{
    fileNew = new QAction(tr("&New"), this);
    fileNew->setShortcuts(QKeySequence::New);
    connect(fileNew, SIGNAL(triggered()), boxImage, SLOT(newFile()));

    fileOpen = new QAction(tr("&Open"), this);
    fileOpen->setShortcuts(QKeySequence::Open);
    connect(fileOpen, SIGNAL(triggered()), boxImage, SLOT(open()));

    fileSave = new QAction(tr("&Save"), this);
    fileSave->setShortcuts(QKeySequence::Save);
    connect(fileSave, SIGNAL(triggered()), boxImage, SLOT(save()));

    fileSaveAs = new QAction(tr("&Save as"), this);
    fileSaveAs->setShortcut(QKeySequence("Ctrl+Shift+S"));
    connect(fileSaveAs, SIGNAL(triggered()), boxImage, SLOT(saveAs()));

    fileQuit = new QAction(tr("&Quit"), this);
    fileQuit->setShortcut(QKeySequence("Ctrl+W"));
    connect(fileQuit, SIGNAL(triggered()), boxImage, SLOT(quit()));

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

    slidePlay = new QAction(tr("&Play Slide Show"), this);
    slidePlay->setShortcut(QKeySequence("P"));
    connect(slidePlay, SIGNAL(triggered()), boxImage, SLOT(playSlide()));

    slideStop = new QAction(tr("&Stop Slide Show"), this);
    slideStop->setShortcut(QKeySequence("Shift+P"));
    connect(slideStop, SIGNAL(triggered()), boxImage, SLOT(stopSlide()));
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

    slideMenu = menuBar()->addMenu(tr("&Slide Show"));
    slideMenu->addAction(slidePlay);
    slideMenu->addAction(slideStop);
}

void ImageBox::initializeDisplayGrid()
{
    QWidget *previewGridWidget = new QWidget();
    previewGrid = new QGridLayout(splitter);

    previewGridWidget->setLayout(previewGrid->layout());
    previewGridWidget->setMaximumWidth(300);   //don't think i need this guy

    //START Trying to add labels
    previewImage = new QLabel();
    previewImage->setFixedSize(300, 300);
    previewImage->setText("Nothing Selected");
    previewImage->setAlignment(Qt::AlignCenter);
    previewHeader = new QLabel("Selected Image");
    previewHeader->setFixedHeight(16);
    previewHeader->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    QFont font;
    font.setUnderline(true);
    font.setBold(true);
    font.setPixelSize(14);
    previewHeader->setFont(font);

    slideImage = new QLabel();
    slideImage->setFixedSize(300, 150);
    slideImage->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    slideHeader = new QLabel("Slide Show");
    slideHeader->setFixedHeight(16);
    slideHeader->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    slideHeader->setFont(font);


    previewGrid->addWidget(previewHeader, 1, 1);
    previewGrid->addWidget(previewImage, 2, 1);
    previewGrid->addWidget(slideHeader, 3, 1);
    previewGrid->addWidget(slideImage, 4, 1);
    //END

    splitter->insertWidget(0, previewGridWidget);
    splitter->setStretchFactor(0, 1);
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
        QPixmap *blank = new QPixmap();
        previewImage->setPixmap(*blank);
        previewImage->setText("Nothing Selected");
        return;
    }

    const QPixmap *pixmap = image->pixmap();
    previewImage->setPixmap(pixmap->scaled(300, 300));
    previewImage->show();
}

void ImageBox::updateSlide(ImageLabel *image)
{
    if(image == NULL)
    {
        QPixmap *blank = new QPixmap();
        slideImage->setPixmap(*blank);
        return;
    }

    const QPixmap *pixmap = image->pixmap();
    slideImage->setPixmap(pixmap->scaled(150, 150));
    slideImage->show();
}
