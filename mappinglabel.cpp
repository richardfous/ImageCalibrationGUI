#include "mappinglabel.h"
#include "ui_mappinglabel.h"

MappingLabel::MappingLabel(QWidget *parent) :
    QLabel(parent),
    ui(new Ui::MappingLabel)
{
    ui->setupUi(this);
    this->setAlignment(Qt::AlignTop);

}

MappingLabel::~MappingLabel()
{
    delete ui;
}

void MappingLabel::mousePressEvent(QMouseEvent *e){

    if(m_enabledClick == true && e->button() == Qt::LeftButton){

        if(e->pos().x() < m_backgroundImage.cols*m_scaleFactor && e->pos().y() < m_backgroundImage.rows*m_scaleFactor){


            if(m_isFootball == true){

                m_mappingPoint.x = e->pos().x()/m_scaleFactor;
                m_mappingPoint.y = e->pos().y()/m_scaleFactor;

                m_mappingPoint = findCorrectPoint(m_mappingPoint,m_footballPoints);

            }else if(m_isCustom == true){
                m_mappingPoint.x = e->pos().x()/m_scaleFactor;
                m_mappingPoint.y = e->pos().y()/m_scaleFactor;
                m_mappingPoint = findCorrectPoint(m_mappingPoint,m_inputMappingPoints);
            }else{
                float xFactor = m_backgroundImage.cols*m_scaleFactor/m_width;
                float yFactor = m_backgroundImage.rows*m_scaleFactor/m_height;

                m_mappingPoint.x = e->pos().x()/xFactor;
                m_mappingPoint.y = e->pos().y()/yFactor;

                m_mappingPoint = findCorrectPoint(m_mappingPoint,m_inputMappingPoints);

            }
            emit signalMappingPoint();
            emit signalActivateImageClick();
            m_enabledClick = false;
        }
    }else if(e->button() == Qt::RightButton){
        emit signalClearMappingPont();
    }

}

void MappingLabel::setBackgroundImage(cv::Mat image){

    m_backgroundImage = image.clone();

}

void MappingLabel::loadFromQrc(QString qrc,bool firstLoad, int flag )
{

    QFile file(qrc);
    if(file.open(QIODevice::ReadOnly))
    {
        qint64 sz = file.size();
        std::vector<uchar> buf(sz);
        file.read((char*)buf.data(), sz);
        m_backgroundImage = cv::imdecode(buf, flag);
        if(firstLoad){
            displayBackgroundImage(m_backgroundImage);
        }
    }else{return;}


}

void MappingLabel::displayBackgroundImage(cv::Mat inputImage){

    cv::Mat image = inputImage.clone();

    if(image.empty()){
        return;
    }

    cv::cvtColor(image,image,cv::COLOR_BGR2RGB);
    m_newPixmap = QPixmap::fromImage(QImage(image.data, image.cols, image.rows, image.step, QImage::Format_RGB888));
    this->setPixmap(m_newPixmap.scaled(this->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    m_aspectRationImage = static_cast<float>(image.cols)/image.rows;
    m_aspectRationLabel = static_cast<float>(this->size().width())/this->size().height();

    if(m_aspectRationImage < m_aspectRationLabel){

        m_scaleFactor = static_cast<float>(this->size().height())/image.rows;

    }else if(m_aspectRationImage > m_aspectRationLabel)
        {
        m_scaleFactor = static_cast<float>(this->size().width())/image.cols;
    }else{m_scaleFactor = 1.0;}

}

void MappingLabel::displayPoints(std::vector<cv::Point2f> pairedMappingPoints, bool isSet){

    cv::Mat temp = m_backgroundImage.clone();
    cv::Point2i tempPoint;

    for(uint x = 0; x < m_inputMappingPoints.size(); x++ ){

        if(m_isFootball){
            tempPoint.x = std::round(m_footballPoints[x].x);
            tempPoint.y = std::round(m_footballPoints[x].y);
        }else if(m_isCustom){
            tempPoint.x = std::round(m_inputMappingPoints[x].x);
            tempPoint.y = std::round(m_inputMappingPoints[x].y);
        }else{
            float x1Factor = m_backgroundImage.cols/m_width;
            float y1Factor = m_backgroundImage.rows/m_height;

            tempPoint.x = std::round((m_inputMappingPoints[x].x/m_pointScaleFactor)*x1Factor);
            tempPoint.y = std::round((m_inputMappingPoints[x].y/m_pointScaleFactor)*y1Factor);
        }

    bool found = false;
    for(uint y = 0; y < pairedMappingPoints.size(); y++){

        if(m_inputMappingPoints[x] == pairedMappingPoints[y]){

            if(isSet == true){
                cv::circle(temp,tempPoint,5,{255,0,0},-1,cv::LINE_AA);
                found = true;
                break;
            }else{
                cv::circle(temp,tempPoint,5,{147,205,255},-1,cv::LINE_AA);
                found = true;
                break;
                }

        }
    }
    if(!found){
        cv::circle(temp,tempPoint,5,{0,0,255},-1,cv::LINE_AA);

    }
    }

  displayBackgroundImage(temp);

}

cv::Point2f MappingLabel::getMappingPoint(){

    return m_mappingPoint;

}

void MappingLabel::resetMappingPoint(){

    m_mappingPoint = cv::Point2f(0.0f,0.0f);

}

bool MappingLabel::getEnabledClick(){

    return m_enabledClick;

}

void MappingLabel::setEnabledClick(bool status){

    m_enabledClick = status;

}

void MappingLabel::setWidth(float width){

    m_width = width;

}

void MappingLabel::setHeight(float height){

    m_height = height;

}

float MappingLabel::getWidth(){

    return m_width;

}

float MappingLabel::getHeight(){

    return m_height;

}

void MappingLabel::setIsFootball(bool isFootaball){

    m_isFootball = isFootaball;
    storeFootballPoint();

}

bool MappingLabel::getIsFootball(){

    return m_isFootball;

}

void MappingLabel::setIsCustom(bool isCustom){

    m_isCustom = isCustom;

}

bool MappingLabel::getIsCustom(){

    return m_isCustom;

}

void MappingLabel::setInputMappingPoints(std::vector<cv::Point2f> input){

    m_inputMappingPoints = input;

}

cv::Point2f MappingLabel::findCorrectPoint(cv::Point2f point, std::vector<cv::Point2f> inputVector){

    std::vector<cv::Point2f> vector = inputVector;

    if(m_isFootball == false && m_isCustom == false){

       for(auto &i : vector){
           i.x /= m_pointScaleFactor;
           i.y /= m_pointScaleFactor;
       }

    }

    float distance = static_cast<float>(cv::norm(point-vector[2]));
    float tempDistance = 0.0;
    int index = 0;

    for(uint x = 0; x < vector.size(); x++){

         tempDistance = static_cast<float>(cv::norm(point-vector[x]));
         if(tempDistance <= distance && tempDistance > 0.01){
             distance = tempDistance;
             index = x;

         }

    }
    return m_inputMappingPoints[index];
}

void MappingLabel::setPointScaleFactor(float scaleFactor){

    m_pointScaleFactor = scaleFactor;

}

float MappingLabel::getPointScaleFactor(){

    return m_pointScaleFactor;

}

void MappingLabel::storeFootballPoint(){

    m_footballPoints = {

        { 0.0f, 0.0f },
        { 27.0f, 0.0f },
        { 75.0f, 0.0f },
        { 294.0f, 0.0f },
        { 512.0f, 0.0f },
        { 560.0f, 0.0f },
        { 587.0f, 0.0f },

        { 0.0f, 27.0f },
        { 587.0f, 27.0f },

        { 0.0f, 75.0f },
        { 587.0f, 75.0f },

        { 0.0f, 127.0f },
        { 68.0f, 127.0f },
        { 519.0f, 127.0f },
        { 587.0f, 127.0f },

        { 294.0f, 157.0f },

        { 0.0f, 163.0f },
        { 35.0f, 163.0f },
        { 552.0f, 163.0f },
        { 587.0f, 163.0f },

        { 53.0f, 200.0f },
        { 294.0f, 200.0f },
        { 534.0f, 200.0f },

        { 0.0f, 239.0f },
        { 35.0f, 239.0f },
        { 552.0f, 239.0f },
        { 587.0f, 239.0f },

        { 294.0f, 243.0f },

        { 0.0f, 274.0f },
        { 68.0f, 274.0f },
        { 519.0f, 274.0f },
        { 587.0f, 274.0f },

        { 0.0f, 325.0f },
        { 587.0f, 325.0f },

        { 0.0f, 373.0f },
        { 587.0f, 373.0f },


        { 0.0f, 400.0f },
        { 27.0f, 400.0f },
        { 75.0f, 400.0f },
        { 294.0f, 400.0f },
        { 512.0f, 400.0f },
        { 560.0f, 400.0f },
        { 587.0f, 400.0f }

    };

}
