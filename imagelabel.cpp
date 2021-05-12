#include "imagelabel.h"
#include "ui_imagelabel.h"

ImageLabel::ImageLabel(QWidget *parent) :
    QLabel(parent),
    ui(new Ui::imageLabel)
{
    ui->setupUi(this);
    this->setAlignment(Qt::AlignTop);

}

ImageLabel::~ImageLabel()
{
    delete ui;
}

void ImageLabel::mousePressEvent(QMouseEvent *e){


    if(m_enabledClick && e->button() == Qt::LeftButton){
        if(e->pos().x() < m_backgroundImage.cols*m_scaleFactor && e->pos().y() < m_backgroundImage.rows*m_scaleFactor){
            m_imagePoint.x = (e->pos().x()/m_scaleFactor);
            m_imagePoint.y = (e->pos().y()/m_scaleFactor);
            emit signalImagePoint();
        }

   }else if(e->button() == Qt::RightButton){
            emit signalClearImagePoint();
        }
}

void ImageLabel::loadBackgroundImage(std::string path){

    m_backgroundImage = cv::imread(path);
    if(m_backgroundImage.empty()){

        return;

    }
    setBackgroundImage(m_backgroundImage);

}

void ImageLabel::changeDefaultBackgroundImage(cv::Mat inputImage){

    m_backgroundImage = inputImage.clone();
    if(m_backgroundImage.empty()){

        return;

    }
    setBackgroundImage(m_backgroundImage);

}

void ImageLabel::setBackgroundImage(cv::Mat inputImage){

    cv::Mat image = inputImage.clone();

    if(image.empty()){
        return;
    }

    cv::cvtColor(image,image,cv::COLOR_BGR2RGB);

    m_newPixmap = QPixmap::fromImage(QImage(image.data, image.cols, image.rows, image.step, QImage::Format_RGB888));
    this->setPixmap(m_newPixmap.scaled(this->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);


    m_aspectRationImage = static_cast<double>(image.cols)/image.rows;
    m_aspectRationLabel = static_cast<double>(this->size().width())/this->size().height();

    if(m_aspectRationImage < m_aspectRationLabel){

        m_scaleFactor = static_cast<double>(this->size().height())/image.rows;

    }else if(m_aspectRationImage > m_aspectRationLabel)
        {
        m_scaleFactor = static_cast<double>(this->size().width())/image.cols;
    }else{m_scaleFactor = 1.0;}

}

void ImageLabel::activateImageClick(){

    m_enabledClick = true;

}

void ImageLabel::resetImagePoint(){

    m_imagePoint = cv::Point2f();

}

cv::Point2f ImageLabel::getImagePoint(){

    return m_imagePoint;

}

void ImageLabel::deactivateImageClick(){

    m_enabledClick = false;

}

cv::Mat ImageLabel::getBackgroundImage(){

    return m_backgroundImage;

}

void ImageLabel::displayPoints(std::vector<cv::Point2f> pairedImagePoints){

    cv::Mat temp = m_backgroundImage.clone();
    cv::Point2i tempPoint;

    for(uint x = 0; x < pairedImagePoints.size(); x++ ){

            tempPoint.x = std::round(pairedImagePoints[x].x);
            tempPoint.y = std::round(pairedImagePoints[x].y);

            cv::circle(temp,tempPoint,5,{255,0,0},-1,cv::LINE_AA);

    }

  setBackgroundImage(temp);

}
