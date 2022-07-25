#include "ImageComposer.h"
#include <QGridLayout>
#include <QFileDialog>
static const QSize resultSize(200,200);
ImageComposer::ImageComposer()
{
    sourceButton = new QToolButton;
    sourceButton->setIconSize(resultSize);

    operatorComboBox = new QComboBox;
    addOp(QPainter::CompositionMode_SourceOver, tr("SourceOver"));
    addOp(QPainter::CompositionMode_DestinationOver, tr("DestinationOver"));
    addOp(QPainter::CompositionMode_Clear, tr("Clear"));
    addOp(QPainter::CompositionMode_Source, tr("Source"));
    addOp(QPainter::CompositionMode_Destination, tr("Destination"));
    addOp(QPainter::CompositionMode_SourceIn, tr("SourceIn"));
    addOp(QPainter::CompositionMode_DestinationIn, tr("DestinationIn"));
    addOp(QPainter::CompositionMode_SourceOut, tr("SourceOut"));
    addOp(QPainter::CompositionMode_DestinationOut, tr("DestinationOut"));
    addOp(QPainter::CompositionMode_SourceAtop, tr("SourceAtop"));
    addOp(QPainter::CompositionMode_DestinationAtop, tr("DestinationAtop"));
    addOp(QPainter::CompositionMode_Xor, tr("Xor"));
    addOp(QPainter::CompositionMode_Plus, tr("Plus"));
    addOp(QPainter::CompositionMode_Multiply, tr("Multiply"));
    addOp(QPainter::CompositionMode_Screen, tr("Screen"));
    addOp(QPainter::CompositionMode_Overlay, tr("Overlay"));
    addOp(QPainter::CompositionMode_Darken, tr("Darken"));
    addOp(QPainter::CompositionMode_Lighten, tr("Lighten"));
    addOp(QPainter::CompositionMode_ColorDodge, tr("ColorDodge"));
    addOp(QPainter::CompositionMode_ColorBurn, tr("ColorBurn"));
    addOp(QPainter::CompositionMode_HardLight, tr("HardLight"));
    addOp(QPainter::CompositionMode_SoftLight, tr("SoftLight"));
    addOp(QPainter::CompositionMode_Difference, tr("Difference"));
    addOp(QPainter::CompositionMode_Exclusion, tr("Exclusion"));

    destinationButton = new QToolButton;
    destinationButton->setIconSize(resultSize);

    equalLabel = new QLabel(tr("="));

    resultLabel = new QLabel;
    resultLabel->setMinimumWidth(resultSize.width());

    connect(sourceButton, &QAbstractButton::clicked,
            this, &ImageComposer::chooseSource);
    connect(operatorComboBox, QOverload<int>::of(&QComboBox::activated),
            this, &ImageComposer::recalculateResult);
    connect(destinationButton, &QAbstractButton::clicked,
            this, &ImageComposer::chooseDestination);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(sourceButton, 0, 0, 3, 1);
    mainLayout->addWidget(operatorComboBox, 1, 1);
    mainLayout->addWidget(destinationButton, 0, 2, 3, 1);
    mainLayout->addWidget(equalLabel, 1, 3);
    mainLayout->addWidget(resultLabel, 0, 4, 3, 1);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(mainLayout);

    resultImage = QImage(resultSize, QImage::Format_ARGB32_Premultiplied);
    //":/image/qrc/image/tree2.jpeg"
    loadImage(":/image/qrc/image/water.jpeg", &sourceImage, sourceButton);
    loadImage(":/image/qrc/image/tree.jpeg", &destinationImage, destinationButton);

    setWindowTitle(tr("Image Composition"));
}


void ImageComposer::chooseSource()
{
    chooseImage(tr("Choose Source Image"), &sourceImage, sourceButton);
}

void ImageComposer::chooseDestination()
{
    chooseImage(tr("Choose Destination Image"), &destinationImage, destinationButton);
}

void ImageComposer::chooseImage(const QString &title, QImage *image,
                                QToolButton *button)
{
    QString fileName = QFileDialog::getOpenFileName(this, title);
    if (!fileName.isEmpty())
        loadImage(fileName, image, button);
}

void ImageComposer::recalculateResult()
{
    QPainter::CompositionMode mode = currentMode();
    QPainter painter(&resultImage);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(resultImage.rect(), Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(0, 0, destinationImage);
    painter.setCompositionMode(mode);
    painter.drawImage(0, 0, sourceImage);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
    painter.fillRect(resultImage.rect(), Qt::white);
    painter.end();
    resultLabel->setPixmap(QPixmap::fromImage(resultImage));
}


void ImageComposer::addOp(QPainter::CompositionMode mode, const QString &name)
{
    operatorComboBox->addItem(name, mode);
}


void ImageComposer::loadImage(const QString &fileName, QImage *image,
                              QToolButton *button)
{
    image->load(fileName);

    // Scale the image to given size
    *image = image->scaled(resultSize, Qt::KeepAspectRatio);

    QImage fixedImage(resultSize, QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&fixedImage);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(fixedImage.rect(), Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(imagePos(*image), *image);
    painter.end();
    button->setIcon(QPixmap::fromImage(fixedImage));

    *image = fixedImage;

    recalculateResult();
}

QPainter::CompositionMode ImageComposer::currentMode() const
{
    return (QPainter::CompositionMode)
           operatorComboBox->itemData(operatorComboBox->currentIndex()).toInt();
}

QPoint ImageComposer::imagePos(const QImage &image) const
{
    return QPoint((resultSize.width() - image.width()) / 2,
                  (resultSize.height() - image.height()) / 2);
}
