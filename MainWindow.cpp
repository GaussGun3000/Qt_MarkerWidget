#include "MainWindow.h"
#include <qmessagebox.h>
#include <qfiledialog.h>
#include "XMLLoader.h"
#include <qpainter.h>
#include <qevent.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);    
    connect(ui.markerArea, &MarkerWidget::scaleChanged, this, &MainWindow::updateScale);
}

MainWindow::~MainWindow()
{
}
void MainWindow::loadButtonClicked()
{
    //file dialog
    XMLLoader xml = XMLLoader();
    std::string status;
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open XML"), QDir::currentPath(), tr("XML Files (*.xml)"));
    if (!fileName.isEmpty())
    {
        std::string filePath = fileName.toStdString();
        auto data = xml.readXML(filePath, &status);
        if (!data.empty())
        {
            ui.statusBar->showMessage((std::string("File loaded, total points: ") + std::to_string(data.size())).c_str());
            ui.markerArea->render(data);
        }
        else
        {
            status = "XML Error: " + status;
            ui.statusBar->showMessage(status.c_str());
        }
    }
}

void MainWindow::aboutButtonClicked()
{
    QMessageBox::aboutQt(this, "About application");
}

void MainWindow::updateScale()
{
    ui.scaleLabel->setText(ui.markerArea->getScale().c_str());
}

void MainWindow::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.drawRect(ui.markerArea->x(), ui.markerArea->y(), ui.markerArea->width(), ui.markerArea->height());
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    ui.markerArea->render();
    event->accept();
}
