#ifndef IMAGECOMPOSER_H
#define IMAGECOMPOSER_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QToolButton>
#include <QComboBox>
#include <QLabel>

class ImageComposer:public QWidget
{
public:
    ImageComposer();
    Q_OBJECT
private slots:
    void chooseSource();
    void chooseDestination();
    void recalculateResult();

private:
    void addOp(QPainter::CompositionMode mode, const QString &name);
    void chooseImage(const QString &title, QImage *image, QToolButton *button);
    void loadImage(const QString &fileName, QImage *image, QToolButton *button);
    QPainter::CompositionMode currentMode() const;
    QPoint imagePos(const QImage &image) const;

    QToolButton *sourceButton;
    QToolButton *destinationButton;
    QComboBox *operatorComboBox;
    QLabel *equalLabel;
    QLabel *resultLabel;

    QImage sourceImage;
    QImage destinationImage;
    QImage resultImage;
    static const int a=0;
};

#endif // IMAGECOMPOSER_H
