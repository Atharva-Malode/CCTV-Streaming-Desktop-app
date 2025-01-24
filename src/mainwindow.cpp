#include "include/mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    ui.setupUi(this);
}

MainWindow::~MainWindow() {
}

void MainWindow::displayImage(const cv::Mat& image) {
    // Convert OpenCV Mat to QImage
    cv::Mat rgbImage;
    cv::cvtColor(image, rgbImage, cv::COLOR_BGR2RGB);
    QImage qimg(rgbImage.data, rgbImage.cols, rgbImage.rows, rgbImage.step, QImage::Format_RGB888);
    
    // Scale image to fit label while maintaining aspect ratio
    ui.label->setScaledContents(true);
    ui.label->setPixmap(QPixmap::fromImage(qimg));
}

void MainWindow::on_pushButton_clicked() {
    // Load a sample image
    cv::Mat sampleImage = cv::imread("C:/Users/HP/Pictures/Screenshots/Screenshot 2024-04-06 160259.png");
    
    if (!sampleImage.empty()) {
        displayImage(sampleImage);
    } else {
        // Handle error if image can't be loaded
        ui.label->setText("Failed to load image");
    }
}
